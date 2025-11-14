const STORAGE_KEY = "sartoriaData_v1";

let state = {
  clients: [], // {id,name,phone,email}
  orders: [], // {id,clientId,itemType,measurements,dueDate,price,status,notes,createdAt}
};

function uid(prefix = "id") {
  return (
    prefix +
    "_" +
    Date.now().toString(36) +
    Math.random().toString(36).slice(2, 7)
  );
}

function loadState() {
  const raw = localStorage.getItem(STORAGE_KEY);
  if (raw) {
    try {
      state = JSON.parse(raw);
    } catch (e) {
      console.error("Errore parsing state", e);
    }
  }
}

function saveState() {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
}

function findOrCreateClient(name, phone, email) {
  name = (name || "").trim();
  let client = state.clients.find(
    (c) => c.name.toLowerCase() === name.toLowerCase() && c.email === email
  );
  if (!client) {
    client = { id: uid("c"), name, phone, email };
    state.clients.push(client);
  }
  return client;
}

function addOrder(data) {
  const client = findOrCreateClient(
    data.clientName,
    data.clientPhone,
    data.clientEmail
  );
  const order = {
    id: uid("o"),
    clientId: client.id,
    itemType: data.itemType || "",
    measurements: data.measurements || "",
    dueDate: data.dueDate || "",
    price: parseFloat(data.price) || 0,
    status: data.status || "in-lavorazione",
    notes: data.notes || "",
    createdAt: new Date().toISOString(),
  };
  state.orders.unshift(order);
  saveState();
  render();
}

function updateOrder(id, fields) {
  const o = state.orders.find((x) => x.id === id);
  if (!o) return;
  Object.assign(o, fields);
  saveState();
  render();
}

function deleteOrder(id) {
  state.orders = state.orders.filter((o) => o.id !== id);
  saveState();
  render();
}

function render() {
  renderOrders();
  renderClients();
}

function formatDate(d) {
  if (!d) return "";
  const dt = new Date(d);
  if (isNaN(dt)) return d;
  return dt.toLocaleDateString();
}

function renderOrders() {
  const tbody = document.querySelector("#ordersTable tbody");
  if (!tbody) return;
  const search = document
    .getElementById("searchInput")
    .value.trim()
    .toLowerCase();
  const filterStatus = document.getElementById("filterStatus").value;

  tbody.innerHTML = "";
  const rows = state.orders.filter((o) => {
    if (filterStatus && o.status !== filterStatus) return false;
    if (!search) return true;
    const client = state.clients.find((c) => c.id === o.clientId) || {};
    const hay = (
      client.name +
      " " +
      o.itemType +
      " " +
      o.notes +
      " " +
      o.measurements
    ).toLowerCase();
    return hay.includes(search);
  });

  for (const o of rows) {
    const client = state.clients.find((c) => c.id === o.clientId) || {
      name: "—",
    };
    const tr = document.createElement("tr");
    tr.innerHTML = `
			<td>${o.id}</td>
			<td>${escapeHtml(client.name)}</td>
			<td>${escapeHtml(o.itemType)}</td>
			<td>${formatDate(o.dueDate)}</td>
			<td>€ ${o.price.toFixed(2)}</td>
			<td>${o.status}</td>
			<td class="actions">
				<button data-action="edit" data-id="${o.id}">Modifica</button>
				<button data-action="delete" data-id="${o.id}">Elimina</button>
				<button data-action="print" data-id="${o.id}">Stampa</button>
			</td>
		`;
    tbody.appendChild(tr);
  }
}

function renderClients() {
  const tbody = document.querySelector("#clientsTable tbody");
  tbody.innerHTML = "";
  for (const c of state.clients) {
    const lastOrder = state.orders.find((o) => o.clientId === c.id);
    const tr = document.createElement("tr");
    tr.innerHTML = `
			<td>${escapeHtml(c.name)}</td>
			<td>${escapeHtml(c.phone)}</td>
			<td>${escapeHtml(c.email)}</td>
			<td>${
        lastOrder
          ? formatDate(lastOrder.dueDate) +
            " — " +
            escapeHtml(lastOrder.itemType)
          : ""
      }</td>
		`;
    tbody.appendChild(tr);
  }
}

function escapeHtml(s) {
  if (!s && s !== 0) return "";
  return String(s)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#39;");
}

function bindEvents() {
  document.getElementById("orderForm").addEventListener("submit", (e) => {
    e.preventDefault();
    const f = e.target;
    const data = {
      clientName: f.clientName.value,
      clientPhone: f.clientPhone.value,
      clientEmail: f.clientEmail.value,
      itemType: f.itemType.value,
      measurements: f.measurements.value,
      dueDate: f.dueDate.value,
      price: f.price.value,
      status: f.status.value,
      notes: f.notes.value,
    };
    addOrder(data);
    f.reset();
  });

  document.getElementById("resetForm").addEventListener("click", () => {
    document.getElementById("orderForm").reset();
  });

  document
    .getElementById("searchInput")
    .addEventListener("input", renderOrders);
  document
    .getElementById("filterStatus")
    .addEventListener("change", renderOrders);

  document.getElementById("ordersTable").addEventListener("click", (e) => {
    const btn = e.target.closest("button");
    if (!btn) return;
    const id = btn.dataset.id;
    const action = btn.dataset.action;
    if (action === "delete") {
      if (confirm("Eliminare questo ordine?")) deleteOrder(id);
    } else if (action === "edit") {
      startEditOrder(id);
    } else if (action === "print") {
      printOrder(id);
    }
  });

  document.getElementById("clearStorage").addEventListener("click", () => {
    if (confirm("Eliminare tutti i dati locali?")) {
      localStorage.removeItem(STORAGE_KEY);
      state = { clients: [], orders: [] };
      render();
    }
  });
}

function startEditOrder(id) {
  const o = state.orders.find((x) => x.id === id);
  if (!o) return alert("Ordine non trovato");
  const client = state.clients.find((c) => c.id === o.clientId) || {};
  const f = document.getElementById("orderForm");
  f.clientName.value = client.name || "";
  f.clientPhone.value = client.phone || "";
  f.clientEmail.value = client.email || "";
  f.itemType.value = o.itemType || "";
  f.measurements.value = o.measurements || "";
  f.dueDate.value = o.dueDate ? o.dueDate.split("T")[0] : "";
  f.price.value = o.price || "";
  f.status.value = o.status || "in-lavorazione";
  f.notes.value = o.notes || "";

  // when saving, replace update listener temporarily
  const saveHandler = function (e) {
    e.preventDefault();
    updateOrder(id, {
      itemType: f.itemType.value,
      measurements: f.measurements.value,
      dueDate: f.dueDate.value,
      price: parseFloat(f.price.value) || 0,
      status: f.status.value,
      notes: f.notes.value,
    });
    // update client info
    const c = state.clients.find((c2) => c2.id === o.clientId);
    if (c) {
      c.name = f.clientName.value;
      c.phone = f.clientPhone.value;
      c.email = f.clientEmail.value;
    }
    saveState();
    f.reset();
    // restore submit handler
    f.removeEventListener("submit", saveHandler);
    f.addEventListener("submit", submitDefaultHandler);
  };

  // replace default submit handler
  f.removeEventListener("submit", submitDefaultHandler);
  f.addEventListener("submit", saveHandler);
}

// keep reference to default submit for swapping
function submitDefaultHandler(e) {
  e.preventDefault();
  const f = e.target;
  const data = {
    clientName: f.clientName.value,
    clientPhone: f.clientPhone.value,
    clientEmail: f.clientEmail.value,
    itemType: f.itemType.value,
    measurements: f.measurements.value,
    dueDate: f.dueDate.value,
    price: f.price.value,
    status: f.status.value,
    notes: f.notes.value,
  };
  addOrder(data);
  f.reset();
}

function printOrder(id) {
  const o = state.orders.find((x) => x.id === id);
  if (!o) return alert("Ordine non trovato");
  const c = state.clients.find((x) => x.id === o.clientId) || {};
  const win = window.open("", "_blank", "width=800,height=600");
  const html = `
		<html><head><title>Fattura ${o.id}</title>
		<style>
			body{font-family:sans-serif;padding:20px}
			.header{display:flex;justify-content:space-between;align-items:center}
			.table{width:100%;border-collapse:collapse;margin-top:20px}
			.table td, .table th{border:1px solid #ccc;padding:8px}
		</style>
		</head><body>
		<div class="header">
			<h2>Sartoria - Fattura/Note</h2>
			<div>Ordine: ${o.id}<br/>Data: ${new Date(o.createdAt).toLocaleString()}</div>
		</div>
		<hr/>
		<div>
			<strong>Cliente</strong><br/>
			${escapeHtml(c.name)}<br/>
			${escapeHtml(c.phone)} ${escapeHtml(c.email)}
		</div>
		<table class="table">
			<tr><th>Capo</th><td>${escapeHtml(o.itemType)}</td></tr>
			<tr><th>Misure</th><td>${escapeHtml(o.measurements)}</td></tr>
			<tr><th>Consegna</th><td>${formatDate(o.dueDate)}</td></tr>
			<tr><th>Prezzo</th><td>€ ${o.price.toFixed(2)}</td></tr>
			<tr><th>Note</th><td>${escapeHtml(o.notes)}</td></tr>
			<tr><th>Stato</th><td>${o.status}</td></tr>
		</table>
		<p style="margin-top:30px">Grazie per la preferenza.</p>
		<script>window.print()</script>
		</body></html>
	`;
  win.document.write(html);
  win.document.close();
}

// init
(function init() {
  loadState();
  bindEvents();
  // ensure default submit handler is attached
  const f = document.getElementById("orderForm");
  f.removeEventListener("submit", submitDefaultHandler);
  f.addEventListener("submit", submitDefaultHandler);
  render();
})();

// app.js - Frontend JavaScript completo per GestioneSartoria
const API_URL = "http://localhost:8080/api";

// Carica dati all'avvio
window.onload = function () {
  console.log("✅ Pagina caricata");
  caricaRotoli();
  mostraSezione("rotoli"); // Mostra sezione rotoli di default
};

/* ========================================
   GESTIONE NAVIGAZIONE TAB
======================================== */

function mostraSezione(nomeSezione) {
  // Nascondi tutte le sezioni
  const sezioni = document.querySelectorAll(".sezione");
  sezioni.forEach((s) => s.classList.remove("active"));

  // Rimuovi active da tutti i bottoni
  const bottoni = document.querySelectorAll(".tab-btn");
  bottoni.forEach((b) => b.classList.remove("active"));

  // Mostra sezione selezionata
  const sezioneEl = document.getElementById(`sezione-${nomeSezione}`);
  if (sezioneEl) {
    sezioneEl.classList.add("active");
  }

  // NON usare event.target direttamente, trova il bottone
  const bottoneDaAttivare = document.querySelector(
    `[onclick*="${nomeSezione}"]`
  );
  if (bottoneDaAttivare) {
    bottoneDaAttivare.classList.add("active");
  }

  // Carica dati della sezione
  switch (nomeSezione) {
    case "rotoli":
      caricaRotoli();
      break;
    case "prelievi":
      // Non caricare se API non esiste
      console.log("ℹ️ Prelievi: API non ancora implementata");
      break;
    case "ritagli":
      console.log("ℹ️ Ritagli: API non ancora implementata");
      break;
    case "fornitori":
      console.log("ℹ️ Fornitori: API non ancora implementata");
      break;
    case "inventario":
      console.log("ℹ️ Inventario: API non ancora implementata");
      break;
  }
}

/* ========================================
   GESTIONE ROTOLI
======================================== */

async function caricaRotoli() {
  const container = document.getElementById("rotoli-container");
  if (!container) {
    console.error("❌ Container rotoli non trovato");
    return;
  }

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /rotoli...");
    const response = await fetch(`${API_URL}/rotoli`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    const rotoli = await response.json();
    console.log("✅ Rotoli ricevuti:", rotoli);

    if (!Array.isArray(rotoli) || rotoli.length === 0) {
      container.innerHTML =
        '<p class="empty-state">📦 Nessun rotolo presente. Aggiungi il primo!</p>';
      return;
    }

    let html = `
      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>Tipo</th>
            <th>Colore</th>
            <th>Fantasia</th>
            <th>Fornitore</th>
            <th>Lotto</th>
            <th>Lunghezza Tot.</th>
            <th>Lunghezza Att.</th>
            <th>Costo/m</th>
            <th>Valore Tot.</th>
            <th>Stato</th>
            <th>Azioni</th>
          </tr>
        </thead>
        <tbody>
    `;

    rotoli.forEach((r) => {
      // GESTIONE SICURA: supporta entrambi i formati
      const lunghezzaTot = parseFloat(r.lunghezza_totale || r.lunghezza || 0);
      const lunghezzaAtt = parseFloat(r.lunghezza_attuale || r.lunghezza || 0);
      const costo = parseFloat(r.costo_metro || r.costo || 0);

      const valoreTotale = (lunghezzaAtt * costo).toFixed(2);
      const percentualeUso =
        lunghezzaTot > 0 ? ((lunghezzaAtt / lunghezzaTot) * 100).toFixed(0) : 0;

      const stato = r.stato || "Disponibile";
      const statoClass =
        stato === "Disponibile"
          ? "status-disponibile"
          : stato === "Esaurito"
          ? "status-esaurito"
          : "status-uso";

      html += `
        <tr>
          <td><strong>${r.id || "N/A"}</strong></td>
          <td>${r.tipo || "N/A"}</td>
          <td>${r.colore || "N/A"}</td>
          <td>${r.fantasia || "-"}</td>
          <td>${r.fornitore || "-"}</td>
          <td>${r.lotto || "-"}</td>
          <td>${lunghezzaTot.toFixed(2)} m</td>
          <td>${lunghezzaAtt.toFixed(2)} m (${percentualeUso}%)</td>
          <td>€${costo.toFixed(2)}</td>
          <td><strong>€${valoreTotale}</strong></td>
          <td><span class="${statoClass}">${stato}</span></td>
          <td>
            <button onclick="modificaRotolo('${
              r.id
            }')" class="btn-icon" title="Modifica">✏️</button>
            <button onclick="eliminaRotolo('${
              r.id
            }')" class="btn-icon" title="Elimina">🗑️</button>
          </td>
        </tr>
      `;
    });

    html += "</tbody></table>";
    container.innerHTML = html;
    console.log("✅ Tabella rotoli generata");
  } catch (error) {
    console.error("❌ Errore caricamento rotoli:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

async function aggiungiRotolo() {
  console.log("🖱️ Click su Aggiungi Rotolo");

  const tipo = document.getElementById("tipo").value.trim();
  const colore = document.getElementById("colore").value.trim();
  const fantasia = document.getElementById("fantasia")
    ? document.getElementById("fantasia").value.trim()
    : "";
  const lunghezzaTotale = parseFloat(
    document.getElementById("lunghezza_totale")
      ? document.getElementById("lunghezza_totale").value
      : document.getElementById("lunghezza").value
  );
  const costo = parseFloat(document.getElementById("costo").value);
  const fornitore = document.getElementById("fornitore")
    ? document.getElementById("fornitore").value.trim()
    : "";
  const lotto = document.getElementById("lotto")
    ? document.getElementById("lotto").value.trim()
    : "";
  const stato = document.getElementById("stato")
    ? document.getElementById("stato").value
    : "Disponibile";
  const note = document.getElementById("note")
    ? document.getElementById("note").value.trim()
    : "";

  // Validazione
  if (!tipo || !colore) {
    alert("⚠️ Inserisci tipo e colore del tessuto");
    return;
  }

  if (isNaN(lunghezzaTotale) || lunghezzaTotale <= 0) {
    alert("⚠️ Inserisci una lunghezza valida");
    return;
  }

  if (isNaN(costo) || costo <= 0) {
    alert("⚠️ Inserisci un costo valido");
    return;
  }

  const rotolo = {
    tipo: tipo,
    colore: colore,
    fantasia: fantasia,
    lunghezza: lunghezzaTotale, // Supporta entrambi i formati
    lunghezza_totale: lunghezzaTotale,
    lunghezza_attuale: lunghezzaTotale,
    costo: costo, // Supporta entrambi i formati
    costo_metro: costo,
    fornitore: fornitore,
    lotto: lotto,
    stato: stato,
    noteAggiuntive: note,
    data: getCurrentDate(),
  };

  try {
    console.log("📤 Invio rotolo:", rotolo);

    const response = await fetch(`${API_URL}/rotoli/add`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(rotolo),
    });

    const result = await response.json();
    console.log("📥 Risposta server:", result);

    if (result.status === "ok") {
      alert("✅ Rotolo aggiunto con successo!");

      // Pulisci form
      document.getElementById("tipo").value = "";
      document.getElementById("colore").value = "";
      if (document.getElementById("fantasia"))
        document.getElementById("fantasia").value = "";
      if (document.getElementById("lunghezza_totale")) {
        document.getElementById("lunghezza_totale").value = "";
      } else {
        document.getElementById("lunghezza").value = "";
      }
      document.getElementById("costo").value = "";
      if (document.getElementById("fornitore"))
        document.getElementById("fornitore").value = "";
      if (document.getElementById("lotto"))
        document.getElementById("lotto").value = "";
      if (document.getElementById("stato"))
        document.getElementById("stato").value = "Disponibile";
      if (document.getElementById("note"))
        document.getElementById("note").value = "";

      // Ricarica lista
      caricaRotoli();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("❌ Errore aggiunta rotolo:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

async function eliminaRotolo(id) {
  if (!confirm(`Sei sicuro di voler eliminare il rotolo ${id}?`)) {
    return;
  }

  try {
    const response = await fetch(`${API_URL}/rotoli/delete/${id}`, {
      method: "DELETE",
    });

    const result = await response.json();

    if (result.status === "ok") {
      alert("✅ Rotolo eliminato con successo!");
      caricaRotoli();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

function filtraRotoli() {
  const filtro = document.getElementById("ricerca-rotoli").value.toLowerCase();
  const righe = document.querySelectorAll("#rotoli-container tbody tr");

  righe.forEach((riga) => {
    const testo = riga.textContent.toLowerCase();
    if (testo.includes(filtro)) {
      riga.style.display = "";
    } else {
      riga.style.display = "none";
    }
  });
}

/* ========================================
   GESTIONE PRELIEVI
======================================== */

async function caricaPrelievi() {
  const container = document.getElementById("prelievi-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /prelievi...");
    const response = await fetch(`${API_URL}/prelievi`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    const prelievi = await response.json();
    console.log("✅ Prelievi ricevuti:", prelievi);

    if (!Array.isArray(prelievi) || prelievi.length === 0) {
      container.innerHTML =
        '<p class="empty-state">✂️ Nessun prelievo registrato</p>';
      return;
    }

    let html = `
      <table>
        <thead>
          <tr>
            <th>ID Prelievo</th>
            <th>ID Rotolo</th>
            <th>Metraggio</th>
            <th>Operatore</th>
            <th>Cliente/Progetto</th>
            <th>Data</th>
          </tr>
        </thead>
        <tbody>
    `;

    prelievi.forEach((p) => {
      const metraggio = parseFloat(p.metraggio_prelevato || p.metraggio || 0);
      html += `
        <tr>
          <td><strong>${p.id || "N/A"}</strong></td>
          <td>${p.id_rotolo || "N/A"}</td>
          <td>${metraggio.toFixed(2)} m</td>
          <td>${p.operatore || "-"}</td>
          <td>${p.cliente || "-"}</td>
          <td>${formatDate(p.data)}</td>
        </tr>
      `;
    });

    html += "</tbody></table>";
    container.innerHTML = html;
  } catch (error) {
    console.error("❌ Errore caricamento prelievi:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

async function registraPrelievo() {
  const idRotolo = document.getElementById("prelievo-rotolo").value.trim();
  const metraggio = parseFloat(document.getElementById("metraggio").value);
  const operatore = document.getElementById("operatore").value.trim();
  const cliente = document.getElementById("cliente")
    ? document.getElementById("cliente").value.trim()
    : "";

  if (!idRotolo || !operatore) {
    alert("⚠️ Inserisci ID rotolo e operatore");
    return;
  }

  if (isNaN(metraggio) || metraggio <= 0) {
    alert("⚠️ Inserisci un metraggio valido");
    return;
  }

  const prelievo = {
    id_rotolo: idRotolo,
    metraggio_prelevato: metraggio,
    operatore: operatore,
    cliente: cliente,
    data: getCurrentDate(),
  };

  try {
    const response = await fetch(`${API_URL}/prelievi/add`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(prelievo),
    });

    const result = await response.json();

    if (result.status === "ok") {
      alert("✅ Prelievo registrato con successo!");

      document.getElementById("prelievo-rotolo").value = "";
      document.getElementById("metraggio").value = "";
      document.getElementById("operatore").value = "";
      if (document.getElementById("cliente"))
        document.getElementById("cliente").value = "";

      caricaPrelievi();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

/* ========================================
   GESTIONE RITAGLI
======================================== */

async function caricaRitagli() {
  const container = document.getElementById("ritagli-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /ritagli...");
    const response = await fetch(`${API_URL}/ritagli`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    const ritagli = await response.json();
    console.log("✅ Ritagli ricevuti:", ritagli);

    if (!Array.isArray(ritagli) || ritagli.length === 0) {
      container.innerHTML =
        '<p class="empty-state">🧶 Nessun ritaglio presente</p>';
      return;
    }

    // Calcola statistiche
    const riutilizzabili = ritagli.filter(
      (r) => parseFloat(r.lunghezza || 0) >= 0.5
    );
    const scarti = ritagli.filter((r) => parseFloat(r.lunghezza || 0) < 0.3);
    const metraggioRecuperabile = riutilizzabili.reduce(
      (sum, r) => sum + parseFloat(r.lunghezza || 0),
      0
    );

    // Aggiorna statistiche se gli elementi esistono
    if (document.getElementById("ritagli-riutilizzabili"))
      document.getElementById("ritagli-riutilizzabili").textContent =
        riutilizzabili.length;
    if (document.getElementById("scarti-totali"))
      document.getElementById("scarti-totali").textContent = scarti.length;
    if (document.getElementById("metraggio-recuperabile"))
      document.getElementById("metraggio-recuperabile").textContent =
        metraggioRecuperabile.toFixed(2) + " m";

    let html = `
      <table>
        <thead>
          <tr>
            <th>ID Ritaglio</th>
            <th>ID Rotolo</th>
            <th>Lunghezza</th>
            <th>Categoria</th>
            <th>Data</th>
          </tr>
        </thead>
        <tbody>
    `;

    ritagli.forEach((r) => {
      const lunghezza = parseFloat(r.lunghezza || 0);
      const categoria =
        lunghezza >= 0.5
          ? "✅ Riutilizzabile"
          : lunghezza < 0.3
          ? "🗑️ Scarto"
          : "⚠️ Medio";
      html += `
        <tr>
          <td><strong>${r.idRitaglio || r.id || "N/A"}</strong></td>
          <td>${r.id_rotolo || "N/A"}</td>
          <td>${lunghezza.toFixed(2)} m</td>
          <td>${categoria}</td>
          <td>${formatDate(r.data)}</td>
        </tr>
      `;
    });

    html += "</tbody></table>";
    container.innerHTML = html;
  } catch (error) {
    console.error("❌ Errore caricamento ritagli:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

/* ========================================
   GESTIONE FORNITORI
======================================== */

async function caricaFornitori() {
  const container = document.getElementById("fornitori-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /fornitori...");
    const response = await fetch(`${API_URL}/fornitori`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    const fornitori = await response.json();
    console.log("✅ Fornitori ricevuti:", fornitori);

    if (!Array.isArray(fornitori) || fornitori.length === 0) {
      container.innerHTML =
        '<p class="empty-state">📦 Nessun fornitore presente</p>';
      return;
    }

    let html = `
      <table>
        <thead>
          <tr>
            <th>Nome</th>
            <th>P.IVA</th>
            <th>Telefono</th>
            <th>Email</th>
            <th>Indirizzo</th>
            <th>Azioni</th>
          </tr>
        </thead>
        <tbody>
    `;

    fornitori.forEach((f) => {
      html += `
        <tr>
          <td><strong>${f.nome || "N/A"}</strong></td>
          <td>${f.partita_iva || "-"}</td>
          <td>${f.telefono || "-"}</td>
          <td>${f.email || "-"}</td>
          <td>${f.indirizzo || "-"}</td>
          <td>
            <button onclick="eliminaFornitore('${
              f.nome
            }')" class="btn-icon" title="Elimina">🗑️</button>
          </td>
        </tr>
      `;
    });

    html += "</tbody></table>";
    container.innerHTML = html;
  } catch (error) {
    console.error("❌ Errore caricamento fornitori:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

async function aggiungiFornitore() {
  const nome = document.getElementById("nome-fornitore").value.trim();
  const piva = document.getElementById("piva").value.trim();
  const telefono = document.getElementById("telefono")
    ? document.getElementById("telefono").value.trim()
    : "";
  const email = document.getElementById("email-fornitore")
    ? document.getElementById("email-fornitore").value.trim()
    : "";
  const indirizzo = document.getElementById("indirizzo")
    ? document.getElementById("indirizzo").value.trim()
    : "";

  if (!nome || !piva) {
    alert("⚠️ Inserisci almeno nome e P.IVA");
    return;
  }

  const fornitore = {
    nome: nome,
    partita_iva: piva,
    telefono: telefono,
    email: email,
    indirizzo: indirizzo,
  };

  try {
    const response = await fetch(`${API_URL}/fornitori/add`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(fornitore),
    });

    const result = await response.json();

    if (result.status === "ok") {
      alert("✅ Fornitore aggiunto con successo!");

      document.getElementById("nome-fornitore").value = "";
      document.getElementById("piva").value = "";
      if (document.getElementById("telefono"))
        document.getElementById("telefono").value = "";
      if (document.getElementById("email-fornitore"))
        document.getElementById("email-fornitore").value = "";
      if (document.getElementById("indirizzo"))
        document.getElementById("indirizzo").value = "";

      caricaFornitori();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

async function eliminaFornitore(nome) {
  if (!confirm(`Sei sicuro di voler eliminare il fornitore ${nome}?`)) {
    return;
  }

  try {
    const response = await fetch(
      `${API_URL}/fornitori/delete/${encodeURIComponent(nome)}`,
      {
        method: "DELETE",
      }
    );

    const result = await response.json();

    if (result.status === "ok") {
      alert("✅ Fornitore eliminato!");
      caricaFornitori();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

/* ========================================
   GESTIONE INVENTARIO
======================================== */

async function caricaInventario() {
  try {
    console.log("📡 Chiamata API GET /inventario...");
    const response = await fetch(`${API_URL}/inventario`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    const inventario = await response.json();
    console.log("✅ Inventario ricevuto:", inventario);

    // Aggiorna statistiche se gli elementi esistono
    if (document.getElementById("totale-rotoli"))
      document.getElementById("totale-rotoli").textContent =
        inventario.numeroRotoli || 0;
    if (document.getElementById("metraggio-totale"))
      document.getElementById("metraggio-totale").textContent =
        (inventario.metraggioTotale || 0).toFixed(2) + " m";
    if (document.getElementById("valore-totale"))
      document.getElementById("valore-totale").textContent =
        "€" + (inventario.valoreTotale || 0).toFixed(2);
    if (document.getElementById("rotoli-disponibili"))
      document.getElementById("rotoli-disponibili").textContent =
        inventario.rotoliDisponibili || 0;
  } catch (error) {
    console.error("❌ Errore caricamento inventario:", error);
    alert("❌ Errore caricamento inventario");
  }
}

/* ========================================
   FUNZIONI EXPORT
======================================== */

async function esportaCSV() {
  try {
    const response = await fetch(`${API_URL}/export/csv`);
    const blob = await response.blob();
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = `inventario_${getCurrentDateString()}.csv`;
    document.body.appendChild(a);
    a.click();
    window.URL.revokeObjectURL(url);
    document.body.removeChild(a);
    alert("✅ CSV esportato con successo!");
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore durante l'esportazione");
  }
}

function stampaReport() {
  window.print();
}

async function backupDati() {
  if (!confirm("Vuoi creare un backup di tutti i dati?")) {
    return;
  }

  try {
    const response = await fetch(`${API_URL}/backup`, {
      method: "POST",
    });

    const result = await response.json();

    if (result.status === "ok") {
      alert("✅ Backup creato con successo!");
    } else {
      alert("❌ Errore durante il backup");
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore durante il backup");
  }
}

/* ========================================
   UTILITY FUNCTIONS
======================================== */

function getCurrentDate() {
  const now = new Date();
  return {
    giorno: now.getDate(),
    mese: now.getMonth() + 1,
    anno: now.getFullYear(),
  };
}

function getCurrentDateString() {
  const now = new Date();
  return `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(
    2,
    "0"
  )}-${String(now.getDate()).padStart(2, "0")}`;
}

function formatDate(dataObj) {
  if (!dataObj) return "-";
  if (typeof dataObj === "string") return dataObj;
  return `${String(dataObj.giorno).padStart(2, "0")}/${String(
    dataObj.mese
  ).padStart(2, "0")}/${dataObj.anno}`;
}

// Funzioni stub per evitare errori
function modificaRotolo(id) {
  alert(`Funzionalità modifica rotolo ${id} non ancora implementata`);
}

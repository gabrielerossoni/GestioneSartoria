// app.js - Frontend JavaScript completo per GestioneSartoria
// Versione: 2.0 - Compatibile con JSON locale generato dal programma C

// ===== CONFIGURAZIONE =====
const DATA_FILE = "./dati.json"; // File JSON locale generato dal C

// ===== INIZIALIZZAZIONE PAGINA =====
window.onload = function () {
  console.log("✅ Pagina caricata");
  caricaDati(); // Carica tutti i dati all'avvio
  mostraSezione("rotoli"); // Mostra la sezione rotoli di default
};

// ===== VARIABILE GLOBALE PER CACHE DATI =====
let datiCache = null;

/* ========================================
   CARICAMENTO DATI DAL FILE JSON
======================================== */
async function caricaDati() {
  try {
    console.log("📡 Caricamento dati da", DATA_FILE);
    const response = await fetch(DATA_FILE);
    
    if (!response.ok) {
      throw new Error(`Errore HTTP: ${response.status}`);
    }
    
    datiCache = await response.json();
    console.log("✅ Dati caricati:", datiCache);
    return datiCache;
  } catch (error) {
    console.error("❌ Errore caricamento dati:", error);
    datiCache = { rotoli: [], prelievi: [], ritagli: [], fornitori: [], progetti: [] };
    return datiCache;
  }
}

/* ========================================
   GESTIONE NAVIGAZIONE TAB
======================================== */
function mostraSezione(nomeSezione) {
  // Nascondi tutte le sezioni
  const sezioni = document.querySelectorAll(".sezione");
  sezioni.forEach((s) => s.classList.remove("active"));

  // Rimuovi active da tutti i pulsanti
  const bottoni = document.querySelectorAll(".tab-btn");
  bottoni.forEach((b) => b.classList.remove("active"));

  // Mostra la sezione selezionata
  const sezioneEl = document.getElementById(`sezione-${nomeSezione}`);
  if (sezioneEl) {
    sezioneEl.classList.add("active");
  }

  // Attiva il pulsante corrispondente
  const bottoneDaAttivare = document.querySelector(`[onclick*="${nomeSezione}"]`);
  if (bottoneDaAttivare) {
    bottoneDaAttivare.classList.add("active");
  }

  // Carica i dati della sezione
  switch (nomeSezione) {
    case "rotoli":
      caricaRotoli();
      break;
    case "prelievi":
      caricaPrelievi();
      break;
    case "ritagli":
      caricaRitagli();
      break;
    case "fornitori":
      caricaFornitori();
      break;
    case "inventario":
      caricaInventario();
      break;
  }
}

/* ========================================
   GESTIONE ROTOLI
======================================== */
async function caricaRotoli() {
  const container = document.getElementById("rotoli-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    const dati = datiCache || (await caricaDati());
    const rotoli = dati.rotoli || [];

    if (rotoli.length === 0) {
      container.innerHTML = '<p class="empty-state">📦 Nessun rotolo presente. Il programma C deve prima aggiungere dati.</p>';
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
          </tr>
        </thead>
        <tbody>
    `;

    rotoli.forEach((r) => {
      const lunghezzaTot = parseFloat(r.lunghezza_totale || 0);
      const lunghezzaAtt = parseFloat(r.lunghezza_attuale || 0);
      const costo = parseFloat(r.costo_metro || 0);
      const valoreTotale = (lunghezzaAtt * costo).toFixed(2);
      const percentualeUso = lunghezzaTot > 0 ? ((lunghezzaAtt / lunghezzaTot) * 100).toFixed(0) : 0;

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

function filtraRotoli() {
  const filtro = document.getElementById("ricerca-rotoli").value.toLowerCase();
  const righe = document.querySelectorAll("#rotoli-container tbody tr");

  righe.forEach((riga) => {
    const testo = riga.textContent.toLowerCase();
    riga.style.display = testo.includes(filtro) ? "" : "none";
  });
}

function aggiungiRotolo() {
  alert("⚠️ ATTENZIONE: L'aggiunta di rotoli deve essere fatta dal programma C.\nQuesta interfaccia web è in SOLA LETTURA.");
}

/* ========================================
   GESTIONE PRELIEVI
======================================== */
async function caricaPrelievi() {
  const container = document.getElementById("prelievi-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    const dati = datiCache || (await caricaDati());
    const prelievi = dati.prelievi || [];

    if (prelievi.length === 0) {
      container.innerHTML = '<p class="empty-state">✂️ Nessun prelievo registrato</p>';
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
            <th>Data</th>
          </tr>
        </thead>
        <tbody>
    `;

    prelievi.forEach((p) => {
      const metraggio = parseFloat(p.metraggio_prelevato || 0);
      html += `
        <tr>
          <td><strong>${p.id || "N/A"}</strong></td>
          <td>${p.id_rotolo || "N/A"}</td>
          <td>${metraggio.toFixed(2)} m</td>
          <td>${p.operatore || "-"}</td>
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

function registraPrelievo() {
  alert("⚠️ ATTENZIONE: I prelievi devono essere registrati dal programma C.\nQuesta interfaccia web è in SOLA LETTURA.");
}

/* ========================================
   GESTIONE RITAGLI
======================================== */
async function caricaRitagli() {
  const container = document.getElementById("ritagli-container");
  if (!container) return;

  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    const dati = datiCache || (await caricaDati());
    const ritagli = dati.ritagli || [];

    if (ritagli.length === 0) {
      container.innerHTML = '<p class="empty-state">🧶 Nessun ritaglio presente</p>';
      return;
    }

    // Calcolo statistiche
    const riutilizzabili = ritagli.filter((r) => parseFloat(r.lunghezza || 0) >= 0.5);
    const scarti = ritagli.filter((r) => parseFloat(r.lunghezza || 0) < 0.3);
    const metraggioRecuperabile = riutilizzabili.reduce(
      (sum, r) => sum + parseFloat(r.lunghezza || 0),
      0
    );

    // Aggiorna statistiche
    if (document.getElementById("ritagli-riutilizzabili"))
      document.getElementById("ritagli-riutilizzabili").textContent = riutilizzabili.length;
    if (document.getElementById("scarti-totali"))
      document.getElementById("scarti-totali").textContent = scarti.length;
    if (document.getElementById("metraggio-recuperabile"))
      document.getElementById("metraggio-recuperabile").textContent = metraggioRecuperabile.toFixed(2) + " m";

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
    const dati = datiCache || (await caricaDati());
    const fornitori = dati.fornitori || [];

    if (fornitori.length === 0) {
      container.innerHTML = '<p class="empty-state">📦 Nessun fornitore presente</p>';
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

function aggiungiFornitore() {
  alert("⚠️ ATTENZIONE: L'aggiunta di fornitori deve essere fatta dal programma C.\nQuesta interfaccia web è in SOLA LETTURA.");
}

/* ========================================
   GESTIONE INVENTARIO
======================================== */
async function caricaInventario() {
  try {
    const dati = datiCache || (await caricaDati());
    const rotoli = dati.rotoli || [];

    // Calcola statistiche
    const numeroRotoli = rotoli.length;
    const metraggioTotale = rotoli.reduce((sum, r) => sum + parseFloat(r.lunghezza_totale || 0), 0);
    const valoreTotale = rotoli.reduce((sum, r) => {
      const lunghezza = parseFloat(r.lunghezza_attuale || 0);
      const costo = parseFloat(r.costo_metro || 0);
      return sum + (lunghezza * costo);
    }, 0);
    const rotoliDisponibili = rotoli.filter((r) => r.stato === "Disponibile").length;

    // Aggiorna statistiche
    if (document.getElementById("totale-rotoli"))
      document.getElementById("totale-rotoli").textContent = numeroRotoli;
    if (document.getElementById("metraggio-totale"))
      document.getElementById("metraggio-totale").textContent = metraggioTotale.toFixed(2) + " m";
    if (document.getElementById("valore-totale"))
      document.getElementById("valore-totale").textContent = "€" + valoreTotale.toFixed(2);
    if (document.getElementById("rotoli-disponibili"))
      document.getElementById("rotoli-disponibili").textContent = rotoliDisponibili;

    console.log("✅ Inventario aggiornato");
  } catch (error) {
    console.error("❌ Errore caricamento inventario:", error);
  }
}

/* ========================================
   FUNZIONI EXPORT
======================================== */
function stampaReport() {
  window.print();
}

function backupDati() {
  alert("⚠️ Il backup deve essere fatto dal programma C (opzione 8 del menu).");
}

/* ========================================
   UTILITY FUNCTIONS
======================================== */
function formatDate(dataObj) {
  if (!dataObj) return "-";
  if (typeof dataObj === "string") return dataObj;
  return `${String(dataObj.giorno).padStart(2, "0")}/${String(dataObj.mese).padStart(2, "0")}/${dataObj.anno}`;
}

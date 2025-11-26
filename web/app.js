// app.js - Frontend JavaScript completo per GestioneSartoria
// Versione: 3.1

// ===== CONFIGURAZIONE =====
const DATA_FILE = "dati.json";

// ===== VARIABILI GLOBALI =====
let datiCache = null;
let fileHandle = null;

// ===== INIZIALIZZAZIONE PAGINA =====
window.onload = function () {
  console.log("✅ Pagina caricata");
  caricaDati();
  mostraSezione("rotoli");
};

/* ========================================
   CARICAMENTO DATI DAL FILE JSON
======================================== */
async function caricaDati() {
  try {
    console.log("📡 Caricamento dati da", DATA_FILE);
    const response = await fetch(DATA_FILE + "?t=" + new Date().getTime());

    if (!response.ok) {
      throw new Error(`Errore HTTP: ${response.status}`);
    }

    const datiGrezzi = await response.json();
    
    // ✅ FIX: Converti cm → metri per l'interfaccia web
    datiCache = {
      rotoli: (datiGrezzi.rotoli || []).map(r => ({
        ...r,
        lunghezza_attuale: (r.lunghezza_attuale || 0) / 100 // cm → m
      })),
      prelievi: datiGrezzi.prelievi || [],
      ritagli: (datiGrezzi.ritagli || []).map(rit => ({
        ...rit,
        lunghezza: (rit.lunghezza || 0) / 100 // cm → m  
      })),
      fornitori: datiGrezzi.fornitori || [],
      progetti: datiGrezzi.progetti || []
    };
    
    console.log("✅ Dati caricati e convertiti:", datiCache);
    return datiCache;
  } catch (error) {
    console.error("❌ Errore caricamento dati:", error);
    datiCache = {
      rotoli: [],
      prelievi: [],
      ritagli: [],
      fornitori: [],
      progetti: [],
    };
    return datiCache;
  }
}

/* ========================================
   SALVATAGGIO DIRETTO SU FILE
======================================== */
async function salvaDati(nuoviDati) {
  try {
    if (!window.showSaveFilePicker) {
      console.warn("⚠️ Browser non supporta File System Access API");
      return salvaDatiDownload(nuoviDati);
    }

    if (!fileHandle) {
      try {
        fileHandle = await window.showSaveFilePicker({
          suggestedName: "dati.json",
          types: [
            {
              description: "File JSON",
              accept: { "application/json": [".json"] },
            },
          ],
        });
      } catch (err) {
        if (err.name === "AbortError") {
          console.log("❌ Salvataggio annullato dall'utente");
          return false;
        }
        throw err;
      }
    }

    // ✅ FIX: Crea nuova copia convertendo metri → cm
    const datiPerC = {
      rotoli: nuoviDati.rotoli.map(r => ({
        ...r,
        lunghezza_attuale: (r.lunghezza_attuale || 0) * 100 // m → cm
      })),
      prelievi: nuoviDati.prelievi,
      ritagli: nuoviDati.ritagli.map(rit => ({
        ...rit,
        lunghezza: (rit.lunghezza || 0) * 100 // m → cm
      })),
      fornitori: nuoviDati.fornitori,
      progetti: nuoviDati.progetti
    };

    const writable = await fileHandle.createWritable();
    await writable.write(JSON.stringify(datiPerC, null, 2));
    await writable.close();

    console.log("✅ Dati salvati direttamente su file!");
    datiCache = nuoviDati;
    return true;
  } catch (error) {
    console.error("❌ Errore salvataggio diretto:", error);
    
    if (confirm("⚠️ Errore nel salvataggio diretto. Vuoi scaricare il file manualmente?")) {
      return salvaDatiDownload(nuoviDati);
    }
    return false;
  }
}

/* ========================================
   SALVATAGGIO TRAMITE DOWNLOAD (FALLBACK)
======================================== */
function salvaDatiDownload(nuoviDati) {
  try {
    // ✅ FIX: Crea nuova copia convertendo metri → cm
    const datiPerC = {
      rotoli: nuoviDati.rotoli.map(r => ({
        ...r,
        lunghezza_attuale: (r.lunghezza_attuale || 0) * 100 // m → cm
      })),
      prelievi: nuoviDati.prelievi,
      ritagli: nuoviDati.ritagli.map(rit => ({
        ...rit,
        lunghezza: (rit.lunghezza || 0) * 100 // m → cm
      })),
      fornitori: nuoviDati.fornitori,
      progetti: nuoviDati.progetti
    };

    const jsonString = JSON.stringify(datiPerC, null, 2);
    const blob = new Blob([jsonString], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "dati.json";
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);

    console.log("✅ File dati.json generato per il download");
    alert("💾 File dati.json scaricato! Sostituiscilo nella cartella /web/");

    datiCache = nuoviDati;
    return true;
  } catch (error) {
    console.error("❌ Errore download:", error);
    alert("❌ Errore nel salvataggio dei dati!");
    return false;
  }
}

/* ========================================
   RESETTA HANDLE FILE
======================================== */
function resetFileHandle() {
  fileHandle = null;
  alert("✅ File handle resettato. Al prossimo salvataggio potrai selezionare un nuovo file.");
}

/* ========================================
   GESTIONE NAVIGAZIONE TAB
======================================== */
function mostraSezione(nomeSezione) {
  const sezioni = document.querySelectorAll(".sezione");
  sezioni.forEach((s) => s.classList.remove("active"));

  const bottoni = document.querySelectorAll(".tab-btn");
  bottoni.forEach((b) => b.classList.remove("active"));

  const sezioneEl = document.getElementById(`sezione-${nomeSezione}`);
  if (sezioneEl) {
    sezioneEl.classList.add("active");
  }

  const bottoneDaAttivare = document.querySelector(`[onclick*="${nomeSezione}"]`);
  if (bottoneDaAttivare) {
    bottoneDaAttivare.classList.add("active");
  }

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
      container.innerHTML =
        '<p class="empty-state">📦 Nessun rotolo presente. Il programma C deve prima aggiungere dati.</p>';
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
      const percentualeUso =
        lunghezzaTot > 0 ? ((lunghezzaAtt / lunghezzaTot) * 100).toFixed(0) : 0;

      const stato = r.stato || "DISPONIBILE";
      const statoClass =
        stato === "DISPONIBILE"
          ? "status-disponibile"
          : stato === "ESAURITO"
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

/* ========================================
   AGGIUNTA ROTOLO
======================================== */
async function aggiungiRotolo() {
  const tipo = document.getElementById("tipo").value;
  const colore = document.getElementById("colore").value;
  const fantasia = document.getElementById("fantasia").value;
  const lunghezza_totale = parseFloat(document.getElementById("lunghezza_totale").value);
  const costo = parseFloat(document.getElementById("costo").value);
  const fornitore = document.getElementById("fornitore").value;
  const lotto = document.getElementById("lotto").value;
  const stato = document.getElementById("stato").value;
  const note = document.getElementById("note").value;

  if (!tipo || !colore || !lunghezza_totale || !costo) {
    alert("⚠️ Compila tutti i campi obbligatori!");
    return;
  }

  const dati = datiCache || (await caricaDati());
  const nuovoId = `R${String(dati.rotoli.length + 1).padStart(4, "0")}`;
  const oggi = new Date();

  const nuovoRotolo = {
    id: nuovoId,
    tipo: tipo,
    colore: colore,
    fantasia: fantasia,
    lunghezza_totale: lunghezza_totale,
    lunghezza_attuale: lunghezza_totale,
    costo_metro: costo,
    fornitore: fornitore,
    lotto: lotto,
    stato: stato,
    data: {
      giorno: oggi.getDate(),
      mese: oggi.getMonth() + 1,
      anno: oggi.getFullYear(),
    },
    noteAggiuntive: note,
  };

  dati.rotoli.push(nuovoRotolo);

  if (await salvaDati(dati)) {
    alert(`✅ Rotolo ${nuovoId} aggiunto e salvato con successo!`);
    
    document.getElementById("tipo").value = "";
    document.getElementById("colore").value = "";
    document.getElementById("fantasia").value = "";
    document.getElementById("lunghezza_totale").value = "";
    document.getElementById("costo").value = "";
    document.getElementById("fornitore").value = "";
    document.getElementById("lotto").value = "";
    document.getElementById("note").value = "";
    
    caricaRotoli();
  }
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

/* ========================================
   REGISTRA PRELIEVO (VERSIONE CORRETTA)
======================================== */
async function registraPrelievo() {
  const id_rotolo = document.getElementById("prelievo-rotolo").value;
  const metraggio = parseFloat(document.getElementById("metraggio").value);
  const operatore = document.getElementById("operatore").value;

  if (!id_rotolo || !metraggio || !operatore) {
    alert("⚠️ Compila tutti i campi!");
    return;
  }

  const dati = datiCache || (await caricaDati());
  const rotolo = dati.rotoli.find((r) => r.id === id_rotolo);

  if (!rotolo) {
    alert("❌ Rotolo non trovato!");
    return;
  }

  if (rotolo.lunghezza_attuale < metraggio) {
    alert(`❌ Metraggio insufficiente! Disponibile: ${rotolo.lunghezza_attuale.toFixed(2)} m`);
    return;
  }

  const nuovoIdPrelievo = `P${String(dati.prelievi.length + 1).padStart(4, "0")}`;
  const oggi = new Date();

  const nuovoPrelievo = {
    id: nuovoIdPrelievo,
    id_rotolo: id_rotolo,
    metraggio_prelevato: metraggio,
    operatore: operatore,
    data: {
      giorno: oggi.getDate(),
      mese: oggi.getMonth() + 1,
      anno: oggi.getFullYear(),
    },
  };

  rotolo.lunghezza_attuale -= metraggio;

  // ✅ FIX: Gestione ritagli senza duplicati
  if (rotolo.lunghezza_attuale <= 0.5 && rotolo.lunghezza_attuale > 0) {
    // Cambia stato solo se non è già RITAGLIO o ESAURITO
    if (rotolo.stato !== "RITAGLIO" && rotolo.stato !== "ESAURITO") {
      rotolo.stato = "RITAGLIO";

      // Controlla se esiste già un ritaglio per questo rotolo
      const ritaglioEsistente = dati.ritagli.find(rit => rit.id_rotolo === id_rotolo);

      if (!ritaglioEsistente) {
        // Crea nuovo ritaglio solo se non esiste
        const nuovoIdRitaglio = `RIT${String(dati.ritagli.length + 1).padStart(4, "0")}`;
        const nuovoRitaglio = {
          idRitaglio: nuovoIdRitaglio,
          id_rotolo: id_rotolo,
          lunghezza: rotolo.lunghezza_attuale,
          data: {
            giorno: oggi.getDate(),
            mese: oggi.getMonth() + 1,
            anno: oggi.getFullYear(),
          },
        };
        dati.ritagli.push(nuovoRitaglio);
        console.log(`✅ Ritaglio ${nuovoIdRitaglio} creato per rotolo ${id_rotolo}`);
      } else {
        // Aggiorna lunghezza del ritaglio esistente
        ritaglioEsistente.lunghezza = rotolo.lunghezza_attuale;
        console.log(`✅ Ritaglio ${ritaglioEsistente.idRitaglio} aggiornato: ${rotolo.lunghezza_attuale.toFixed(2)} m`);
      }
    }
  }

  // ✅ FIX: Imposta ESAURITO se finito completamente
  if (rotolo.lunghezza_attuale <= 0) {
    rotolo.stato = "ESAURITO";
  }

  dati.prelievi.push(nuovoPrelievo);

  if (await salvaDati(dati)) {
    alert(`✅ Prelievo registrato! Rimanenti: ${rotolo.lunghezza_attuale.toFixed(2)} m`);
    
    document.getElementById("prelievo-rotolo").value = "";
    document.getElementById("metraggio").value = "";
    document.getElementById("operatore").value = "";
    
    caricaPrelievi();
    caricaRotoli();
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
    const dati = datiCache || (await caricaDati());
    const ritagli = dati.ritagli || [];

    if (ritagli.length === 0) {
      container.innerHTML = '<p class="empty-state">🧶 Nessun ritaglio presente</p>';
      return;
    }

    const riutilizzabili = ritagli.filter((r) => parseFloat(r.lunghezza || 0) >= 0.5);
    const scarti = ritagli.filter((r) => parseFloat(r.lunghezza || 0) < 0.3);
    const metraggioRecuperabile = riutilizzabili.reduce(
      (sum, r) => sum + parseFloat(r.lunghezza || 0),
      0
    );

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

/* ========================================
   AGGIUNGI FORNITORE
======================================== */
async function aggiungiFornitore() {
  const nome = document.getElementById("nome-fornitore").value;
  const piva = document.getElementById("piva").value;
  const telefono = document.getElementById("telefono").value;
  const email = document.getElementById("email-fornitore").value;
  const indirizzo = document.getElementById("indirizzo").value;

  if (!nome || !piva) {
    alert("⚠️ Nome e Partita IVA sono obbligatori!");
    return;
  }

  const dati = datiCache || (await caricaDati());

  const nuovoFornitore = {
    nome: nome,
    partita_iva: piva,
    telefono: telefono,
    email: email,
    indirizzo: indirizzo,
  };

  dati.fornitori.push(nuovoFornitore);

  if (await salvaDati(dati)) {
    alert(`✅ Fornitore ${nome} aggiunto!`);
    
    document.getElementById("nome-fornitore").value = "";
    document.getElementById("piva").value = "";
    document.getElementById("telefono").value = "";
    document.getElementById("email-fornitore").value = "";
    document.getElementById("indirizzo").value = "";
    
    caricaFornitori();
  }
}

/* ========================================
   GESTIONE INVENTARIO
======================================== */
async function caricaInventario() {
  try {
    const dati = datiCache || (await caricaDati());
    const rotoli = dati.rotoli || [];

    const numeroRotoli = rotoli.length;
    
    // ✅ FIX: Usa lunghezza_attuale (già in metri dopo conversione)
    const metraggioTotale = rotoli.reduce((sum, r) => {
      return sum + parseFloat(r.lunghezza_attuale || 0);
    }, 0);
    
    // ✅ FIX: Calcola valore con lunghezza_attuale (già in metri)
    const valoreTotale = rotoli.reduce((sum, r) => {
      const lunghezza = parseFloat(r.lunghezza_attuale || 0);
      const costo = parseFloat(r.costo_metro || 0);
      return sum + (lunghezza * costo);
    }, 0);
    
    const rotoliDisponibili = rotoli.filter((r) => r.stato === "DISPONIBILE").length;

    // Aggiorna DOM
    const elemTotaleRotoli = document.getElementById("totale-rotoli");
    const elemMetraggio = document.getElementById("metraggio-totale");
    const elemValore = document.getElementById("valore-totale");
    const elemDisponibili = document.getElementById("rotoli-disponibili");

    if (elemTotaleRotoli) {
      elemTotaleRotoli.textContent = numeroRotoli;
    }
    if (elemMetraggio) {
      elemMetraggio.textContent = metraggioTotale.toFixed(2) + " m";
    }
    if (elemValore) {
      elemValore.textContent = "€" + valoreTotale.toFixed(2);
    }
    if (elemDisponibili) {
      elemDisponibili.textContent = rotoliDisponibili;
    }

    console.log("✅ Inventario aggiornato:", {
      numeroRotoli,
      metraggioTotale: metraggioTotale.toFixed(2),
      valoreTotale: valoreTotale.toFixed(2),
      rotoliDisponibili
    });
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
// Fine app.js
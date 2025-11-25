// app.js - Frontend JavaScript completo per GestioneSartoria
// Questo file gestisce tutta la logica dell'applicazione lato client

// ===== CONFIGURAZIONE =====
// URL base dell'API backend - tutti i fetch requests partono da qui
const API_URL = "http://localhost:8080/api";

// ===== INIZIALIZZAZIONE PAGINA =====
// Funzione che si esegue automaticamente quando la pagina è completamente caricata
window.onload = function () {
  console.log("✅ Pagina caricata"); // Log di conferma nel console
  caricaRotoli(); // Carica i rotoli dal database all'avvio
  mostraSezione("rotoli"); // Mostra la sezione rotoli di default
};

/* ========================================
   GESTIONE NAVIGAZIONE TAB
======================================== */
// Questo blocco gestisce il cambio tra le 5 sezioni principali dell'app

/**
 * Cambia la sezione visibile quando l'utente clicca su un tab
 * @param {string} nomeSezione - Nome della sezione da mostrare (rotoli, prelievi, ritagli, fornitori, inventario)
 */
function mostraSezione(nomeSezione) {
  // Step 1: Nascondi tutte le sezioni rimuovendo la classe "active"
  const sezioni = document.querySelectorAll(".sezione"); // Seleziona tutti gli elementi con classe "sezione"
  sezioni.forEach((s) => s.classList.remove("active")); // Rimuove "active" da ognuna

  // Step 2: Rimuovi la classe "active" da tutti i pulsanti tab
  const bottoni = document.querySelectorAll(".tab-btn"); // Seleziona tutti i pulsanti di navigazione
  bottoni.forEach((b) => b.classList.remove("active")); // Rimuove "active" da ognuno

  // Step 3: Mostra la sezione selezionata aggiungendo la classe "active"
  const sezioneEl = document.getElementById(`sezione-${nomeSezione}`); // Cerca l'elemento con ID "sezione-[nome]"
  if (sezioneEl) {
    sezioneEl.classList.add("active"); // Aggiunge "active" per mostrare la sezione
  }

  // Step 4: Attiva il pulsante corrispondente
  // Cerca il pulsante che contiene il nome della sezione nel suo onclick
  const bottoneDaAttivare = document.querySelector(
    `[onclick*="${nomeSezione}"]`
  );
  if (bottoneDaAttivare) {
    bottoneDaAttivare.classList.add("active"); // Evidenzia il pulsante
  }

  // Step 5: Carica i dati della sezione selezionata
  // Esegue funzioni diverse a seconda di quale sezione è stata cliccata
  switch (nomeSezione) {
    case "rotoli":
      caricaRotoli(); // Carica la lista dei rotoli dal database
      break;
    case "prelievi":
      // Non caricare se API non esiste - placeholder per futuri sviluppi
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
// Questo blocco gestisce tutte le operazioni relative ai rotoli di tessuto

/**
 * Carica l'elenco di tutti i rotoli dal database e li visualizza in una tabella
 * Questa è una funzione asincrona che comunica con l'API backend
 */
async function caricaRotoli() {
  // Trova il container dove inserire la tabella dei rotoli
  const container = document.getElementById("rotoli-container");
  if (!container) {
    console.error("❌ Container rotoli non trovato"); // Log errore se l'elemento non esiste
    return; // Esce dalla funzione
  }

  // Mostra messaggio di caricamento mentre si attende la risposta dal server
  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /rotoli..."); // Log della chiamata API
    // Effettua una richiesta GET all'endpoint /rotoli del backend
    const response = await fetch(`${API_URL}/rotoli`);

    // Controlla se la risposta dal server è ok (status 200-299)
    if (!response.ok) {
      throw new Error("Errore server: " + response.status); // Lancia un errore se la risposta non è ok
    }

    // Converte la risposta JSON in un oggetto JavaScript
    const rotoli = await response.json();
    console.log("✅ Rotoli ricevuti:", rotoli); // Log dei dati ricevuti

    // Controlla se l'array di rotoli è vuoto
    if (!Array.isArray(rotoli) || rotoli.length === 0) {
      // Mostra messaggio se non ci sono rotoli
      container.innerHTML =
        '<p class="empty-state">📦 Nessun rotolo presente. Aggiungi il primo!</p>';
      return; // Esce dalla funzione
    }

    // Inizio della costruzione della tabella HTML
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

    // Cicla su ogni rotolo ricevuto e crea una riga della tabella
    rotoli.forEach((r) => {
      // GESTIONE SICURA: supporta entrambi i formati di nomenclatura dal backend
      const lunghezzaTot = parseFloat(r.lunghezza_totale || r.lunghezza || 0); // Estrae la lunghezza totale
      const lunghezzaAtt = parseFloat(r.lunghezza_attuale || r.lunghezza || 0); // Estrae la lunghezza attuale
      const costo = parseFloat(r.costo_metro || r.costo || 0); // Estrae il costo al metro

      // Calcoli dei dati da visualizzare
      const valoreTotale = (lunghezzaAtt * costo).toFixed(2); // Calcola il valore totale (lunghezza * prezzo)
      const percentualeUso =
        lunghezzaTot > 0 ? ((lunghezzaAtt / lunghezzaTot) * 100).toFixed(0) : 0; // Calcola la percentuale di utilizzo

      // Estrae lo stato del rotolo dal database
      const stato = r.stato || "Disponibile";
      // Assegna una classe CSS differente in base allo stato
      const statoClass =
        stato === "Disponibile"
          ? "status-disponibile" // Verde
          : stato === "Esaurito"
          ? "status-esaurito" // Rosso
          : "status-uso"; // Giallo (in uso)

      // Aggiunge una riga alla tabella con tutti i dati del rotolo
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
            <!-- Pulsanti di azione: modifica e cancellazione -->
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

    // Chiude il tag della tabella
    html += "</tbody></table>";
    // Inserisce la tabella nel container
    container.innerHTML = html;
    console.log("✅ Tabella rotoli generata"); // Log di conferma
  } catch (error) {
    // Blocco di gestione degli errori
    console.error("❌ Errore caricamento rotoli:", error); // Log dell'errore nella console
    // Mostra un messaggio di errore all'utente
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

/**
 * Aggiunge un nuovo rotolo di tessuto al database
 * Raccoglie i dati dal form, li valida e li invia al backend
 */
async function aggiungiRotolo() {
  console.log("🖱️ Click su Aggiungi Rotolo"); // Log del click

  // === RACCOLTA DATI DAL FORM ===
  // Legge i valori dagli input del form e li assegna a variabili
  const tipo = document.getElementById("tipo").value.trim(); // Tipo di tessuto (Cotone, Seta, ecc.)
  const colore = document.getElementById("colore").value.trim(); // Colore del tessuto
  const fantasia = document.getElementById("fantasia")
    ? document.getElementById("fantasia").value.trim()
    : ""; // Pattern/Fantasia (opzionale)
  const lunghezzaTotale = parseFloat(
    // Lunghezza totale del rotolo in metri
    document.getElementById("lunghezza_totale")
      ? document.getElementById("lunghezza_totale").value
      : document.getElementById("lunghezza").value
  );
  const costo = parseFloat(document.getElementById("costo").value); // Costo al metro in euro
  const fornitore = document.getElementById("fornitore")
    ? document.getElementById("fornitore").value.trim()
    : ""; // Nome del fornitore (opzionale)
  const lotto = document.getElementById("lotto")
    ? document.getElementById("lotto").value.trim()
    : ""; // Codice lotto/partita (opzionale)
  const stato = document.getElementById("stato")
    ? document.getElementById("stato").value
    : "Disponibile"; // Stato del rotolo (Disponibile, In uso, Esaurito, Riservato)
  const note = document.getElementById("note")
    ? document.getElementById("note").value.trim()
    : ""; // Note aggiuntive (opzionale)

  // === VALIDAZIONE DATI ===
  // Controlla che i dati obbligatori siano stati inseriti correttamente
  if (!tipo || !colore) {
    alert("⚠️ Inserisci tipo e colore del tessuto"); // Avvisa l'utente
    return; // Esce dalla funzione
  }

  if (isNaN(lunghezzaTotale) || lunghezzaTotale <= 0) {
    alert("⚠️ Inserisci una lunghezza valida");
    return;
  }

  if (isNaN(costo) || costo <= 0) {
    alert("⚠️ Inserisci un costo valido");
    return;
  }

  // === CREAZIONE OGGETTO ROTOLO ===
  // Prepara l'oggetto con tutti i dati da inviare al backend
  const rotolo = {
    tipo: tipo,
    colore: colore,
    fantasia: fantasia,
    lunghezza: lunghezzaTotale, // Supporta entrambi i formati
    lunghezza_totale: lunghezzaTotale,
    lunghezza_attuale: lunghezzaTotale, // All'inizio la lunghezza attuale = totale
    costo: costo, // Supporta entrambi i formati
    costo_metro: costo,
    fornitore: fornitore,
    lotto: lotto,
    stato: stato,
    noteAggiuntive: note,
    data: getCurrentDate(), // Aggiunge la data attuale
  };

  try {
    console.log("📤 Invio rotolo:", rotolo); // Log dell'oggetto inviato

    // === INVIO AL BACKEND ===
    // Effettua una richiesta POST all'endpoint /rotoli/add
    const response = await fetch(`${API_URL}/rotoli/add`, {
      method: "POST", // Metodo HTTP POST per creare un nuovo record
      headers: {
        "Content-Type": "application/json", // Specifica che il contenuto è JSON
      },
      body: JSON.stringify(rotolo), // Converte l'oggetto in stringa JSON
    });

    // Converte la risposta in JSON
    const result = await response.json();
    console.log("📥 Risposta server:", result); // Log della risposta

    // Controlla se l'operazione è andata a buon fine
    if (result.status === "ok") {
      alert("✅ Rotolo aggiunto con successo!"); // Mostra messaggio di successo

      // === PULIZIA FORM ===
      // Cancella i valori da tutti gli input per preparare il form a un nuovo inserimento
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

      // === RICARICA LISTA ===
      // Ricarica la tabella dei rotoli per mostrare il nuovo rotolo
      caricaRotoli();
    } else {
      // Se il backend restituisce un errore
      alert("❌ Errore: " + result.message); // Mostra il messaggio di errore
    }
  } catch (error) {
    // Gestione errori di comunicazione con il server
    console.error("❌ Errore aggiunta rotolo:", error); // Log dell'errore
    alert("❌ Errore nella comunicazione con il server");
  }
}

/**
 * Elimina un rotolo dal database
 * @param {string} id - L'ID del rotolo da eliminare
 */
async function eliminaRotolo(id) {
  // Chiede conferma all'utente prima di cancellare
  if (!confirm(`Sei sicuro di voler eliminare il rotolo ${id}?`)) {
    return; // Se l'utente clicca "Annulla", esce dalla funzione
  }

  try {
    // === INVIO RICHIESTA DELETE AL BACKEND ===
    const response = await fetch(`${API_URL}/rotoli/delete/${id}`, {
      method: "DELETE", // Metodo HTTP DELETE per cancellare
    });

    // Converte la risposta in JSON
    const result = await response.json();

    // Controlla se l'eliminazione è andata a buon fine
    if (result.status === "ok") {
      alert("✅ Rotolo eliminato con successo!"); // Messaggio di successo
      caricaRotoli(); // Ricarica la tabella per mostrare che il rotolo è stato rimosso
    } else {
      alert("❌ Errore: " + result.message); // Mostra errore dal backend
    }
  } catch (error) {
    console.error("Errore:", error); // Log errore
    alert("❌ Errore nella comunicazione con il server");
  }
}

/**
 * Filtra la tabella dei rotoli in base al testo cercato
 * Questa funzione viene chiamata ad ogni pressione di tasto nella barra di ricerca
 */
function filtraRotoli() {
  // Legge il valore dalla barra di ricerca e lo converte in minuscolo
  const filtro = document.getElementById("ricerca-rotoli").value.toLowerCase();

  // Seleziona tutte le righe della tabella (tranne l'header)
  const righe = document.querySelectorAll("#rotoli-container tbody tr");

  // Cicla su ogni riga della tabella
  righe.forEach((riga) => {
    // Estrae il testo della riga e lo converte in minuscolo
    const testo = riga.textContent.toLowerCase();

    // Se il testo della riga contiene il filtro ricercato
    if (testo.includes(filtro)) {
      riga.style.display = ""; // Mostra la riga (vuoto = display predefinito)
    } else {
      riga.style.display = "none"; // Nascondi la riga
    }
  });
}

/* ========================================
   GESTIONE PRELIEVI
======================================== */
// Questo blocco gestisce i prelievi di tessuto dai rotoli

/**
 * Carica l'elenco di tutti i prelievi dal database e li visualizza in una tabella
 */
async function caricaPrelievi() {
  // Trova il container per la tabella dei prelievi
  const container = document.getElementById("prelievi-container");
  if (!container) return; // Esce se il container non esiste

  // Mostra messaggio di caricamento
  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /prelievi..."); // Log della chiamata
    // Effettua richiesta GET all'endpoint /prelievi
    const response = await fetch(`${API_URL}/prelievi`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    // Converte la risposta in JSON
    const prelievi = await response.json();
    console.log("✅ Prelievi ricevuti:", prelievi); // Log dei dati

    // Se non ci sono prelievi
    if (!Array.isArray(prelievi) || prelievi.length === 0) {
      container.innerHTML =
        '<p class="empty-state">✂️ Nessun prelievo registrato</p>';
      return;
    }

    // Inizio della costruzione della tabella HTML
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

    // Cicla su ogni prelievo e crea una riga della tabella
    prelievi.forEach((p) => {
      // Estrae il metraggio prelevato in metri
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

    // Chiude la tabella
    html += "</tbody></table>";
    container.innerHTML = html; // Inserisce la tabella nel container
  } catch (error) {
    console.error("❌ Errore caricamento prelievi:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

/**
 * Registra un nuovo prelievo di tessuto da un rotolo
 */
async function registraPrelievo() {
  // === RACCOLTA DATI DAL FORM ===
  const idRotolo = document.getElementById("prelievo-rotolo").value.trim(); // ID del rotolo
  const metraggio = parseFloat(document.getElementById("metraggio").value); // Metraggio da prelevare
  const operatore = document.getElementById("operatore").value.trim(); // Chi esegue il prelievo
  const cliente = document.getElementById("cliente")
    ? document.getElementById("cliente").value.trim()
    : ""; // Cliente/Progetto (opzionale)

  // === VALIDAZIONE ===
  if (!idRotolo || !operatore) {
    alert("⚠️ Inserisci ID rotolo e operatore");
    return;
  }

  if (isNaN(metraggio) || metraggio <= 0) {
    alert("⚠️ Inserisci un metraggio valido");
    return;
  }

  // === CREAZIONE OGGETTO PRELIEVO ===
  const prelievo = {
    id_rotolo: idRotolo,
    metraggio_prelevato: metraggio,
    operatore: operatore,
    cliente: cliente,
    data: getCurrentDate(), // Data attuale
  };

  try {
    // === INVIO AL BACKEND ===
    const response = await fetch(`${API_URL}/prelievi/add`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(prelievo),
    });

    // Converte la risposta in JSON
    const result = await response.json();

    // Controlla se il prelievo è stato registrato
    if (result.status === "ok") {
      alert("✅ Prelievo registrato con successo!"); // Messaggio di successo

      // === PULIZIA FORM ===
      document.getElementById("prelievo-rotolo").value = "";
      document.getElementById("metraggio").value = "";
      document.getElementById("operatore").value = "";
      if (document.getElementById("cliente"))
        document.getElementById("cliente").value = "";

      // Ricarica la tabella dei prelievi
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
// Questo blocco gestisce gli scarti/ritagli di tessuto

/**
 * Carica l'elenco dei ritagli dal database e visualizza statistiche
 */
async function caricaRitagli() {
  // Trova il container per i ritagli
  const container = document.getElementById("ritagli-container");
  if (!container) return;

  // Mostra messaggio di caricamento
  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /ritagli...");
    // Effettua richiesta GET all'endpoint /ritagli
    const response = await fetch(`${API_URL}/ritagli`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    // Converte la risposta in JSON
    const ritagli = await response.json();
    console.log("✅ Ritagli ricevuti:", ritagli);

    // Se non ci sono ritagli
    if (!Array.isArray(ritagli) || ritagli.length === 0) {
      container.innerHTML =
        '<p class="empty-state">🧶 Nessun ritaglio presente</p>';
      return;
    }

    // === CALCOLO STATISTICHE ===
    // Filtra i ritagli riutilizzabili (lunghezza >= 0.5 m)
    const riutilizzabili = ritagli.filter(
      (r) => parseFloat(r.lunghezza || 0) >= 0.5
    );
    // Filtra gli scarti (lunghezza < 0.3 m)
    const scarti = ritagli.filter((r) => parseFloat(r.lunghezza || 0) < 0.3);
    // Calcola il metraggio totale recuperabile dai ritagli riutilizzabili
    const metraggioRecuperabile = riutilizzabili.reduce(
      (sum, r) => sum + parseFloat(r.lunghezza || 0),
      0
    );

    // === AGGIORNAMENTO STATISTICHE NELLA PAGINA ===
    // Aggiorna il numero di ritagli riutilizzabili se l'elemento esiste
    if (document.getElementById("ritagli-riutilizzabili"))
      document.getElementById("ritagli-riutilizzabili").textContent =
        riutilizzabili.length;
    // Aggiorna il numero di scarti
    if (document.getElementById("scarti-totali"))
      document.getElementById("scarti-totali").textContent = scarti.length;
    // Aggiorna il metraggio recuperabile
    if (document.getElementById("metraggio-recuperabile"))
      document.getElementById("metraggio-recuperabile").textContent =
        metraggioRecuperabile.toFixed(2) + " m";

    // Inizio della costruzione della tabella HTML
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

    // Cicla su ogni ritaglio e crea una riga della tabella
    ritagli.forEach((r) => {
      const lunghezza = parseFloat(r.lunghezza || 0);
      // Categorizza il ritaglio in base alla lunghezza
      const categoria =
        lunghezza >= 0.5
          ? "✅ Riutilizzabile" // Verde - può essere riutilizzato
          : lunghezza < 0.3
          ? "🗑️ Scarto" // Rosso - troppo piccolo
          : "⚠️ Medio"; // Giallo - dimensione intermedia
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

    // Chiude la tabella
    html += "</tbody></table>";
    container.innerHTML = html; // Inserisce la tabella nel container
  } catch (error) {
    console.error("❌ Errore caricamento ritagli:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

/* ========================================
   GESTIONE FORNITORI
======================================== */
// Questo blocco gestisce l'elenco dei fornitori di tessuto

/**
 * Carica l'elenco di tutti i fornitori dal database
 */
async function caricaFornitori() {
  // Trova il container per la tabella dei fornitori
  const container = document.getElementById("fornitori-container");
  if (!container) return;

  // Mostra messaggio di caricamento
  container.innerHTML = '<p class="loading">⏳ Caricamento in corso...</p>';

  try {
    console.log("📡 Chiamata API GET /fornitori...");
    // Effettua richiesta GET all'endpoint /fornitori
    const response = await fetch(`${API_URL}/fornitori`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    // Converte la risposta in JSON
    const fornitori = await response.json();
    console.log("✅ Fornitori ricevuti:", fornitori);

    // Se non ci sono fornitori
    if (!Array.isArray(fornitori) || fornitori.length === 0) {
      container.innerHTML =
        '<p class="empty-state">📦 Nessun fornitore presente</p>';
      return;
    }

    // Inizio della costruzione della tabella HTML
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

    // Cicla su ogni fornitore e crea una riga della tabella
    fornitori.forEach((f) => {
      html += `
        <tr>
          <td><strong>${f.nome || "N/A"}</strong></td>
          <td>${f.partita_iva || "-"}</td>
          <td>${f.telefono || "-"}</td>
          <td>${f.email || "-"}</td>
          <td>${f.indirizzo || "-"}</td>
          <td>
            <!-- Pulsante per eliminare il fornitore -->
            <button onclick="eliminaFornitore('${
              f.nome
            }')" class="btn-icon" title="Elimina">🗑️</button>
          </td>
        </tr>
      `;
    });

    // Chiude la tabella
    html += "</tbody></table>";
    container.innerHTML = html; // Inserisce la tabella nel container
  } catch (error) {
    console.error("❌ Errore caricamento fornitori:", error);
    container.innerHTML = `<p class="empty-state">❌ Errore: ${error.message}</p>`;
  }
}

/**
 * Aggiunge un nuovo fornitore al database
 */
async function aggiungiFornitore() {
  // === RACCOLTA DATI DAL FORM ===
  const nome = document.getElementById("nome-fornitore").value.trim(); // Nome del fornitore
  const piva = document.getElementById("piva").value.trim(); // Partita IVA
  const telefono = document.getElementById("telefono")
    ? document.getElementById("telefono").value.trim()
    : ""; // Telefono (opzionale)
  const email = document.getElementById("email-fornitore")
    ? document.getElementById("email-fornitore").value.trim()
    : ""; // Email (opzionale)
  const indirizzo = document.getElementById("indirizzo")
    ? document.getElementById("indirizzo").value.trim()
    : ""; // Indirizzo (opzionale)

  // === VALIDAZIONE ===
  if (!nome || !piva) {
    alert("⚠️ Inserisci almeno nome e P.IVA");
    return;
  }

  // === CREAZIONE OGGETTO FORNITORE ===
  const fornitore = {
    nome: nome,
    partita_iva: piva,
    telefono: telefono,
    email: email,
    indirizzo: indirizzo,
  };

  try {
    // === INVIO AL BACKEND ===
    const response = await fetch(`${API_URL}/fornitori/add`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(fornitore),
    });

    // Converte la risposta in JSON
    const result = await response.json();

    // Controlla se il fornitore è stato aggiunto
    if (result.status === "ok") {
      alert("✅ Fornitore aggiunto con successo!"); // Messaggio di successo

      // === PULIZIA FORM ===
      document.getElementById("nome-fornitore").value = "";
      document.getElementById("piva").value = "";
      if (document.getElementById("telefono"))
        document.getElementById("telefono").value = "";
      if (document.getElementById("email-fornitore"))
        document.getElementById("email-fornitore").value = "";
      if (document.getElementById("indirizzo"))
        document.getElementById("indirizzo").value = "";

      // Ricarica la tabella dei fornitori
      caricaFornitori();
    } else {
      alert("❌ Errore: " + result.message);
    }
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore nella comunicazione con il server");
  }
}

/**
 * Elimina un fornitore dal database
 * @param {string} nome - Il nome del fornitore da eliminare
 */
async function eliminaFornitore(nome) {
  // Chiede conferma all'utente prima di cancellare
  if (!confirm(`Sei sicuro di voler eliminare il fornitore ${nome}?`)) {
    return;
  }

  try {
    // === INVIO RICHIESTA DELETE AL BACKEND ===
    // Usa encodeURIComponent per codificare correttamente il nome nel URL
    const response = await fetch(
      `${API_URL}/fornitori/delete/${encodeURIComponent(nome)}`,
      {
        method: "DELETE",
      }
    );

    // Converte la risposta in JSON
    const result = await response.json();

    // Controlla se l'eliminazione è andata a buon fine
    if (result.status === "ok") {
      alert("✅ Fornitore eliminato!"); // Messaggio di successo
      caricaFornitori(); // Ricarica la tabella
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
// Questo blocco gestisce il riepilogo generale dell'inventario

/**
 * Carica le statistiche generali dell'inventario e le visualizza nella dashboard
 */
async function caricaInventario() {
  try {
    console.log("📡 Chiamata API GET /inventario...");
    // Effettua richiesta GET all'endpoint /inventario
    const response = await fetch(`${API_URL}/inventario`);

    if (!response.ok) {
      throw new Error("Errore server: " + response.status);
    }

    // Converte la risposta in JSON
    const inventario = await response.json();
    console.log("✅ Inventario ricevuto:", inventario);

    // === AGGIORNAMENTO STATISTICHE NELLA PAGINA ===
    // Aggiorna il numero totale di rotoli se l'elemento esiste
    if (document.getElementById("totale-rotoli"))
      document.getElementById("totale-rotoli").textContent =
        inventario.numeroRotoli || 0;
    // Aggiorna il metraggio totale in magazzino
    if (document.getElementById("metraggio-totale"))
      document.getElementById("metraggio-totale").textContent =
        (inventario.metraggioTotale || 0).toFixed(2) + " m";
    // Aggiorna il valore economico totale dell'inventario
    if (document.getElementById("valore-totale"))
      document.getElementById("valore-totale").textContent =
        "€" + (inventario.valoreTotale || 0).toFixed(2);
    // Aggiorna il numero di rotoli disponibili (pronti per l'uso)
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
// Questo blocco gestisce l'esportazione e il backup dei dati

/**
 * Esporta i dati dell'inventario in formato CSV
 * Scarica il file sul computer dell'utente
 */
async function esportaCSV() {
  try {
    // Effettua richiesta GET all'endpoint /export/csv
    const response = await fetch(`${API_URL}/export/csv`);
    // Ottiene il file come blob (binary large object)
    const blob = await response.blob();
    // Crea un URL temporaneo per il blob
    const url = window.URL.createObjectURL(blob);
    // Crea un elemento <a> invisibile per il download
    const a = document.createElement("a");
    a.href = url; // Assegna l'URL del blob al link
    // Assegna il nome del file da scaricare
    a.download = `inventario_${getCurrentDateString()}.csv`;
    // Aggiunge il link al documento
    document.body.appendChild(a);
    // Simula il click per avviare il download
    a.click();
    // Libera la memoria del blob
    window.URL.revokeObjectURL(url);
    // Rimuove il link dal documento
    document.body.removeChild(a);
    alert("✅ CSV esportato con successo!");
  } catch (error) {
    console.error("Errore:", error);
    alert("❌ Errore durante l'esportazione");
  }
}

/**
 * Apre la finestra di stampa del browser per stampare il report
 */
function stampaReport() {
  window.print(); // Funzione nativa del browser che apre la finestra di stampa
}

/**
 * Crea un backup di tutti i dati del sistema
 */
async function backupDati() {
  // Chiede conferma all'utente prima di creare il backup
  if (!confirm("Vuoi creare un backup di tutti i dati?")) {
    return; // Se l'utente clicca "Annulla", esce dalla funzione
  }

  try {
    // === INVIO RICHIESTA BACKUP AL BACKEND ===
    const response = await fetch(`${API_URL}/backup`, {
      method: "POST", // Usa POST per richieste che modificano lo stato del server
    });

    // Converte la risposta in JSON
    const result = await response.json();

    // Controlla se il backup è stato creato
    if (result.status === "ok") {
      alert("✅ Backup creato con successo!"); // Messaggio di successo
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
// Questo blocco contiene funzioni di utilità generali usate in tutta l'app

/**
 * Restituisce la data attuale nel formato: {giorno, mese, anno}
 * @returns {Object} Oggetto con giorno, mese e anno
 */
function getCurrentDate() {
  const now = new Date(); // Crea un oggetto Date con la data/ora attuale
  return {
    giorno: now.getDate(), // Numero del giorno (1-31)
    mese: now.getMonth() + 1, // Numero del mese (1-12, +1 perché getMonth() ritorna 0-11)
    anno: now.getFullYear(), // Anno a 4 cifre
  };
}

/**
 * Restituisce la data attuale nel formato stringa: YYYY-MM-DD
 * Utile per i nomi dei file di export
 * @returns {string} Data formattata come stringa
 */
function getCurrentDateString() {
  const now = new Date(); // Crea un oggetto Date con la data/ora attuale
  // Costruisce la stringa nel formato YYYY-MM-DD
  return `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(
    2,
    "0"
  )}-${String(now.getDate()).padStart(2, "0")}`;
}

/**
 * Formatta un oggetto data nel formato: GG/MM/AAAA
 * @param {Object} dataObj - Oggetto data con proprietà {giorno, mese, anno}
 * @returns {string} Data formattata come stringa
 */
function formatDate(dataObj) {
  if (!dataObj) return "-"; // Se dataObj è null/undefined, ritorna "-"
  if (typeof dataObj === "string") return dataObj; // Se è già una stringa, la ritorna così com'è
  // Formatta la data con leading zeros (01/02/2024 invece di 1/2/2024)
  return `${String(dataObj.giorno).padStart(2, "0")}/${String(
    dataObj.mese
  ).padStart(2, "0")}/${dataObj.anno}`;
}

// === FUNZIONI STUB ===
// Questi sono placeholder per funzioni non ancora implementate

/**
 * Funzione stub per la modifica di un rotolo
 * Da implementare in futuro
 * @param {string} id - L'ID del rotolo da modificare
 */
function modificaRotolo(id) {
  alert(`Funzionalità modifica rotolo ${id} non ancora implementata`);
}

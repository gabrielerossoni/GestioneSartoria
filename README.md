# 🧵 Sistema di Gestione Sartoria

**Sistema di Gestione Sartoria** è un progetto sviluppato in **C**, pensato per digitalizzare la gestione dei tessuti e dei processi interni di una sartoria artigianale.  
Il software permette di **monitorare rotoli di tessuto**, **gestire prelievi**, **ottimizzare i tagli** e **archiviare i dati in modo persistente**, integrandosi con un’interfaccia **web interattiva**.

---

## 📌 Obiettivi del progetto

- Automatizzare la gestione dei **rotoli di tessuto**.
- Ridurre gli **sprechi** e migliorare l'efficienza operativa.
- Fornire una **piattaforma modulare e scalabile**.
- Offrire un'interfaccia **moderna e intuitiva** per l'utilizzo da browser o smartphone.

---

## 🌿 Branch principali

- **main:** branch stabile, versione base finita del progetto. È la versione affidabile, pronta per l’uso e consegnata secondo le richieste must.
- **master:** branch di sviluppo, contiene funzionalità aggiuntive, prove, estensioni e modifiche in corso. Qui si trovano tutte le feature extra e le sperimentazioni che non sono richieste obbligatorie, ma possono mostrare novità e idee avanzate.

> Nella consegna dell'esercizio veniva richiesto di mantenere due versioni: **base** (base, stabile, finita) e l'**evolutiva** (estesa, sperimentale). Abbiamo lavorato su entrambi, usando **main** per la versione definitiva e **master** per tutto ciò che può andare oltre le richieste must.

**Sintesi:**  

- **main** è dedicato a una versione semplice e affidabile.  
- **master** è utilizzato per miglioramenti, funzioni avanzate e sviluppi futuri.

---

## 🏗️ Architettura del sistema

Il sistema è organizzato su più livelli:

### 🔹 Livello applicativo (C)

Contiene la logica principale del programma:

- Gestione dei rotoli, prelievi e ritagli.
- Calcolo delle quantità residue e aggiornamento automatico.
- Ottimizzazione dei tagli per ridurre gli sprechi.

### 🔹 Livello di persistenza

- Salvataggio dei dati tramite **file binari locali**:
  - `backup_sartoria` → Contiene il backup binario del Gestionale.
  - `dati.json` → Disponibile solo nel branch Master per la comunicazione Web.
- I dati restano disponibili tra le sessioni e sono salvati automaticamente.

### 🔹 Interfaccia web

- Sviluppata con **HTML**, **CSS** e **JavaScript**.
- Consente la consultazione dell’inventario, la ricerca e il monitoraggio in tempo reale.

### 🔹 Servizi di supporto

- **GitHub** per versionamento e collaborazione.
- **Visual Studio Code** come ambiente di sviluppo principale.

---

## ⚙️ Requisiti funzionali

### 🧾 Gestione dei rotoli

- Inserimento, modifica ed eliminazione (logica) dei rotoli.
- Visualizzazione elenco completo.
- Ricerca per tipo, colore, fornitore o quantità residua.

### ✂️ Gestione prelievi

- Registrazione dei prelievi per progetto o cliente.
- Aggiornamento automatico del residuo.

### 🧶 Gestione ritagli e scarti

- Classificazione automatica:
  - **Riutilizzabili** ≥ 0,50 m
  - **Scarti** < 0,30 m
- Tracciabilità dei ritagli riutilizzabili.

### 🔍 Ricerca e filtraggio

- Filtri per tipo, colore, lotto o disponibilità minima.

### 📊 Storico e reportistica

- Log completo di tutte le operazioni.
- Report automatici su consumi, fornitori e utilizzo settimanale.

---

## 🧱 Strutture dati principali (in C)

```c
typedef struct
{
    int giorno;
    int mese;
    int anno;
} t_Data;

typedef struct
{
    char id[MAX_CARATTERI]; // auto incrementale
    char tipo[MAX_CARATTERI];
    char colore[MAX_CARATTERI];
    char fantasia[MAX_CARATTERI];
    float lunghezza_totale;  /* metri */
    float lunghezza_attuale; /* centimetri */
    float costo_metro;
    char fornitore[MAX_CARATTERI];
    char lotto[MAX_CARATTERI];
    t_Data data;
    char stato[MAX_CARATTERI];
    char noteAggiuntive[MAX_CHAR_NOTEAGGIUNTIVE];
} t_Rotolo;

typedef struct
{
    char id[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float metraggio_prelevato;
    t_Data data;
    char operatore[MAX_CARATTERI];
} t_Prelievo;

typedef struct
{
    char idRitaglio[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float lunghezza;
    t_Data data;
} t_Ritaglio;
typedef struct
{
    char nome[MAX_CARATTERI];
    char partita_iva[MAX_CARATTERI];
    char indirizzo[MAX_CARATTERI];
    char telefono[MAX_CARATTERI];
    char email[MAX_CARATTERI];
} t_Fornitore;

typedef struct
{
    char id[MAX_CARATTERI];
    char idCliente[MAX_CARATTERI];
    char tipo_capo[MAX_CARATTERI];
    char idRotolo[MAX_CARATTERI];
    char tessuto_usato[MAX_CARATTERI];
    t_Data data;
} t_Progetto;

typedef struct
{
    t_Data data_controllo;
    t_Rotolo rotoli[MAX_ROTOLI];
    float valoreTotale;
    float metraggioTotale;
    int numeroRotoli;
    int priorita_utilizzo;
} t_Magazzino;

```

Tutte sono pensate per essere **compatte**, **modulari** e facilmente **serializzabili in file binari**.

---

## 💾 Gestione della persistenza

- I dati vengono salvati e caricati automaticamente da **file binari**.
- **Backup** e **integrità** assicurati da salvataggi periodici.
- utlizzo e **sincronizzazione** con repository **GitHub**.
- Struttura predisposta per l’estensione futura a un **database SQL**.

---

## 🔒 Norme di sicurezza

- **Validazione input** su tutti i campi per evitare errori.
- **Backup automatici** su archivi esterni.
- **Log** con timestamp e operatore per ogni azione critica.

---

## ⚡ Aspetti di ottimizzazione

- Utilizzo di **tipi di dato compatti** per efficienza in memoria.
- **Lettura/scrittura binaria** per massime prestazioni.
- **Algoritmo di ottimizzazione dei tagli** per ridurre gli scarti.
- Progetto **scalabile**, pronto per un database o una rete di postazioni.

---

## 🧰 Considerazioni tecniche

### 🧠 Linguaggi

- **C** → logica e gestione dati
- **HTML**, **CSS**, **JavaScript** → interfaccia web

### ⚙️ Strumenti

- **Visual Studio Code** per scrittura e debug del codice
- **GitHub** per controllo versione e collaborazione

---

## 🚀 Estensioni future

- Integrazione con **scanner QR o RFID** per identificazione rapida dei rotoli.
- **Analisi predittiva** dei consumi.
- Collegamento a **macchinari tessili automatizzati**.
- Versione **online** con database e dashboard in tempo reale.

---

## 🏁 Conclusioni

Il **Sistema di Gestione Sartoria** rappresenta un progetto completo per la **digitalizzazione di una sartoria moderna**.  
Offre un approccio **professionale, modulare e facilmente estendibile**, applicabile sia in contesti **scolastici** che **reali**.

---

## 📂 Struttura del repository

```plaintext
SistemaGestioneSartoria/
│
├── programma/
│   ├── dati.json
│   └── main.c
│
├── file/
│   └── backup_sartoria.dat
│
├── web/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── README.md
├── .gitignore
└── LICENSE
```

---

## 👨‍💻 Autori

**Gabriele Rossoni e Marcello Patrini**
Studenti di Informatica — _ITIS Crema_  
📍 Lombardia, Italia

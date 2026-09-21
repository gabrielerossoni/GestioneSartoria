# Gestione Sartoria

Sistema di gestione sartoriale artigianale modulare sviluppato in **Java Swing**, evoluto per gestire in modo integrato commesse su misura, tracciamento materiali per piani di magazzino, anagrafica clienti, fornitori e organico dipendenti con relative forme contrattuali.

---

## 📌 Nuove Entità e Requisiti V2

Il sistema include la modellazione completa delle entità richieste per la Versione 2:

1. **Fornitori**
   - Ragione sociale / Nome e Cognome referente
   - Partita IVA e Codice Fiscale
   - Recapiti telefonici, email e indirizzo sede
   - Ricerca indicizzata per Partita IVA e Ragione Sociale

2. **Magazzino Diviso in Piani**
   - Suddivisione fisica e logica in **3 Piani**:
     - **Piano 0**: Area Ricevimento Merci e Scorte Pesanti
     - **Piano 1**: Reparto Tessuti Pregiati e Rotoli
     - **Piano 2**: Minuteria, Filati, Bottoni e Accessori
   - Ripartizione per codici scaffale/corsia (es. `A-12`, `B-04`)
   - Monitoraggio del valore economico complessivo e articoli stoccati per ciascun piano
   - Allarme scorte per prodotti sotto soglia minima

3. **Personale e Tipologie Contrattuali**
   - Anagrafica dipendenti e mansioni (Sarto Capo, Sarto Specializzato, Apprendista, Modellista, Receptionist)
   - **Tipologia di contratto**:
     - *Tempo Indeterminato*
     - *Tempo Determinato*
     - *Apprendistato*
     - *Stage / Tirocinio*
     - *Part-Time*
   - Retribuzione mensile, data assunzione e stato operativo (Attivo / In ferie / Cessato)
   - Filtraggio rapido del personale in base alla forma contrattuale

4. **Clienti (Committenti)**
   - Codice Fiscale e generalità complete
   - Recapiti di contatto (telefono, email)
   - Scheda misure sartoriali e note di conformazione fisica

5. **Componenti (Materiali di Creazione)**
   - Catalogo materiali: Tessuti, Minuterie, Fodere, Filati, Accessori
   - Tracciamento della giacenza e unità di misura (`m`, `cm`, `pezzi`, `rocchetti`)
   - Collegamento al fornitore di provenienza
   - Collocazione esatta (Piano magazzino e Scaffale)
   - Costo unitario e verifica disponibilità in tempo reale

6. **Progetti (Abiti su Misura)**
   - Commissione associata direttamente al cliente
   - Tipologia di capo (abito da cerimonia, completo uomo tre pezzi, camicia su misura, ecc.)
   - Sarto responsabile assegnato al progetto
   - **Distinta base materiali**: prelievo e scarico automatico dei componenti dal magazzino
   - Stato avanzamento lavorazione (*In Attesa*, *In Lavorazione*, *Prima Prova*, *Completato*, *Consegnato*)
   - Data inizio, data consegna promessa e preventivo finale

---

## 🏗️ Persistenza Dati

I dati della V2 vengono salvati in modalità binaria su:

- `java/data/archivio_sartoria_v2.dat`

In questo modo i dati e i backup della versione precedente (V1) rimangono intatti e protetti da sovrascritture.

---

## 🌿 Organizzazione Branch Git

- **v2-dev:** branch attivo di sviluppo per la nuova architettura V2.
- **legacy/v1-web:** branch storico contenente la prima versione del progetto con interfaccia web.
- **main:** branch base originale.

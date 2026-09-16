// Gabriele Rossoni e Marcello Patrini - 4IB
// PROGETTO: Gestione Sartoria Digitale - VERSIONE 2 (V2)
// Repository GitHub: https://github.com/gabrielerossoni/GestioneSartoria (branch v2-dev)

// ---LIBRERIE---
#include <stdio.h>      // Input/Output standard (printf, scanf, fgets, fopen, fclose, fwrite, fread)
#include <stdlib.h>     // Funzioni di utilità generale
#include <string.h>     // Manipolazione stringhe (strcpy, strcmp, strlen, strcspn, ecc.)
#include <ctype.h>      // Funzioni di classificazione caratteri
#include <time.h>       // Gestione di data e ora correnti

// ---COSTANTI DIMENSIONALI---
#define MAX_CARATTERI 50                // Lunghezza standard per stringhe (ID, nomi, codici)
#define MAX_DESCRIZIONE 100             // Lunghezza per note o descrizioni estese
#define MAX_FORNITORI 200               // Numero massimo fornitori
#define MAX_CLIENTI 300                 // Numero massimo clienti committenti
#define MAX_PERSONALE 100               // Numero massimo dipendenti/personale
#define MAX_COMPONENTI 500              // Numero massimo componenti a magazzino
#define MAX_PROGETTI 300                // Numero massimo progetti/abiti
#define MAX_COMP_PER_PROGETTO 15        // Numero max componenti diversi impiegati per singolo abito
#define NUMERO_PIANI 3                  // Numero di piani del magazzino (0 = Piano Terra/Scorte, 1 = Tessuti/Rotoli, 2 = Minuteria/Accessori)

// ---COSTANTI DATA---
#define ANNO_MIN 1900
#define ANNO_MAX 2100

// ---NOME FILE BACKUP V2---
#define FN_BACKUP_V2 "../file/backup_sartoria_v2.dat"

// ---STRUTTURE DATI---

// Data generica (Giorno, Mese, Anno)
typedef struct {
    int giorno;
    int mese;
    int anno;
} t_Data;

// 1. FORNITORI (Nome, Cognome/Ragione sociale, Partita IVA, contatti)
typedef struct {
    char id[MAX_CARATTERI];                 // es. FOR0001
    char nome[MAX_CARATTERI];               // Nome referente o ragione sociale
    char cognome[MAX_CARATTERI];            // Cognome referente
    char partita_iva[MAX_CARATTERI];        // Partita IVA o Codice Fiscale azienda
    char telefono[MAX_CARATTERI];           // Recapito telefonico
    char email[MAX_CARATTERI];              // Email fornitore
    char indirizzo[MAX_DESCRIZIONE];        // Indirizzo sede
} t_Fornitore;

// 2. CLIENTI (Chi commissiona il lavoro)
typedef struct {
    char id[MAX_CARATTERI];                 // es. CLI0001
    char codice_fiscale[MAX_CARATTERI];     // CF univoco cliente
    char nome[MAX_CARATTERI];               // Nome cliente
    char cognome[MAX_CARATTERI];            // Cognome cliente
    char telefono[MAX_CARATTERI];           // Telefono cliente
    char email[MAX_CARATTERI];              // Email cliente
    char note_misure[MAX_DESCRIZIONE];      // Note su taglia, conformazione e misure
} t_Cliente;

// 3. PERSONALE (Dipendenti, ruoli e contratti)
typedef struct {
    char id[MAX_CARATTERI];                 // es. DIP0001
    char nome[MAX_CARATTERI];               // Nome dipendente
    char cognome[MAX_CARATTERI];            // Cognome dipendente
    char ruolo[MAX_CARATTERI];              // Ruolo (Sarto Principale, Sarto Specializzato, Apprendista, Magazziniere, Receptionist)
    char tipo_contratto[MAX_CARATTERI];     // Tipo di contratto (Indeterminato, Determinato, Apprendistato, Stage, Part-Time)
    float retribuzione_mensile;             // Retribuzione o compenso pattuito
    t_Data data_assunzione;                 // Data di assunzione
    char stato[MAX_CARATTERI];              // Stato (ATTIVO, FERIE, CESSATO)
} t_Personale;

// 4. COMPONENTI (Elementi gestiti a magazzino per creare progetti/abiti)
typedef struct {
    char id[MAX_CARATTERI];                 // es. CMP0001
    char nome[MAX_CARATTERI];               // es. Tessuto Seta Nera, Bottoni Madreperla, Cerniera Lampo 30cm
    char categoria[MAX_CARATTERI];          // TESSUTO, MINUTERIA, FODERA, ACCESSORIO, FILATO
    float quantita_disponibile;             // Quantita residua in magazzino
    char unita_misura[MAX_CARATTERI];       // m, cm, pezzi, rocchetti
    float costo_unitario;                   // Costo unitario in Euro
    char id_fornitore[MAX_CARATTERI];       // ID o P.IVA del fornitore da cui e' stato acquistato
    int piano_magazzino;                    // Piano fisico in magazzino (0, 1 o 2)
    char scaffale[MAX_CARATTERI];           // Codice scaffale/settore (es. A1, B3, C2)
    float soglia_minima;                    // Soglia minima di riordino
} t_Componente;

// Elemento componente associato a un progetto
typedef struct {
    char id_componente[MAX_CARATTERI];      // Riferimento al componente
    float quantita_usata;                   // Quantita impiegata
} t_ComponenteUsato;

// 5. PROGETTI (Abiti realizzati su commissione)
typedef struct {
    char id[MAX_CARATTERI];                 // es. PRJ0001
    char id_cliente[MAX_CARATTERI];         // Riferimento al cliente committente
    char tipo_capo[MAX_CARATTERI];          // es. Completo giacca e pantalone, Abito da sera, Camicia su misura
    char id_sarto_assegnato[MAX_CARATTERI]; // Riferimento al personale incaricato
    char stato_avanzamento[MAX_CARATTERI];  // IN ATTESA, IN LAVORAZIONE, PRIMA PROVA, COMPLETATO, CONSEGNATO
    t_Data data_inizio;                     // Data avvio progetto
    t_Data data_consegna_prevista;          // Data promessa al cliente
    float prezzo_finale;                    // Preventivo / prezzo finale al cliente
    int num_componenti;                     // Numero componenti impiegati
    t_ComponenteUsato componenti[MAX_COMP_PER_PROGETTO]; // Lista dei materiali usati
} t_Progetto;

// ---PROTOTIPI FUNZIONI---
void pulisciBuffer();
int controlloData(t_Data d);
void stampaData(t_Data d);

// Menu
int menuGenerale();
int menuFornitori();
int menuClienti();
int menuPersonale();
int menuComponenti();
int menuProgetti();
int menuMagazzinoPiani();

// Gestione Fornitori
int inserisciFornitore(t_Fornitore fornitori[], int *nFornitori);
int modificaFornitore(t_Fornitore fornitori[], int nFornitori, const char *id);
int eliminaFornitore(t_Fornitore fornitori[], int *nFornitori, const char *id);
void visualizzaFornitori(t_Fornitore fornitori[], int nFornitori);
int cercaFornitore(t_Fornitore fornitori[], int nFornitori, const char *ricerca);

// Gestione Clienti
int inserisciCliente(t_Cliente clienti[], int *nClienti);
int modificaCliente(t_Cliente clienti[], int nClienti, const char *id);
int eliminaCliente(t_Cliente clienti[], int *nClienti, const char *id);
void visualizzaClienti(t_Cliente clienti[], int nClienti);
int cercaCliente(t_Cliente clienti[], int nClienti, const char *ricerca);

// Gestione Personale
int inserisciPersonale(t_Personale personale[], int *nPersonale);
int modificaPersonale(t_Personale personale[], int nPersonale, const char *id);
int eliminaPersonale(t_Personale personale[], int *nPersonale, const char *id);
void visualizzaPersonale(t_Personale personale[], int nPersonale);
int cercaPersonalePerContratto(t_Personale personale[], int nPersonale, const char *contratto);

// Gestione Componenti & Magazzino
int inserisciComponente(t_Componente componenti[], int *nComponenti);
int modificaComponente(t_Componente componenti[], int nComponenti, const char *id);
int eliminaComponente(t_Componente componenti[], int *nComponenti, const char *id);
void visualizzaComponenti(t_Componente componenti[], int nComponenti);
int cercaComponente(t_Componente componenti[], int nComponenti, const char *termine);
void visualizzaMagazzinoPerPiano(t_Componente componenti[], int nComponenti, int piano);
void controlloSottoscorta(t_Componente componenti[], int nComponenti);

// Gestione Progetti (Abiti)
int inserisciProgetto(t_Progetto progetti[], int *nProgetti, t_Cliente clienti[], int nClienti,
                      t_Personale personale[], int nPersonale, t_Componente componenti[], int nComponenti);
int modificaStatoProgetto(t_Progetto progetti[], int nProgetti, const char *id);
int eliminaProgetto(t_Progetto progetti[], int *nProgetti, const char *id);
void visualizzaProgetti(t_Progetto progetti[], int nProgetti);
int cercaProgetto(t_Progetto progetti[], int nProgetti, const char *id);

// Persistenza Dati V2
int SalvaTuttoSuFileV2(t_Fornitore fornitori[], int nFornitori,
                      t_Cliente clienti[], int nClienti,
                      t_Personale personale[], int nPersonale,
                      t_Componente componenti[], int nComponenti,
                      t_Progetto progetti[], int nProgetti);
int CaricaTuttoDaFileV2(t_Fornitore fornitori[], int *nFornitori,
                       t_Cliente clienti[], int *nClienti,
                       t_Personale personale[], int *nPersonale,
                       t_Componente componenti[], int *nComponenti,
                       t_Progetto progetti[], int *nProgetti);

// ---MAIN---
int main() {
    t_Fornitore fornitori[MAX_FORNITORI];
    t_Cliente clienti[MAX_CLIENTI];
    t_Personale personale[MAX_PERSONALE];
    t_Componente componenti[MAX_COMPONENTI];
    t_Progetto progetti[MAX_PROGETTI];

    int nFornitori = 0;
    int nClienti = 0;
    int nPersonale = 0;
    int nComponenti = 0;
    int nProgetti = 0;

    int sceltaGenerale, sceltaSub;
    char bufferID[MAX_CARATTERI];
    char bufferFiltro[MAX_CARATTERI];
    int esito;
    int terminato = 0;

    printf("========================================================\n");
    printf("     BENVENUTO NEL SISTEMA SARTORIA DIGITALE (V2)      \n");
    printf("========================================================\n");

    // Caricamento dati da file binario V2
    esito = CaricaTuttoDaFileV2(fornitori, &nFornitori, clienti, &nClienti, personale, &nPersonale,
                                componenti, &nComponenti, progetti, &nProgetti);
    if (esito == 0) {
        printf("[OK] Dati V2 caricati: %d Fornitori, %d Clienti, %d Dipendenti, %d Componenti, %d Progetti\n",
               nFornitori, nClienti, nPersonale, nComponenti, nProgetti);
    } else {
        printf("[INFO] Nessun archivio V2 trovato. Inizializzazione archivio vuoto.\n");
    }

    do {
        sceltaGenerale = menuGenerale();
        switch (sceltaGenerale) {

        // --- 1. GESTIONE FORNITORI ---
        case 1:
            do {
                sceltaSub = menuFornitori();
                switch (sceltaSub) {
                case 1:
                    inserisciFornitore(fornitori, &nFornitori);
                    break;
                case 2:
                    printf("Inserisci ID o P.IVA fornitore da modificare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (modificaFornitore(fornitori, nFornitori, bufferID) == 0)
                        printf("Fornitore modificato con successo.\n");
                    else
                        printf("Fornitore non trovato.\n");
                    break;
                case 3:
                    printf("Inserisci ID fornitore da eliminare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (eliminaFornitore(fornitori, &nFornitori, bufferID) == 0)
                        printf("Fornitore eliminato con successo.\n");
                    else
                        printf("Fornitore non trovato.\n");
                    break;
                case 4:
                    visualizzaFornitori(fornitori, nFornitori);
                    break;
                case 5:
                    printf("Inserisci P.IVA o Cognome/Ragione Sociale da cercare: ");
                    scanf("%49s", bufferFiltro);
                    pulisciBuffer();
                    cercaFornitore(fornitori, nFornitori, bufferFiltro);
                    break;
                case 6:
                    break; // Torna indietro
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 6);
            break;

        // --- 2. GESTIONE CLIENTI (Committenti) ---
        case 2:
            do {
                sceltaSub = menuClienti();
                switch (sceltaSub) {
                case 1:
                    inserisciCliente(clienti, &nClienti);
                    break;
                case 2:
                    printf("Inserisci ID o Codice Fiscale cliente da modificare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (modificaCliente(clienti, nClienti, bufferID) == 0)
                        printf("Cliente aggiornato con successo.\n");
                    else
                        printf("Cliente non trovato.\n");
                    break;
                case 3:
                    printf("Inserisci ID cliente da eliminare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (eliminaCliente(clienti, &nClienti, bufferID) == 0)
                        printf("Cliente rimosso con successo.\n");
                    else
                        printf("Cliente non trovato.\n");
                    break;
                case 4:
                    visualizzaClienti(clienti, nClienti);
                    break;
                case 5:
                    printf("Inserisci Codice Fiscale o Cognome cliente da cercare: ");
                    scanf("%49s", bufferFiltro);
                    pulisciBuffer();
                    cercaCliente(clienti, nClienti, bufferFiltro);
                    break;
                case 6:
                    break;
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 6);
            break;

        // --- 3. GESTIONE PERSONALE & CONTRATTI ---
        case 3:
            do {
                sceltaSub = menuPersonale();
                switch (sceltaSub) {
                case 1:
                    inserisciPersonale(personale, &nPersonale);
                    break;
                case 2:
                    printf("Inserisci ID dipendente da modificare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (modificaPersonale(personale, nPersonale, bufferID) == 0)
                        printf("Dati dipendente aggiornati.\n");
                    else
                        printf("Dipendente non trovato.\n");
                    break;
                case 3:
                    printf("Inserisci ID dipendente da eliminare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (eliminaPersonale(personale, &nPersonale, bufferID) == 0)
                        printf("Dipendente rimosso con successo.\n");
                    else
                        printf("Dipendente non trovato.\n");
                    break;
                case 4:
                    visualizzaPersonale(personale, nPersonale);
                    break;
                case 5:
                    printf("Inserisci tipologia di contratto da filtrare (es. Apprendistato, Indeterminato): ");
                    scanf("%49s", bufferFiltro);
                    pulisciBuffer();
                    cercaPersonalePerContratto(personale, nPersonale, bufferFiltro);
                    break;
                case 6:
                    break;
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 6);
            break;

        // --- 4. GESTIONE COMPONENTI & MATERIALI ---
        case 4:
            do {
                sceltaSub = menuComponenti();
                switch (sceltaSub) {
                case 1:
                    inserisciComponente(componenti, &nComponenti);
                    break;
                case 2:
                    printf("Inserisci ID componente da modificare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (modificaComponente(componenti, nComponenti, bufferID) == 0)
                        printf("Componente modificato con successo.\n");
                    else
                        printf("Componente non trovato.\n");
                    break;
                case 3:
                    printf("Inserisci ID componente da eliminare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (eliminaComponente(componenti, &nComponenti, bufferID) == 0)
                        printf("Componente eliminato con successo.\n");
                    else
                        printf("Componente non trovato.\n");
                    break;
                case 4:
                    visualizzaComponenti(componenti, nComponenti);
                    break;
                case 5:
                    printf("Inserisci nome o categoria da cercare: ");
                    scanf("%49s", bufferFiltro);
                    pulisciBuffer();
                    cercaComponente(componenti, nComponenti, bufferFiltro);
                    break;
                case 6:
                    controlloSottoscorta(componenti, nComponenti);
                    break;
                case 7:
                    break;
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 7);
            break;

        // --- 5. GESTIONE MAGAZZINO A PIANI ---
        case 5:
            do {
                sceltaSub = menuMagazzinoPiani();
                switch (sceltaSub) {
                case 1:
                    visualizzaMagazzinoPerPiano(componenti, nComponenti, 0);
                    break;
                case 2:
                    visualizzaMagazzinoPerPiano(componenti, nComponenti, 1);
                    break;
                case 3:
                    visualizzaMagazzinoPerPiano(componenti, nComponenti, 2);
                    break;
                case 4:
                    printf("\n--- RIEPILOGO COMPLETO MAGAZZINO (TUTTI I PIANI) ---\n");
                    for (int p = 0; p < NUMERO_PIANI; p++) {
                        visualizzaMagazzinoPerPiano(componenti, nComponenti, p);
                    }
                    break;
                case 5:
                    break;
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 5);
            break;

        // --- 6. GESTIONE PROGETTI (Abiti) ---
        case 6:
            do {
                sceltaSub = menuProgetti();
                switch (sceltaSub) {
                case 1:
                    inserisciProgetto(progetti, &nProgetti, clienti, nClienti, personale, nPersonale, componenti, nComponenti);
                    break;
                case 2:
                    printf("Inserisci ID progetto di cui aggiornare lo stato: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    modificaStatoProgetto(progetti, nProgetti, bufferID);
                    break;
                case 3:
                    printf("Inserisci ID progetto da eliminare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    if (eliminaProgetto(progetti, &nProgetti, bufferID) == 0)
                        printf("Progetto eliminato con successo.\n");
                    else
                        printf("Progetto non trovato.\n");
                    break;
                case 4:
                    visualizzaProgetti(progetti, nProgetti);
                    break;
                case 5:
                    printf("Inserisci ID progetto da cercare: ");
                    scanf("%49s", bufferID);
                    pulisciBuffer();
                    cercaProgetto(progetti, nProgetti, bufferID);
                    break;
                case 6:
                    break;
                default:
                    printf("Scelta non valida!\n");
                    break;
                }
            } while (sceltaSub != 6);
            break;

        // --- 7. SALVATAGGIO ED ESCI ---
        case 7:
            if (SalvaTuttoSuFileV2(fornitori, nFornitori, clienti, nClienti, personale, nPersonale,
                                  componenti, nComponenti, progetti, nProgetti) == 0) {
                printf("\n[SUCCESSO] Tutti i dati V2 sono stati salvati in %s!\n", FN_BACKUP_V2);
                terminato = 1;
            } else {
                printf("\n[ERRORE] Impossibile salvare i dati su file.\n");
            }
            break;

        default:
            printf("Opzione non valida. Riprova!\n");
            break;
        }
    } while (!terminato);

    printf("\nGrazie per aver utilizzato Gestione Sartoria V2. Arrivederci!\n");
    return 0;
}

// ---UTILITY INPUT---
void pulisciBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int controlloData(t_Data d) {
    if (d.anno < ANNO_MIN || d.anno > ANNO_MAX) return 0;
    if (d.mese < 1 || d.mese > 12) return 0;
    if (d.giorno < 1 || d.giorno > 31) return 0;
    if (d.mese == 2) {
        int bisestile = (d.anno % 4 == 0 && d.anno % 100 != 0) || (d.anno % 400 == 0);
        if (bisestile && d.giorno > 29) return 0;
        if (!bisestile && d.giorno > 28) return 0;
    } else if (d.mese == 4 || d.mese == 6 || d.mese == 9 || d.mese == 11) {
        if (d.giorno > 30) return 0;
    }
    return 1;
}

void stampaData(t_Data d) {
    printf("%02d/%02d/%04d", d.giorno, d.mese, d.anno);
}

// ---MENU---
int menuGenerale() {
    int s;
    printf("\n=========================================\n");
    printf("         MENU PRINCIPALE (V2)            \n");
    printf("=========================================\n");
    printf("1. Gestione FORNITORI (P.IVA, dati)\n");
    printf("2. Gestione CLIENTI (Committenti)\n");
    printf("3. Gestione PERSONALE (Contratti, Ruoli)\n");
    printf("4. Gestione COMPONENTI (Materiali)\n");
    printf("5. Gestione MAGAZZINO (Diviso in Piani)\n");
    printf("6. Gestione PROGETTI (Abiti su misura)\n");
    printf("7. Salva ed Esci\n");
    printf("-----------------------------------------\n");
    printf("Seleziona un'opzione: ");
    if (scanf("%d", &s) != 1) {
        pulisciBuffer();
        return -1;
    }
    pulisciBuffer();
    return s;
}

int menuFornitori() {
    int s;
    printf("\n--- GESTIONE FORNITORI ---\n");
    printf("1. Aggiungi Fornitore\n");
    printf("2. Modifica Fornitore\n");
    printf("3. Elimina Fornitore\n");
    printf("4. Visualizza Elenco Fornitori\n");
    printf("5. Cerca Fornitore (per P.IVA o Nome)\n");
    printf("6. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

int menuClienti() {
    int s;
    printf("\n--- GESTIONE CLIENTI (COMMITTENTI) ---\n");
    printf("1. Registra Nuovo Cliente\n");
    printf("2. Modifica Dati/Misure Cliente\n");
    printf("3. Elimina Cliente\n");
    printf("4. Visualizza Elenco Clienti\n");
    printf("5. Cerca Cliente (per Codice Fiscale o Cognome)\n");
    printf("6. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

int menuPersonale() {
    int s;
    printf("\n--- GESTIONE PERSONALE & CONTRATTI ---\n");
    printf("1. Inserisci Nuovo Dipendente\n");
    printf("2. Modifica Dipendente / Ruolo / Contratto\n");
    printf("3. Elimina Dipendente\n");
    printf("4. Visualizza Personale Completo\n");
    printf("5. Filtra Dipendenti per Tipo di Contratto\n");
    printf("6. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

int menuComponenti() {
    int s;
    printf("\n--- GESTIONE COMPONENTI & MATERIALI ---\n");
    printf("1. Registra Nuovo Componente a Magazzino\n");
    printf("2. Modifica Componente (Giacenza/Prezzo/Scaffale)\n");
    printf("3. Elimina Componente\n");
    printf("4. Visualizza Tutti i Componenti\n");
    printf("5. Cerca Componente per Nome / Categoria\n");
    printf("6. Verifica Componenti Sotto-Scorta\n");
    printf("7. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

int menuMagazzinoPiani() {
    int s;
    printf("\n--- GESTIONE MAGAZZINO PER PIANI ---\n");
    printf("1. Piano 0: Ricevimento / Scorte Pesanti\n");
    printf("2. Piano 1: Tessuti & Rotoli Pregiati\n");
    printf("3. Piano 2: Minuteria, Filati & Accessori\n");
    printf("4. Visualizza Tutti i Piani (Riepilogo Totale)\n");
    printf("5. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

int menuProgetti() {
    int s;
    printf("\n--- GESTIONE PROGETTI & ABITI ---\n");
    printf("1. Crea Nuovo Progetto Abito (Commissione Cliente)\n");
    printf("2. Avanza / Modifica Stato Progetto\n");
    printf("3. Elimina Progetto\n");
    printf("4. Visualizza Tutti i Progetti\n");
    printf("5. Cerca Progetto per ID\n");
    printf("6. Torna al menu principale\n");
    printf("Scelta: ");
    if (scanf("%d", &s) != 1) { pulisciBuffer(); return -1; }
    pulisciBuffer();
    return s;
}

// ---GESTIONE FORNITORI---
int inserisciFornitore(t_Fornitore fornitori[], int *nFornitori) {
    if (*nFornitori >= MAX_FORNITORI) {
        printf("[ERRORE] Limite massimo fornitori raggiunto (%d).\n", MAX_FORNITORI);
        return -1;
    }
    int idx = *nFornitori;
    sprintf(fornitori[idx].id, "FOR%04d", idx + 1);

    printf("\n--- Inserimento Fornitore (%s) ---\n", fornitori[idx].id);
    printf("Nome referente / Ragione Sociale: ");
    fgets(fornitori[idx].nome, MAX_CARATTERI, stdin);
    fornitori[idx].nome[strcspn(fornitori[idx].nome, "\n")] = 0;

    printf("Cognome referente: ");
    fgets(fornitori[idx].cognome, MAX_CARATTERI, stdin);
    fornitori[idx].cognome[strcspn(fornitori[idx].cognome, "\n")] = 0;

    printf("Partita IVA / CF: ");
    fgets(fornitori[idx].partita_iva, MAX_CARATTERI, stdin);
    fornitori[idx].partita_iva[strcspn(fornitori[idx].partita_iva, "\n")] = 0;

    printf("Telefono: ");
    fgets(fornitori[idx].telefono, MAX_CARATTERI, stdin);
    fornitori[idx].telefono[strcspn(fornitori[idx].telefono, "\n")] = 0;

    printf("Email: ");
    fgets(fornitori[idx].email, MAX_CARATTERI, stdin);
    fornitori[idx].email[strcspn(fornitori[idx].email, "\n")] = 0;

    printf("Indirizzo sede: ");
    fgets(fornitori[idx].indirizzo, MAX_DESCRIZIONE, stdin);
    fornitori[idx].indirizzo[strcspn(fornitori[idx].indirizzo, "\n")] = 0;

    (*nFornitori)++;
    printf("[OK] Fornitore %s inserito con successo!\n", fornitori[idx].id);
    return 0;
}

int modificaFornitore(t_Fornitore fornitori[], int nFornitori, const char *id) {
    for (int i = 0; i < nFornitori; i++) {
        if (strcmp(fornitori[i].id, id) == 0 || strcmp(fornitori[i].partita_iva, id) == 0) {
            printf("\n--- Modifica Fornitore %s (%s %s) ---\n", fornitori[i].id, fornitori[i].nome, fornitori[i].cognome);
            printf("Nuovo Nome/Ragione Sociale: ");
            fgets(fornitori[i].nome, MAX_CARATTERI, stdin);
            fornitori[i].nome[strcspn(fornitori[i].nome, "\n")] = 0;

            printf("Nuovo Cognome: ");
            fgets(fornitori[i].cognome, MAX_CARATTERI, stdin);
            fornitori[i].cognome[strcspn(fornitori[i].cognome, "\n")] = 0;

            printf("Nuova Partita IVA: ");
            fgets(fornitori[i].partita_iva, MAX_CARATTERI, stdin);
            fornitori[i].partita_iva[strcspn(fornitori[i].partita_iva, "\n")] = 0;

            printf("Nuovo Telefono: ");
            fgets(fornitori[i].telefono, MAX_CARATTERI, stdin);
            fornitori[i].telefono[strcspn(fornitori[i].telefono, "\n")] = 0;

            printf("Nuova Email: ");
            fgets(fornitori[i].email, MAX_CARATTERI, stdin);
            fornitori[i].email[strcspn(fornitori[i].email, "\n")] = 0;
            return 0;
        }
    }
    return -1;
}

int eliminaFornitore(t_Fornitore fornitori[], int *nFornitori, const char *id) {
    for (int i = 0; i < *nFornitori; i++) {
        if (strcmp(fornitori[i].id, id) == 0 || strcmp(fornitori[i].partita_iva, id) == 0) {
            for (int j = i; j < *nFornitori - 1; j++) {
                fornitori[j] = fornitori[j + 1];
            }
            (*nFornitori)--;
            return 0;
        }
    }
    return -1;
}

void visualizzaFornitori(t_Fornitore fornitori[], int nFornitori) {
    printf("\n============================= ELENCO FORNITORI =============================\n");
    if (nFornitori == 0) {
        printf("Nessun fornitore registrato.\n");
        return;
    }
    for (int i = 0; i < nFornitori; i++) {
        printf("[%s] %s %s | P.IVA: %s | Tel: %s | Mail: %s | Sede: %s\n",
               fornitori[i].id, fornitori[i].nome, fornitori[i].cognome,
               fornitori[i].partita_iva, fornitori[i].telefono, fornitori[i].email, fornitori[i].indirizzo);
    }
    printf("Totale Fornitori: %d\n", nFornitori);
}

int cercaFornitore(t_Fornitore fornitori[], int nFornitori, const char *ricerca) {
    int trovati = 0;
    printf("\n--- Risultati ricerca fornitore per '%s' ---\n", ricerca);
    for (int i = 0; i < nFornitori; i++) {
        if (strstr(fornitori[i].partita_iva, ricerca) != NULL ||
            strstr(fornitori[i].nome, ricerca) != NULL ||
            strstr(fornitori[i].cognome, ricerca) != NULL) {
            printf("[%s] %s %s | P.IVA: %s | Tel: %s | Mail: %s\n",
                   fornitori[i].id, fornitori[i].nome, fornitori[i].cognome,
                   fornitori[i].partita_iva, fornitori[i].telefono, fornitori[i].email);
            trovati++;
        }
    }
    if (trovati == 0) printf("Nessun fornitore corrispondente ai criteri.\n");
    return trovati;
}

// ---GESTIONE CLIENTI---
int inserisciCliente(t_Cliente clienti[], int *nClienti) {
    if (*nClienti >= MAX_CLIENTI) {
        printf("[ERRORE] Limite massimo clienti raggiunto (%d).\n", MAX_CLIENTI);
        return -1;
    }
    int idx = *nClienti;
    sprintf(clienti[idx].id, "CLI%04d", idx + 1);

    printf("\n--- Registrazione Nuovo Cliente (%s) ---\n", clienti[idx].id);
    printf("Codice Fiscale: ");
    fgets(clienti[idx].codice_fiscale, MAX_CARATTERI, stdin);
    clienti[idx].codice_fiscale[strcspn(clienti[idx].codice_fiscale, "\n")] = 0;

    printf("Nome: ");
    fgets(clienti[idx].nome, MAX_CARATTERI, stdin);
    clienti[idx].nome[strcspn(clienti[idx].nome, "\n")] = 0;

    printf("Cognome: ");
    fgets(clienti[idx].cognome, MAX_CARATTERI, stdin);
    clienti[idx].cognome[strcspn(clienti[idx].cognome, "\n")] = 0;

    printf("Telefono: ");
    fgets(clienti[idx].telefono, MAX_CARATTERI, stdin);
    clienti[idx].telefono[strcspn(clienti[idx].telefono, "\n")] = 0;

    printf("Email: ");
    fgets(clienti[idx].email, MAX_CARATTERI, stdin);
    clienti[idx].email[strcspn(clienti[idx].email, "\n")] = 0;

    printf("Note Misure (es. Spalle 44cm, Vita 82cm, Torace 96cm): ");
    fgets(clienti[idx].note_misure, MAX_DESCRIZIONE, stdin);
    clienti[idx].note_misure[strcspn(clienti[idx].note_misure, "\n")] = 0;

    (*nClienti)++;
    printf("[OK] Cliente %s inserito con successo!\n", clienti[idx].id);
    return 0;
}

int modificaCliente(t_Cliente clienti[], int nClienti, const char *id) {
    for (int i = 0; i < nClienti; i++) {
        if (strcmp(clienti[i].id, id) == 0 || strcmp(clienti[i].codice_fiscale, id) == 0) {
            printf("\n--- Modifica Cliente %s (%s %s) ---\n", clienti[i].id, clienti[i].nome, clienti[i].cognome);
            printf("Nuovo Nome: ");
            fgets(clienti[i].nome, MAX_CARATTERI, stdin);
            clienti[i].nome[strcspn(clienti[i].nome, "\n")] = 0;

            printf("Nuovo Cognome: ");
            fgets(clienti[i].cognome, MAX_CARATTERI, stdin);
            clienti[i].cognome[strcspn(clienti[i].cognome, "\n")] = 0;

            printf("Nuovo Telefono: ");
            fgets(clienti[i].telefono, MAX_CARATTERI, stdin);
            clienti[i].telefono[strcspn(clienti[i].telefono, "\n")] = 0;

            printf("Nuova Email: ");
            fgets(clienti[i].email, MAX_CARATTERI, stdin);
            clienti[i].email[strcspn(clienti[i].email, "\n")] = 0;

            printf("Nuove Misure / Note: ");
            fgets(clienti[i].note_misure, MAX_DESCRIZIONE, stdin);
            clienti[i].note_misure[strcspn(clienti[i].note_misure, "\n")] = 0;
            return 0;
        }
    }
    return -1;
}

int eliminaCliente(t_Cliente clienti[], int *nClienti, const char *id) {
    for (int i = 0; i < *nClienti; i++) {
        if (strcmp(clienti[i].id, id) == 0 || strcmp(clienti[i].codice_fiscale, id) == 0) {
            for (int j = i; j < *nClienti - 1; j++) {
                clienti[j] = clienti[j + 1];
            }
            (*nClienti)--;
            return 0;
        }
    }
    return -1;
}

void visualizzaClienti(t_Cliente clienti[], int nClienti) {
    printf("\n============================== ELENCO CLIENTI ==============================\n");
    if (nClienti == 0) {
        printf("Nessun cliente registrato.\n");
        return;
    }
    for (int i = 0; i < nClienti; i++) {
        printf("[%s] %s %s | CF: %s | Tel: %s | Misure: %s\n",
               clienti[i].id, clienti[i].nome, clienti[i].cognome,
               clienti[i].codice_fiscale, clienti[i].telefono, clienti[i].note_misure);
    }
    printf("Totale Clienti: %d\n", nClienti);
}

int cercaCliente(t_Cliente clienti[], int nClienti, const char *ricerca) {
    int trovati = 0;
    printf("\n--- Ricerca Cliente '%s' ---\n", ricerca);
    for (int i = 0; i < nClienti; i++) {
        if (strstr(clienti[i].codice_fiscale, ricerca) != NULL ||
            strstr(clienti[i].cognome, ricerca) != NULL ||
            strstr(clienti[i].nome, ricerca) != NULL) {
            printf("[%s] %s %s | CF: %s | Tel: %s | Mail: %s | Note: %s\n",
                   clienti[i].id, clienti[i].nome, clienti[i].cognome,
                   clienti[i].codice_fiscale, clienti[i].telefono, clienti[i].email, clienti[i].note_misure);
            trovati++;
        }
    }
    if (trovati == 0) printf("Nessun cliente trovato con questi dati.\n");
    return trovati;
}

// ---GESTIONE PERSONALE & CONTRATTI---
int inserisciPersonale(t_Personale personale[], int *nPersonale) {
    if (*nPersonale >= MAX_PERSONALE) {
        printf("[ERRORE] Limite massimo personale raggiunto (%d).\n", MAX_PERSONALE);
        return -1;
    }
    int idx = *nPersonale;
    sprintf(personale[idx].id, "DIP%04d", idx + 1);

    printf("\n--- Inserimento Personale (%s) ---\n", personale[idx].id);
    printf("Nome: ");
    fgets(personale[idx].nome, MAX_CARATTERI, stdin);
    personale[idx].nome[strcspn(personale[idx].nome, "\n")] = 0;

    printf("Cognome: ");
    fgets(personale[idx].cognome, MAX_CARATTERI, stdin);
    personale[idx].cognome[strcspn(personale[idx].cognome, "\n")] = 0;

    printf("Ruolo (es. Sarto Specializzato, Apprendista, Modellista, Receptionist): ");
    fgets(personale[idx].ruolo, MAX_CARATTERI, stdin);
    personale[idx].ruolo[strcspn(personale[idx].ruolo, "\n")] = 0;

    printf("Tipo di Contratto (1. Indeterminato | 2. Determinato | 3. Apprendistato | 4. Stage | 5. Part-Time): ");
    int tipoC;
    if (scanf("%d", &tipoC) == 1) {
        switch (tipoC) {
            case 1: strcpy(personale[idx].tipo_contratto, "Indeterminato"); break;
            case 2: strcpy(personale[idx].tipo_contratto, "Determinato"); break;
            case 3: strcpy(personale[idx].tipo_contratto, "Apprendistato"); break;
            case 4: strcpy(personale[idx].tipo_contratto, "Stage"); break;
            case 5: strcpy(personale[idx].tipo_contratto, "Part-Time"); break;
            default: strcpy(personale[idx].tipo_contratto, "Altro"); break;
        }
    } else {
        strcpy(personale[idx].tipo_contratto, "NonSpecificato");
    }
    pulisciBuffer();

    printf("Retribuzione mensile (Euro): ");
    scanf("%f", &personale[idx].retribuzione_mensile);
    pulisciBuffer();

    while (1) {
        printf("Data di Assunzione (GG MM AAAA): ");
        int ok = scanf("%d %d %d", &personale[idx].data_assunzione.giorno,
                       &personale[idx].data_assunzione.mese,
                       &personale[idx].data_assunzione.anno);
        pulisciBuffer();
        if (ok == 3 && controlloData(personale[idx].data_assunzione)) break;
        printf("Data non valida. Riprova!\n");
    }

    strcpy(personale[idx].stato, "ATTIVO");
    (*nPersonale)++;
    printf("[OK] Dipendente %s registrato con successo!\n", personale[idx].id);
    return 0;
}

int modificaPersonale(t_Personale personale[], int nPersonale, const char *id) {
    for (int i = 0; i < nPersonale; i++) {
        if (strcmp(personale[i].id, id) == 0) {
            printf("\n--- Modifica Dipendente %s (%s %s) ---\n", personale[i].id, personale[i].nome, personale[i].cognome);
            printf("Nuovo Ruolo: ");
            fgets(personale[i].ruolo, MAX_CARATTERI, stdin);
            personale[i].ruolo[strcspn(personale[i].ruolo, "\n")] = 0;

            printf("Nuovo Contratto: ");
            fgets(personale[i].tipo_contratto, MAX_CARATTERI, stdin);
            personale[i].tipo_contratto[strcspn(personale[i].tipo_contratto, "\n")] = 0;

            printf("Nuova Retribuzione mensile: ");
            scanf("%f", &personale[i].retribuzione_mensile);
            pulisciBuffer();

            printf("Nuovo Stato (ATTIVO / FERIE / CESSATO): ");
            fgets(personale[i].stato, MAX_CARATTERI, stdin);
            personale[i].stato[strcspn(personale[i].stato, "\n")] = 0;
            return 0;
        }
    }
    return -1;
}

int eliminaPersonale(t_Personale personale[], int *nPersonale, const char *id) {
    for (int i = 0; i < *nPersonale; i++) {
        if (strcmp(personale[i].id, id) == 0) {
            for (int j = i; j < *nPersonale - 1; j++) {
                personale[j] = personale[j + 1];
            }
            (*nPersonale)--;
            return 0;
        }
    }
    return -1;
}

void visualizzaPersonale(t_Personale personale[], int nPersonale) {
    printf("\n============================= ORGANICO PERSONALE =============================\n");
    if (nPersonale == 0) {
        printf("Nessun dipendente registrato.\n");
        return;
    }
    for (int i = 0; i < nPersonale; i++) {
        printf("[%s] %s %s | Ruolo: %s | Contratto: %s | Stipendio: %.2f Euro | Assunto il: ",
               personale[i].id, personale[i].nome, personale[i].cognome,
               personale[i].ruolo, personale[i].tipo_contratto, personale[i].retribuzione_mensile);
        stampaData(personale[i].data_assunzione);
        printf(" | Stato: %s\n", personale[i].stato);
    }
    printf("Totale Dipendenti: %d\n", nPersonale);
}

int cercaPersonalePerContratto(t_Personale personale[], int nPersonale, const char *contratto) {
    int trovati = 0;
    printf("\n--- Dipendenti con contratto '%s' ---\n", contratto);
    for (int i = 0; i < nPersonale; i++) {
        if (strstr(personale[i].tipo_contratto, contratto) != NULL) {
            printf("[%s] %s %s | Ruolo: %s | Contratto: %s | Stato: %s\n",
                   personale[i].id, personale[i].nome, personale[i].cognome,
                   personale[i].ruolo, personale[i].tipo_contratto, personale[i].stato);
            trovati++;
        }
    }
    if (trovati == 0) printf("Nessun dipendente con questo tipo di contratto.\n");
    return trovati;
}

// ---GESTIONE COMPONENTI & MAGAZZINO---
int inserisciComponente(t_Componente componenti[], int *nComponenti) {
    if (*nComponenti >= MAX_COMPONENTI) {
        printf("[ERRORE] Magazzino componenti pieno (%d).\n", MAX_COMPONENTI);
        return -1;
    }
    int idx = *nComponenti;
    sprintf(componenti[idx].id, "CMP%04d", idx + 1);

    printf("\n--- Registrazione Componente Magazzino (%s) ---\n", componenti[idx].id);
    printf("Nome Componente (es. Seta Blu notte, Bottone Madreperla 20mm): ");
    fgets(componenti[idx].nome, MAX_CARATTERI, stdin);
    componenti[idx].nome[strcspn(componenti[idx].nome, "\n")] = 0;

    printf("Categoria (TESSUTO / MINUTERIA / FODERA / ACCESSORIO / FILATO): ");
    fgets(componenti[idx].categoria, MAX_CARATTERI, stdin);
    componenti[idx].categoria[strcspn(componenti[idx].categoria, "\n")] = 0;

    printf("Quantita disponibile: ");
    scanf("%f", &componenti[idx].quantita_disponibile);
    pulisciBuffer();

    printf("Unita di misura (m, cm, pezzi, rocchetti): ");
    fgets(componenti[idx].unita_misura, MAX_CARATTERI, stdin);
    componenti[idx].unita_misura[strcspn(componenti[idx].unita_misura, "\n")] = 0;

    printf("Costo Unitario (Euro): ");
    scanf("%f", &componenti[idx].costo_unitario);
    pulisciBuffer();

    printf("Soglia Minima di scorta: ");
    scanf("%f", &componenti[idx].soglia_minima);
    pulisciBuffer();

    printf("ID o P.IVA Fornitore: ");
    fgets(componenti[idx].id_fornitore, MAX_CARATTERI, stdin);
    componenti[idx].id_fornitore[strcspn(componenti[idx].id_fornitore, "\n")] = 0;

    // Collocazione fisica in magazzino (Piani)
    int piano;
    do {
        printf("Piano Magazzino (0 = Scorte Pesanti, 1 = Tessuti/Rotoli, 2 = Minuteria/Accessori): ");
        if (scanf("%d", &piano) == 1 && piano >= 0 && piano < NUMERO_PIANI) break;
        pulisciBuffer();
        printf("Piano non valido (0-%d). Riprova!\n", NUMERO_PIANI - 1);
    } while (1);
    pulisciBuffer();
    componenti[idx].piano_magazzino = piano;

    printf("Codice Scaffale/Posizione (es. A-12, B-04, C-01): ");
    fgets(componenti[idx].scaffale, MAX_CARATTERI, stdin);
    componenti[idx].scaffale[strcspn(componenti[idx].scaffale, "\n")] = 0;

    (*nComponenti)++;
    printf("[OK] Componente %s salvato nel Magazzino (Piano %d, Scaffale %s)!\n",
           componenti[idx].id, componenti[idx].piano_magazzino, componenti[idx].scaffale);
    return 0;
}

int modificaComponente(t_Componente componenti[], int nComponenti, const char *id) {
    for (int i = 0; i < nComponenti; i++) {
        if (strcmp(componenti[i].id, id) == 0) {
            printf("\n--- Modifica Componente %s (%s) ---\n", componenti[i].id, componenti[i].nome);
            printf("Nuova Quantita Disponibile: ");
            scanf("%f", &componenti[i].quantita_disponibile);
            pulisciBuffer();

            printf("Nuovo Costo Unitario: ");
            scanf("%f", &componenti[i].costo_unitario);
            pulisciBuffer();

            printf("Nuovo Piano (0-%d): ", NUMERO_PIANI - 1);
            scanf("%d", &componenti[i].piano_magazzino);
            pulisciBuffer();

            printf("Nuovo Scaffale: ");
            fgets(componenti[i].scaffale, MAX_CARATTERI, stdin);
            componenti[i].scaffale[strcspn(componenti[i].scaffale, "\n")] = 0;
            return 0;
        }
    }
    return -1;
}

int eliminaComponente(t_Componente componenti[], int *nComponenti, const char *id) {
    for (int i = 0; i < *nComponenti; i++) {
        if (strcmp(componenti[i].id, id) == 0) {
            for (int j = i; j < *nComponenti - 1; j++) {
                componenti[j] = componenti[j + 1];
            }
            (*nComponenti)--;
            return 0;
        }
    }
    return -1;
}

void visualizzaComponenti(t_Componente componenti[], int nComponenti) {
    printf("\n============================= INVENTARIO COMPONENTI =============================\n");
    if (nComponenti == 0) {
        printf("Nessun componente in magazzino.\n");
        return;
    }
    for (int i = 0; i < nComponenti; i++) {
        printf("[%s] %-25s | Cat: %-10s | Giacenza: %7.2f %-4s | Costo: %6.2f EUR | Piano: %d Scaffale: %s\n",
               componenti[i].id, componenti[i].nome, componenti[i].categoria,
               componenti[i].quantita_disponibile, componenti[i].unita_misura,
               componenti[i].costo_unitario, componenti[i].piano_magazzino, componenti[i].scaffale);
    }
    printf("Totale Componenti Registrati: %d\n", nComponenti);
}

int cercaComponente(t_Componente componenti[], int nComponenti, const char *termine) {
    int trovati = 0;
    printf("\n--- Ricerca componente '%s' ---\n", termine);
    for (int i = 0; i < nComponenti; i++) {
        if (strstr(componenti[i].nome, termine) != NULL ||
            strstr(componenti[i].categoria, termine) != NULL ||
            strstr(componenti[i].id, termine) != NULL) {
            printf("[%s] %s | Categoria: %s | Qta: %.2f %s | Collocazione: Piano %d, Scaffale %s\n",
                   componenti[i].id, componenti[i].nome, componenti[i].categoria,
                   componenti[i].quantita_disponibile, componenti[i].unita_misura,
                   componenti[i].piano_magazzino, componenti[i].scaffale);
            trovati++;
        }
    }
    if (trovati == 0) printf("Nessun componente trovato.\n");
    return trovati;
}

void visualizzaMagazzinoPerPiano(t_Componente componenti[], int nComponenti, int piano) {
    const char *descPiano = (piano == 0) ? "PIANO 0: Ricevimento & Scorte" :
                            (piano == 1) ? "PIANO 1: Tessuti Pregiati & Rotoli" :
                            (piano == 2) ? "PIANO 2: Minuteria, Filati & Accessori" : "PIANO GENERICO";

    printf("\n---------------------------------------------------------------------------\n");
    printf(" %s\n", descPiano);
    printf("---------------------------------------------------------------------------\n");

    int count = 0;
    float valorePiano = 0;
    for (int i = 0; i < nComponenti; i++) {
        if (componenti[i].piano_magazzino == piano) {
            printf("  [%s] %-24s | Q.ta: %6.2f %-4s | Scaffale: %-5s | Valore: %7.2f EUR\n",
                   componenti[i].id, componenti[i].nome,
                   componenti[i].quantita_disponibile, componenti[i].unita_misura,
                   componenti[i].scaffale, componenti[i].quantita_disponibile * componenti[i].costo_unitario);
            valorePiano += (componenti[i].quantita_disponibile * componenti[i].costo_unitario);
            count++;
        }
    }
    if (count == 0) {
        printf("  (Nessun elemento stoccato a questo piano)\n");
    } else {
        printf("  >> Articoli al piano: %d | Valore complessivo piano: %.2f Euro\n", count, valorePiano);
    }
}

void controlloSottoscorta(t_Componente componenti[], int nComponenti) {
    int allarmi = 0;
    printf("\n--- VERIFICA SOTTO-SCORTA MAGAZZINO ---\n");
    for (int i = 0; i < nComponenti; i++) {
        if (componenti[i].quantita_disponibile <= componenti[i].soglia_minima) {
            printf("[ATTENZIONE] Componente %s (%s): Qta attuale %.2f %s <= Soglia Minima %.2f %s! (Piano %d, Scaffale %s)\n",
                   componenti[i].id, componenti[i].nome, componenti[i].quantita_disponibile,
                   componenti[i].unita_misura, componenti[i].soglia_minima, componenti[i].unita_misura,
                   componenti[i].piano_magazzino, componenti[i].scaffale);
            allarmi++;
        }
    }
    if (allarmi == 0) {
        printf("[OK] Tutte le scorte sono sopra la soglia minima di sicurezza.\n");
    } else {
        printf("Totale componenti in esaurimento/da riordinare: %d\n", allarmi);
    }
}

// ---GESTIONE PROGETTI (Abiti)---
int inserisciProgetto(t_Progetto progetti[], int *nProgetti, t_Cliente clienti[], int nClienti,
                      t_Personale personale[], int nPersonale, t_Componente componenti[], int nComponenti) {
    if (*nProgetti >= MAX_PROGETTI) {
        printf("[ERRORE] Limite massimo progetti raggiunto (%d).\n", MAX_PROGETTI);
        return -1;
    }
    if (nClienti == 0) {
        printf("[AVVISO] Nessun cliente registrato. Registrare prima almeno un cliente committente.\n");
        return -1;
    }

    int idx = *nProgetti;
    sprintf(progetti[idx].id, "PRJ%04d", idx + 1);

    printf("\n--- Apertura Nuovo Progetto / Abito (%s) ---\n", progetti[idx].id);

    // Selezione Cliente
    visualizzaClienti(clienti, nClienti);
    printf("Inserisci ID o CF del Cliente committente: ");
    char refCliente[MAX_CARATTERI];
    scanf("%49s", refCliente);
    pulisciBuffer();
    strcpy(progetti[idx].id_cliente, refCliente);

    printf("Tipo di Capo / Abito (es. Abito Tre Pezzi Sartoriale, Tubino Seta, Camicia Lino): ");
    fgets(progetti[idx].tipo_capo, MAX_CARATTERI, stdin);
    progetti[idx].tipo_capo[strcspn(progetti[idx].tipo_capo, "\n")] = 0;

    // Assegnazione Sarto / Personale
    if (nPersonale > 0) {
        printf("\nPersonale disponibile per l'assegnazione:\n");
        for (int p = 0; p < nPersonale; p++) {
            printf(" - [%s] %s %s (%s)\n", personale[p].id, personale[p].nome, personale[p].cognome, personale[p].ruolo);
        }
        printf("ID Sarto assegnato al progetto: ");
        scanf("%49s", progetti[idx].id_sarto_assegnato);
        pulisciBuffer();
    } else {
        strcpy(progetti[idx].id_sarto_assegnato, "NON_ASSEGNATO");
    }

    // Date
    while (1) {
        printf("Data Inizio Lavoro (GG MM AAAA): ");
        int ok = scanf("%d %d %d", &progetti[idx].data_inizio.giorno,
                       &progetti[idx].data_inizio.mese,
                       &progetti[idx].data_inizio.anno);
        pulisciBuffer();
        if (ok == 3 && controlloData(progetti[idx].data_inizio)) break;
        printf("Data non valida. Riprova!\n");
    }

    while (1) {
        printf("Data Consegna Prevista (GG MM AAAA): ");
        int ok = scanf("%d %d %d", &progetti[idx].data_consegna_prevista.giorno,
                       &progetti[idx].data_consegna_prevista.mese,
                       &progetti[idx].data_consegna_prevista.anno);
        pulisciBuffer();
        if (ok == 3 && controlloData(progetti[idx].data_consegna_prevista)) break;
        printf("Data non valida. Riprova!\n");
    }

    // Componenti impiegati e scarico dal magazzino
    progetti[idx].num_componenti = 0;
    float costoMateriali = 0;

    if (nComponenti > 0) {
        char risposta;
        printf("\nVuoi associare e prelevare materiali/componenti dal magazzino per questo abito? (s/n): ");
        scanf(" %c", &risposta);
        pulisciBuffer();

        if (risposta == 's' || risposta == 'S') {
            visualizzaComponenti(componenti, nComponenti);
            int quantiComp;
            printf("Quanti diversi componenti vuoi impiegare per questo abito (max %d)? ", MAX_COMP_PER_PROGETTO);
            if (scanf("%d", &quantiComp) == 1 && quantiComp > 0) {
                pulisciBuffer();
                if (quantiComp > MAX_COMP_PER_PROGETTO) quantiComp = MAX_COMP_PER_PROGETTO;

                for (int c = 0; c < quantiComp; c++) {
                    char idCmp[MAX_CARATTERI];
                    float qtaUso;
                    printf("[%d/%d] Inserisci ID Componente da usare: ", c + 1, quantiComp);
                    scanf("%49s", idCmp);
                    pulisciBuffer();

                    int cmpIndex = -1;
                    for (int k = 0; k < nComponenti; k++) {
                        if (strcmp(componenti[k].id, idCmp) == 0) {
                            cmpIndex = k;
                            break;
                        }
                    }

                    if (cmpIndex != -1) {
                        printf("Quantita da prelevare (%s disponibili: %.2f): ",
                               componenti[cmpIndex].unita_misura, componenti[cmpIndex].quantita_disponibile);
                        scanf("%f", &qtaUso);
                        pulisciBuffer();

                        if (qtaUso > componenti[cmpIndex].quantita_disponibile) {
                            printf("[AVVISO] Giacenza insufficiente (disponibili solo %.2f). Prelevato il massimo disponibile.\n",
                                   componenti[cmpIndex].quantita_disponibile);
                            qtaUso = componenti[cmpIndex].quantita_disponibile;
                        }

                        // Aggiorna giacenza magazzino
                        componenti[cmpIndex].quantita_disponibile -= qtaUso;

                        // Salva nel progetto
                        int cIdx = progetti[idx].num_componenti;
                        strcpy(progetti[idx].componenti[cIdx].id_componente, idCmp);
                        progetti[idx].componenti[cIdx].quantita_usata = qtaUso;
                        progetti[idx].num_componenti++;

                        costoMateriali += (qtaUso * componenti[cmpIndex].costo_unitario);
                        printf("[OK] Prelevati %.2f %s di %s (Nuova giacenza: %.2f)\n",
                               qtaUso, componenti[cmpIndex].unita_misura, componenti[cmpIndex].nome,
                               componenti[cmpIndex].quantita_disponibile);
                    } else {
                        printf("[ERRORE] Componente non trovato nel catalogo.\n");
                    }
                }
            }
        }
    }

    printf("\nCosto stimato materiali: %.2f Euro\n", costoMateriali);
    printf("Inserisci Prezzo Finale / Preventivo al cliente (Euro): ");
    scanf("%f", &progetti[idx].prezzo_finale);
    pulisciBuffer();

    strcpy(progetti[idx].stato_avanzamento, "IN_LAVORAZIONE");
    (*nProgetti)++;
    printf("[OK] Progetto Abito %s creato con successo!\n", progetti[idx].id);
    return 0;
}

int modificaStatoProgetto(t_Progetto progetti[], int nProgetti, const char *id) {
    for (int i = 0; i < nProgetti; i++) {
        if (strcmp(progetti[i].id, id) == 0) {
            printf("\nStato attuale progetto %s: %s\n", progetti[i].id, progetti[i].stato_avanzamento);
            printf("Seleziona nuovo stato:\n");
            printf("1. IN ATTESA\n");
            printf("2. IN LAVORAZIONE\n");
            printf("3. PRIMA PROVA\n");
            printf("4. COMPLETATO\n");
            printf("5. CONSEGNATO\n");
            printf("Scelta: ");
            int s;
            if (scanf("%d", &s) == 1) {
                switch (s) {
                    case 1: strcpy(progetti[i].stato_avanzamento, "IN ATTESA"); break;
                    case 2: strcpy(progetti[i].stato_avanzamento, "IN LAVORAZIONE"); break;
                    case 3: strcpy(progetti[i].stato_avanzamento, "PRIMA PROVA"); break;
                    case 4: strcpy(progetti[i].stato_avanzamento, "COMPLETATO"); break;
                    case 5: strcpy(progetti[i].stato_avanzamento, "CONSEGNATO"); break;
                    default: printf("Scelta non valida.\n"); break;
                }
                printf("Stato aggiornato a: %s\n", progetti[i].stato_avanzamento);
            }
            pulisciBuffer();
            return 0;
        }
    }
    printf("Progetto %s non trovato.\n", id);
    return -1;
}

int eliminaProgetto(t_Progetto progetti[], int *nProgetti, const char *id) {
    for (int i = 0; i < *nProgetti; i++) {
        if (strcmp(progetti[i].id, id) == 0) {
            for (int j = i; j < *nProgetti - 1; j++) {
                progetti[j] = progetti[j + 1];
            }
            (*nProgetti)--;
            return 0;
        }
    }
    return -1;
}

void visualizzaProgetti(t_Progetto progetti[], int nProgetti) {
    printf("\n=============================== ELENCO PROGETTI (ABITI) ===============================\n");
    if (nProgetti == 0) {
        printf("Nessun progetto attivo.\n");
        return;
    }
    for (int i = 0; i < nProgetti; i++) {
        printf("[%s] Capo: %-25s | Cliente: %-10s | Sarto: %-10s | Stato: %-14s | Prezzo: %.2f EUR\n",
               progetti[i].id, progetti[i].tipo_capo, progetti[i].id_cliente,
               progetti[i].id_sarto_assegnato, progetti[i].stato_avanzamento, progetti[i].prezzo_finale);
        printf("     Consegna prevista: ");
        stampaData(progetti[i].data_consegna_prevista);
        printf(" | Componenti utilizzati: %d\n", progetti[i].num_componenti);
        for (int c = 0; c < progetti[i].num_componenti; c++) {
            printf("       -> ID Comp: %s | Qta usata: %.2f\n",
                   progetti[i].componenti[c].id_componente, progetti[i].componenti[c].quantita_usata);
        }
    }
    printf("Totale Progetti: %d\n", nProgetti);
}

int cercaProgetto(t_Progetto progetti[], int nProgetti, const char *id) {
    for (int i = 0; i < nProgetti; i++) {
        if (strcmp(progetti[i].id, id) == 0) {
            printf("\n--- Scheda Progetto %s ---\n", progetti[i].id);
            printf("Capo: %s\n", progetti[i].tipo_capo);
            printf("Committente (Cliente): %s\n", progetti[i].id_cliente);
            printf("Sarto Responsabile: %s\n", progetti[i].id_sarto_assegnato);
            printf("Stato: %s\n", progetti[i].stato_avanzamento);
            printf("Inizio: "); stampaData(progetti[i].data_inizio); printf("\n");
            printf("Consegna Prevista: "); stampaData(progetti[i].data_consegna_prevista); printf("\n");
            printf("Prezzo: %.2f Euro\n", progetti[i].prezzo_finale);
            printf("Componenti impiegati: %d\n", progetti[i].num_componenti);
            for (int c = 0; c < progetti[i].num_componenti; c++) {
                printf(" - %s (quantita: %.2f)\n", progetti[i].componenti[c].id_componente, progetti[i].componenti[c].quantita_usata);
            }
            return 1;
        }
    }
    printf("Nessun progetto trovato con ID %s.\n", id);
    return 0;
}

// ---PERSISTENZA BINARIA V2---
int SalvaTuttoSuFileV2(t_Fornitore fornitori[], int nFornitori,
                      t_Cliente clienti[], int nClienti,
                      t_Personale personale[], int nPersonale,
                      t_Componente componenti[], int nComponenti,
                      t_Progetto progetti[], int nProgetti) {
    FILE *f = fopen(FN_BACKUP_V2, "wb");
    if (f == NULL) return -1;

    // Fornitori
    fwrite(&nFornitori, sizeof(int), 1, f);
    fwrite(fornitori, sizeof(t_Fornitore), nFornitori, f);

    // Clienti
    fwrite(&nClienti, sizeof(int), 1, f);
    fwrite(clienti, sizeof(t_Cliente), nClienti, f);

    // Personale
    fwrite(&nPersonale, sizeof(int), 1, f);
    fwrite(personale, sizeof(t_Personale), nPersonale, f);

    // Componenti
    fwrite(&nComponenti, sizeof(int), 1, f);
    fwrite(componenti, sizeof(t_Componente), nComponenti, f);

    // Progetti
    fwrite(&nProgetti, sizeof(int), 1, f);
    fwrite(progetti, sizeof(t_Progetto), nProgetti, f);

    fclose(f);
    return 0;
}

int CaricaTuttoDaFileV2(t_Fornitore fornitori[], int *nFornitori,
                       t_Cliente clienti[], int *nClienti,
                       t_Personale personale[], int *nPersonale,
                       t_Componente componenti[], int *nComponenti,
                       t_Progetto progetti[], int *nProgetti) {
    FILE *f = fopen(FN_BACKUP_V2, "rb");
    if (f == NULL) {
        *nFornitori = 0;
        *nClienti = 0;
        *nPersonale = 0;
        *nComponenti = 0;
        *nProgetti = 0;
        return -1;
    }

    fread(nFornitori, sizeof(int), 1, f);
    fread(fornitori, sizeof(t_Fornitore), *nFornitori, f);

    fread(nClienti, sizeof(int), 1, f);
    fread(clienti, sizeof(t_Cliente), *nClienti, f);

    fread(nPersonale, sizeof(int), 1, f);
    fread(personale, sizeof(t_Personale), *nPersonale, f);

    fread(nComponenti, sizeof(int), 1, f);
    fread(componenti, sizeof(t_Componente), *nComponenti, f);

    fread(nProgetti, sizeof(int), 1, f);
    fread(progetti, sizeof(t_Progetto), *nProgetti, f);

    fclose(f);
    return 0;
}

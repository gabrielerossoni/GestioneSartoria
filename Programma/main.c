// Gabriele Rossoni e Marcello Patrini - 4IB
// LINK PROGETTO GITHUB: https://github.com/gabrielerossoni/GestioneSartoria

// ---LIBRERIE---
#include <stdio.h>      // Libreria standard per input/output (printf, scanf, fopen, fclose, ecc.)
#include <stdlib.h>     // Libreria standard per funzioni di utilità (malloc, free, exit, ecc.)
#include <string.h>     // Libreria per manipolazione stringhe (strcmp, strcpy, strlen, strcspn, ecc.)
#include <ctype.h>      // Libreria per funzioni di classificazione caratteri (isdigit, isalpha, toupper, ecc.)
#include <time.h>       // Libreria per gestione data e ora (time, localtime, ecc)


// ---COSTANTI---
#define MAX_CARATTERI 50                // Lunghezza massima per stringhe generiche (ID, nomi, colori, ecc.)
#define MAX_ROTOLI 500                  // Numero massimo di rotoli gestibili nel sistema
#define MAX_PROGETTI 500                // Numero massimo di progetti gestibili nel sistema
#define MAX_FORNITORI 200               // Numero massimo di fornitori gestibili nel sistema
#define MAX_PRELIEVI 1000               // Numero massimo di prelievi registrabili nel sistema
#define MAX_RITAGLI 1000                // Numero massimo di ritagli gestibili nel sistema
#define MAX_CHAR_NOTEAGGIUNTIVE 100     // Lunghezza massima per il campo note aggiuntive dei rotoli


//---RITAGLIO---
#define SOGLIA_RITAGLIO 0.5

// ---DATA---
#define ANNO_MIN 1900
#define ANNO_MAX 2100

// ---NOMI FILE---
#define FNCOMPLETO "../File/backup_sartoria.dat"

// ---STRUTTURE DATI---
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

// ---PROTOTIPI FUNZIONI---
// Funzioni di Menu
int menuGenerale();
int menuRotoli();
int menuProgetti();
int menuFornitori();
int menuPrelievi();
int menuRitagli();
int menuMagazzino();

// Funzioni di Data
int controlloData(t_Data);

// Funzioni di gestione ROTOLO
int inserisciRotolo(t_Rotolo[], int *);
int modificaRotolo(t_Rotolo[], int, char[]);
int eliminaRotolo(t_Rotolo[], int *, char[]);
int visualizzaRotolo(t_Rotolo[], int);
int cercaRotolo(t_Rotolo[], int);
int menuCercaRotoli();

// Funzioni di gestione RITAGLIO
void visualizzaRitagli(t_Ritaglio[], int);
int cercaRitaglio(t_Ritaglio[], int);
int creaRitaglioAutomatico(t_Ritaglio[], int *, t_Rotolo *);
int menuCercaRitagli();

// Funzioni di gestione PRELIEVO
int eseguiPrelievo(t_Prelievo[], int *, t_Rotolo[], int, t_Ritaglio[], int *);
int visualizzaPrelievo(t_Prelievo[], int);
int cercaPrelievo(t_Prelievo[], int);
int menuCercaPrelievi();

// Funzioni di gestione FORNITORE
int inserisciFornitore(t_Fornitore[], int *);
int modificaFornitore(t_Fornitore[], int, char[]);
int eliminaFornitore(t_Fornitore[], int *, char[]);
int visualizzaFornitore(t_Fornitore[], int);
int cercaFornitore(t_Fornitore[], int, char *);

// Funzioni di gestione PROGETTO
int inserisciProgetto(t_Progetto[], int *);
int modificaProgetto(t_Progetto[], int, char[]);
int eliminaProgetto(t_Progetto[], int *, char[]);
int visualizzaProgetto(t_Progetto[], int);
int cercaProgetto(t_Progetto[], int, char *);

// Funzioni di gestione MAGAZZINO
int inserisciRitaglio(t_Ritaglio[], int);
int modificaRitaglio(t_Ritaglio[], int, int);
int eliminaRitaglio(t_Ritaglio[], int *);
int cercaRitaglio(t_Ritaglio[], int);
void controlloMagazzino(t_Rotolo[], int);
void visualizzaMagazzino(t_Rotolo[], int);

// Funzione salvataggio unico di tutto il programma
int SalvaTuttoSuFile(t_Rotolo[], int, t_Progetto[], int, t_Fornitore[], int, t_Prelievo[], int, t_Ritaglio[], int);
int CaricaTuttoDaFile(t_Rotolo[], int *, t_Progetto[], int *, t_Fornitore[], int *, t_Prelievo[], int *, t_Ritaglio[], int *);

// ---MAIN---
int main(){
    t_Rotolo rotoli[MAX_ROTOLI];
    t_Progetto progetti[MAX_PROGETTI];
    t_Fornitore fornitori[MAX_FORNITORI];
    t_Prelievo prelievi[MAX_PRELIEVI];
    t_Ritaglio ritagli[MAX_RITAGLI];
    int nRotoli = 0, nProgetti = 0, nFornitori = 0, nPrelievi = 0, nRitagli = 0, scelta, flag, quit = 0, scelta_sub, ris;
    char id[MAX_CARATTERI], ricerca_partita_iva[MAX_CARATTERI], nome[MAX_CARATTERI];

    // MENU PRINCIPALE
    // Caricamento dati da file all'avvio del programma

    ris = CaricaTuttoDaFile(rotoli, &nRotoli, progetti, &nProgetti, fornitori, &nFornitori, prelievi, &nPrelievi, ritagli, &nRitagli);
    if (ris == 0){
        printf("DATI CARICATI: %d rotoli, %d progetti, %d fornitori, %d prelievi, %d ritagli\n",
               nRotoli, nProgetti, nFornitori, nPrelievi, nRitagli);
    }else{
        printf("NESSUN FILE DI BACKUP TROVATO. Inizio con dati vuoti.\n");
    }

    do{
        scelta = menuGenerale();
        switch (scelta){
        case 1: // ROTOLO
            do
            {
                scelta_sub = menuRotoli();
                switch (scelta_sub){
                case 1: // AGGIUNGI ROTOLO
                    inserisciRotolo(rotoli, &nRotoli);
                    break;
                case 2: // MODIFICA ROTOLO
                    printf("INSERISCI L'ID DEL ROTOLO DA MODIFICARE: ");
                    scanf("%s", id);
                    flag=modificaRotolo(rotoli, nRotoli, id);
                    if(flag==0)
                        printf("ROTOLO MODIFICATO CON SUCCESSO.\n");
                    else
                        printf("ROTOLO NON TROVATO.\n");
                    break;
                case 3: // ELIMINA ROTOLO
                    printf("INSERISCI L'ID DEL ROTOLO DA ELIMINARE: ");
                    scanf("%s", id);
                    if (eliminaRotolo(rotoli, &nRotoli, id) == 1)
                        printf("ROTOLO ELIMINATO CON SUCCESSO.\n");
                    else
                        printf("ROTOLO NON TROVATO.\n");
                    break;
                case 4: // VISUALIZZA ROTOLO
                    visualizzaRotolo(rotoli, nRotoli);
                    break;
                case 5: // CERCA ROTOLO
                    cercaRotolo(rotoli, nRotoli);
                    break;
                case 6: // TORNA AL MENU PRINCIPALE
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 6);
            break;
        case 2: // RITAGLI
            do
            {
                scelta_sub = menuRitagli();
                switch (scelta_sub)
                {
                case 1:
                    visualizzaRitagli(ritagli, nRitagli);
                    break;
                case 2:
                    cercaRitaglio(ritagli, nRitagli);
                    break;
                case 3: // TORNA AL MENU PRINCIPALE
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 3);
            break;
        case 3: // PRELIEVI
            do
            {
                scelta_sub = menuPrelievi();
                switch (scelta_sub)
                {
                case 1:
                    eseguiPrelievo(prelievi, &nPrelievi, rotoli, nRotoli, ritagli, &nRitagli);
                    break;
                case 2:
                    cercaPrelievo(prelievi, nPrelievi);
                    break;
                case 3:
                    visualizzaPrelievo(prelievi, nPrelievi);
                    break;
                case 4: // TORNA AL MENU PRINCIPALE
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 4);
            break;
        case 4: // PROGETTI
            do
            {
                scelta_sub = menuProgetti();
                switch (scelta_sub)
                {
                case 1:
                    inserisciProgetto(progetti, &nProgetti);
                    break;
                case 2:
                    printf("INSERISCI L'ID DEL PROGETTO DA MODIFICARE: ");
                    scanf("%s", id);
                    modificaProgetto(progetti, nProgetti, id);
                    break;
                case 3:
                    printf("INSERISCI L'ID DEL PROGETTO DA ELIMINARE: ");
                    scanf("%s", id);
                    flag=eliminaProgetto(progetti, &nProgetti, id);
                    if (flag == 1)
                        printf("PROGETTO ELIMINATO CON SUCCESSO.\n");
                    else
                        printf("PROGETTO NON TROVATO.\n");
                    break;
                case 4:
                    visualizzaProgetto(progetti, nProgetti);
                    break;
                case 5:
                    printf("INSERISCI L'ID DEL PROGETTO DA CERCARE: ");
                    scanf("%s", id);
                    flag = cercaProgetto(progetti, nProgetti, id);
                    if (flag == 0)
                        printf("PROGETTO NON TROVATO.\n");
                    break;
                case 6: // TORNA AL MENU PRINCIPALE
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 6);
            break;
        case 5: // MAGAZZINO
            do
            {
                scelta_sub = menuMagazzino();
                switch (scelta_sub)
                {
                case 1: // CONTROLLO MAGAZZINO (valore/metraggio/numero rotoli)
                    controlloMagazzino(rotoli, nRotoli);
                    break;
                case 2: // VISUALIZZA MAGAZZINO
                    visualizzaMagazzino(rotoli, nRotoli);
                    break;
                case 3:
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 3);
            break;
        case 6: // FORNITORI
            do
            {
                scelta_sub = menuFornitori();
                switch (scelta_sub)
                {
                case 1:
                    inserisciFornitore(fornitori, &nFornitori);
                    break;
                case 2:
                    printf("INSERIRE NOME DEL FORNITORE DA MODIFICARE: ");
                    scanf("%s", nome);
                    flag=modificaFornitore(fornitori, nFornitori, nome);
                    if(flag==0)
                        printf("FORNITORE MODIFICATO CON SUCCESSO.\n");
                    else
                        printf("FORNITORE NON TROVATO.\n");
                    break;
                case 3:
                    printf("INSERIRE NOME DEL FORNITORE DA ELIMINARE: ");
                    scanf("%s", nome);
                    flag=eliminaFornitore(fornitori, &nFornitori, nome);
                    if (flag == 1)
                        printf("FORNITORE ELIMINATO CON SUCCESSO.\n");
                    else
                        printf("FORNITORE NON TROVATO.\n");
                    break;
                case 4:
                    visualizzaFornitore(fornitori, nFornitori);
                    break;
                case 5:
                    printf("INSERISCI PARTITA IVA: ");
                    scanf("%s", ricerca_partita_iva);
                    cercaFornitore(fornitori, nFornitori, ricerca_partita_iva);
                    break;
                case 6: // TORNA AL MENU PRINCIPALE
                    break;
                default:
                    printf("OPZIONE NON VALIDA. Riprova.\n");
                    break;
                }
            } while (scelta_sub != 6);
            break;
        case 7: // SALVA E TERMINA PROGRAMMA
            flag=SalvaTuttoSuFile(rotoli, nRotoli, progetti, nProgetti, fornitori, nFornitori, prelievi, nPrelievi, ritagli, nRitagli);
            if (flag == 0){
                printf("SALVATAGGIO COMPLETATO. USCITA PROGRAMMA.\n");
                quit = 1;
            }else{
                printf("ERRORE NEL SALVATAGGIO. Riprovare.\n");
            }
            break;
        default:
            printf("OPZIONE NON VALIDA. Riprova.\n");
            break;
        }
    } while (!quit);
    return 0;
}

//---FUNZIONI---
int menuGenerale(){
    int scelta;
    printf("\n--- MENU GENERALE SARTORIA ---\n");
    printf("1. ROTOLI\n");
    printf("2. RITAGLI\n");
    printf("3. PRELIEVI\n");
    printf("4. PROGETTI\n");
    printf("5. MAGAZZINO\n");
    printf("6. FORNITORI\n");
    printf("7. SALVATAGGIO E TERMINAZIONE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int menuRotoli(){
    int scelta;
    printf("\n--- MENU ROTOLO ---\n");
    printf("1. AGGIUNGI ROTOLO\n");
    printf("2. MODIFICA ROTOLO\n");
    printf("3. ELIMINA ROTOLO\n");
    printf("4. VISUALIZZA ROTOLO\n");
    printf("5. CERCA ROTOLO PER CATEGORIA\n");
    printf("6. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int inserisciRotolo(t_Rotolo rotoli[], int *nRotoli){
    int i, nuovi, idx, ok;
    printf("NUMERO ROTOLI DA AGGIUNGERE: ");
    if (scanf("%d", &nuovi) != 1 || nuovi < 1){
        printf("INPUT NON VALIDO.\n");
        return -1;
    }

    if (*nRotoli + nuovi > MAX_ROTOLI){
        printf("ERRORE: superato limite massimo (%d rotoli).\n", MAX_ROTOLI);
        return -1;
    }

    while (getchar() != '\n');
    for (i = 0; i < nuovi; i++){
        idx = *nRotoli + i;
        printf("\n--- Rotolo %d di %d ---\n", i + 1, nuovi);
        sprintf(rotoli[idx].id, "R%04d", *nRotoli + i + 1);
        printf("ID: %s (generato automaticamente)\n", rotoli[idx].id);

        printf("TIPO: ");
        fgets(rotoli[idx].tipo, 50, stdin);
        rotoli[idx].tipo[strcspn(rotoli[idx].tipo, "\n")] = 0;

        printf("COLORE: ");
        fgets(rotoli[idx].colore, 50, stdin);
        rotoli[idx].colore[strcspn(rotoli[idx].colore, "\n")] = 0;

        printf("FANTASIA: ");
        fgets(rotoli[idx].fantasia, MAX_CARATTERI, stdin);
        rotoli[idx].fantasia[strcspn(rotoli[idx].fantasia, "\n")] = 0; // RIMUOVI \n

        printf("LUNGHEZZA TOTALE (m): ");
        scanf("%f", &rotoli[idx].lunghezza_totale);
        rotoli[idx].lunghezza_attuale = rotoli[idx].lunghezza_totale * 100;

        printf("LUNGHEZZA ATTUALE: %.2f cm (calcolato automaticamente)\n", rotoli[idx].lunghezza_attuale);
        printf("COSTO AL METRO: ");
        scanf("%f", &rotoli[idx].costo_metro);

        printf("FORNITORE: ");
        fgets(rotoli[idx].fornitore, MAX_CARATTERI, stdin);
        rotoli[idx].fornitore[strcspn(rotoli[idx].fornitore, "\n")] = 0; // RIMUOVI \n
        while (getchar() != '\n');
        printf("LOTTO: ");
        fgets(rotoli[idx].lotto, MAX_CARATTERI, stdin);
        rotoli[idx].lotto[strcspn(rotoli[idx].lotto, "\n")] = 0; // RIMUOVI \n
     
        while (1){
            printf("DATA (GG MM AAAA): ");
            ok = scanf("%d %d %d",
                       &rotoli[idx].data.giorno,
                       &rotoli[idx].data.mese,
                       &rotoli[idx].data.anno);
                    while (getchar() != '\n'); // svuota sempre

            if (ok == 3 && controlloData(rotoli[idx].data))
                break;

            printf("DATA NON VALIDA. Riprovare.\n");
        }
        strcpy(rotoli[idx].stato, "DISPONIBILE");
        strcpy(rotoli[idx].noteAggiuntive, "");
    }
    *nRotoli += nuovi;
    printf("\n%d rotoli aggiunti con successo.\n", nuovi);
    return 0;
}

int modificaRotolo(t_Rotolo rotoli[], int nRotoli, char *id){
    int i, flag= -1;
    while (getchar() != '\n');
    for (i = 0; i < nRotoli; i++){
        if (strcmp(rotoli[i].id, id) == 0){
            printf("MODIFICA I DATI DEL ROTOLO %s:\n", id);
            printf("TIPO: ");
            fgets(rotoli[i].tipo, 50, stdin);
            rotoli[i].tipo[strcspn(rotoli[i].tipo, "\n")] = 0;

            printf("COLORE: ");
            fgets(rotoli[i].colore, 50, stdin);
            rotoli[i].colore[strcspn(rotoli[i].colore, "\n")] = 0;

            printf("FANTASIA: ");
            fgets(rotoli[i].fantasia, MAX_CARATTERI, stdin);
            rotoli[i].fantasia[strcspn(rotoli[i].fantasia, "\n")] = 0;

            printf("LUNGHEZZA TOTALE (m): ");
            scanf("%f", &rotoli[i].lunghezza_totale);

            rotoli[i].lunghezza_attuale = rotoli[i].lunghezza_totale * 100;
            printf("LUNGHEZZA ATTUALE: %.2f cm (calcolato automaticamente)\n", rotoli[i].lunghezza_attuale);

            printf("COSTO AL METRO: ");
            scanf("%f", &rotoli[i].costo_metro);
            flag=0;
        }
    }
    return flag;
}

int eliminaRotolo(t_Rotolo rotoli[], int *nRotoli, char *id){
    int i, j;
    for (i = 0; i < *nRotoli; i++){
        if (strcmp(rotoli[i].id, id) == 0){
            for (j = i; j < *nRotoli - 1; j++){
                rotoli[j] = rotoli[j + 1];
            }
            (*nRotoli)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaRotolo(t_Rotolo rotoli[], int nRotoli){
    int i;
    printf("ELENCO ROTOLO:\n");
    for (i = 0; i < nRotoli; i++){
        printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA TOTALE: %.2f m, LUNGHEZZA ATTUALE: %.2f cm, COSTO AL METRO: %.2f, FORNITORE: %s\n",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
               rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale,
               rotoli[i].costo_metro, rotoli[i].fornitore);
    }
    return 0;
}

int menuCercaRotoli(){
    int scelta;
    printf("\n--- RICERCA ROTOLO ---\n");
    printf("1. Cerca per CODICE ROTOLO\n");
    printf("2. Cerca per STATO\n");
    printf("3. Cerca per DISPONIBILITA' MINIMA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int cercaRotolo(t_Rotolo rotoli[], int nRotoli){
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float disponibilitaMin;

    scelta = menuCercaRotoli();

    switch (scelta){
    case 1: // Cerca per ID ROTOLO
        printf("INSERISCI L'ID DEL ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++){
            if (strcmp(rotoli[i].id, ricerca) == 0){
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, LUNGHEZZA ATTUALE: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale, rotoli[i].costo_metro,
                       rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 2: // Cerca per STATO
        printf("INSERISCI STATO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++){
            if (strcmp(rotoli[i].stato, ricerca) == 0){
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, LUNGHEZZA ATTUALE: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale, rotoli[i].costo_metro,
                       rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 3: // Cerca per DISPONIBILITA' MINIMA
        printf("INSERISCI DISPONIBILITA' MINIMA (m): ");
        scanf("%f", &disponibilitaMin);
        for (i = 0; i < nRotoli; i++){
            if (rotoli[i].lunghezza_totale >= disponibilitaMin){
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, LUNGHEZZA ATTUALE: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale, rotoli[i].costo_metro,
                       rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    default:
        printf("OPZIONE NON VALIDA.\n");
        return -1;
    }
    if (trovati == 0)
        printf("NESSUN ROTOLO TROVATO.\n");
    else
        printf("TOTALE ROTOLI TROVATI: %d\n", trovati);
    return trovati;
}

int menuPrelievi(){
    int scelta;
    printf("\n--- MENU PRELIEVI ---\n");
    printf("1. ESEGUI PRELIEVO\n");
    printf("2. CERCA PRELIEVO\n");
    printf("3. VISUALIZZA PRELIEVI\n");
    printf("4. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int eseguiPrelievo(t_Prelievo prelievi[], int *nPrelievi, t_Rotolo rotoli[], int nRotoli, t_Ritaglio ritagli[], int *nRitagli){
    int i, j, nuovi, idx, rotoloTrovato;
    float metraggioCm;
    printf("NUMERO PRELIEVI DA AGGIUNGERE: ");
    if (scanf("%d", &nuovi) != 1 || nuovi < 1){
        printf("INPUT NON VALIDO.\n");
        return -1;
    }
    if (*nPrelievi + nuovi > MAX_PRELIEVI){
        printf("ERRORE: superato limite massimo prelievi.\n");
        return -1;
    }
    for (i = 0; i < nuovi; i++){
        idx = *nPrelievi + i;
        printf("\n--- Prelievo %d di %d ---\n", i + 1, nuovi);
        sprintf(prelievi[idx].id, "P%04d", *nPrelievi + i + 1);
        printf("ID PRELIEVO: %s (auto)\n", prelievi[idx].id);
        printf("ID ROTOLO: ");
        scanf("%49s", prelievi[idx].id_rotolo);
        // Verifica che il rotolo esista
        rotoloTrovato = -1;
        for (j = 0; j < nRotoli; j++){
            if (strcmp(rotoli[j].id, prelievi[idx].id_rotolo) == 0){
                rotoloTrovato = j;
                break;
            }
        }

        if (rotoloTrovato == -1){
            printf("ROTOLO NON TROVATO. Riprovare.\n");
            i--;
            continue;
        }

        printf("METRAGGIO PRELEVATO (m): ");
        scanf("%f", &prelievi[idx].metraggio_prelevato);
        metraggioCm = prelievi[idx].metraggio_prelevato * 100;
        if (metraggioCm > rotoli[rotoloTrovato].lunghezza_attuale){
            printf("ERRORE: metraggio insufficiente (disponibile: %.2f cm).\n", rotoli[rotoloTrovato].lunghezza_attuale);
            i--;
            continue;
        }

        // Aggiorna il lunghezza_attuale del rotolo
        rotoli[rotoloTrovato].lunghezza_attuale -= metraggioCm;
        if (rotoli[rotoloTrovato].lunghezza_attuale <= 0){
            strcpy(rotoli[rotoloTrovato].stato, "ESAURITO");
            printf("ROTOLO %s ESAURITO.\n", rotoli[rotoloTrovato].id);
        } else if (rotoli[rotoloTrovato].lunghezza_attuale <= (SOGLIA_RITAGLIO*100)){
            strcpy(rotoli[rotoloTrovato].stato, "RITAGLIO");
            creaRitaglioAutomatico(ritagli, nRitagli, &rotoli[rotoloTrovato]);
        }
        printf("DATA (GG MM AAAA): ");
        scanf("%d %d %d", &prelievi[idx].data.giorno, &prelievi[idx].data.mese, &prelievi[idx].data.anno);
        if (!controlloData(prelievi[idx].data)){
            printf("DATA NON VALIDA. Riprovare.\n");
            rotoli[rotoloTrovato].lunghezza_attuale += metraggioCm; // Ripristina
            i--;
            continue;
        }
        printf("OPERATORE: ");
        scanf("%49s", prelievi[idx].operatore);
        printf("Prelievo registrato. lunghezza attuale rotolo: %.2f cm\n", rotoli[rotoloTrovato].lunghezza_attuale);
    }
    *nPrelievi += nuovi;
    return 0;
}

int cercaPrelievo(t_Prelievo prelievi[], int nPrelievi){
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float metraggioMin;
    t_Data dataRic;

    scelta = menuCercaPrelievi();
    switch (scelta){
    case 1: // Cerca per ID PRELIEVO
        printf("INSERISCI L'ID DEL PRELIEVO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nPrelievi; i++){
            if (strcmp(prelievi[i].id, ricerca) == 0){
                printf("ID: %s, ID_ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
                       prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
                       prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
                       prelievi[i].operatore);
                trovati++;
            }
        }
        break;
    case 2: // Cerca per DATA
        printf("INSERISCI DATA (GG MM AAAA): ");
        if (scanf("%d %d %d", &dataRic.giorno, &dataRic.mese, &dataRic.anno) != 3){
            printf("INPUT DATA NON VALIDO.\n");
            return -1;
        }
        if (!controlloData(dataRic)){
            printf("DATA NON VALIDA.\n");
            return -1;
        }
        for (i = 0; i < nPrelievi; i++){
            if (prelievi[i].data.giorno == dataRic.giorno && prelievi[i].data.mese == dataRic.mese && prelievi[i].data.anno == dataRic.anno){
                printf("ID: %s, ID_ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
                       prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
                       prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
                       prelievi[i].operatore);
                trovati++;
            }
        }
        break;
    default:
        printf("OPZIONE NON VALIDA.\n");
        return -1;
    }
    if (trovati == 0)
        printf("NESSUN PRELIEVO TROVATO.\n");
    else
        printf("TOTALE PRELIEVI TROVATI: %d\n", trovati);
    return trovati;
}

int menuCercaPrelievi(){
    int scelta;
    printf("\n--- RICERCA PRELIEVO ---\n");
    printf("1. Cerca per ID PRELIEVO\n");
    printf("2. Cerca per DATA\n");
    scanf("%d", &scelta);
    return scelta;
}

int visualizzaPrelievo(t_Prelievo prelievi[], int nPrelievi){
    int i;
    printf("ELENCO PRELIEVI:\n");
    for (i = 0; i < nPrelievi; i++){
        printf("ID: %s, ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
               prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
               prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
               prelievi[i].operatore);
    }
    return 0;
}

int menuRitagli(){
    int scelta;
    printf("\n--- MENU RITAGLI ---\n");
    printf("1. VISUALIZZA RITAGLI\n");
    printf("2. CERCA RITAGLIO\n");
    printf("3. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

void visualizzaRitagli(t_Ritaglio ritagli[], int nRitagli){
    int i;
    printf("ELENCO RITAGLI:\n");
    for (i = 0; i < nRitagli; i++)
    {
        printf("ID_RITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
               ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
               ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
    }
}

int menuCercaRitagli(){
    int scelta;
    printf("\n--- RICERCA RITAGLI ---\n");
    printf("1. Cerca per ID RITAGLIO\n");
    printf("2. Cerca per ID ROTOLO\n");
    printf("3. Cerca per LUNGHEZZA MINIMA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int cercaRitaglio(t_Ritaglio ritagli[], int nRitagli){
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float lunghezzaMin;

    scelta = menuCercaRitagli();

    switch (scelta){
    case 1: // Cerca per CODICE RITAGLIO
        printf("INSERISCI l'ID DEL RITAGLIO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRitagli; i++){
            if (strcmp(ritagli[i].idRitaglio, ricerca) == 0){
                printf("ID_RITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
                       ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
                       ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
                trovati++;
            }
        }
        break;
    case 2: // Cerca per CODICE ROTOLO
        printf("INSERISCI L'ID DEL ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRitagli; i++){
            if (strcmp(ritagli[i].id_rotolo, ricerca) == 0){
                printf("IDRITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
                       ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
                       ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
                trovati++;
            }
        }
        break;
    case 3: // Cerca per LUNGHEZZA MINIMA
        printf("INSERISCI LUNGHEZZA MINIMA (cm): ");
        scanf("%f", &lunghezzaMin);
        lunghezzaMin = lunghezzaMin / 100.0;
        for (i = 0; i < nRitagli; i++){
            if (ritagli[i].lunghezza >= lunghezzaMin){
                printf("IDRITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
                       ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
                       ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
                trovati++;
            }
        }
        break;
    default:
        printf("OPZIONE NON VALIDA.\n");
        return -1;
    }
    if (trovati == 0)
        printf("NESSUN RITAGLIO TROVATO.\n");
    else
        printf("TOTALE RITAGLI TROVATI: %d\n", trovati);
    return trovati;
}

int menuFornitori(){
    int scelta;
    printf("\n--- MENU FORNITORI ---\n");
    printf("1. INSERISCI FORNITORE\n");
    printf("2. MODIFICA FORNITORE\n");
    printf("3. ELIMINA FORNITORE\n");
    printf("4. VISUALIZZA FORNITORI\n");
    printf("5. CERCA FORNITORE\n");
    printf("6. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int inserisciFornitore(t_Fornitore fornitori[], int *nFornitori){
    int i, nuovi, idx;
    printf("NUMERO FORNITORI DA AGGIUNGERE: ");
    if (scanf("%d", &nuovi) != 1 || nuovi < 1){
        printf("INPUT NON VALIDO.\n");
        return -1;
    }
    if (*nFornitori + nuovi > MAX_FORNITORI){
        printf("ERRORE: superato limite massimo fornitori.\n");
        return -1;
    }
    while (getchar() != '\n');
    for (i = 0; i < nuovi; i++){
        idx = *nFornitori + i;
        printf("\n--- Fornitore %d di %d ---\n", i + 1, nuovi);
        printf("NOME: ");
        fgets(fornitori[idx].nome, MAX_CARATTERI, stdin);
        fornitori[idx].nome[strcspn(fornitori[idx].nome, "\n")] = 0; // RIMUOVI \n

        printf("PARTITA IVA: ");
        fgets(fornitori[idx].partita_iva, MAX_CARATTERI, stdin);
        fornitori[idx].partita_iva[strcspn(fornitori[idx].partita_iva, "\n")] = 0; // RIMUOVI \n

        printf("INDIRIZZO: ");
        fgets(fornitori[idx].indirizzo, MAX_CARATTERI, stdin);
        fornitori[idx].indirizzo[strcspn(fornitori[idx].indirizzo, "\n")] = 0; // RIMUOVI \n

        printf("TELEFONO: ");
        fgets(fornitori[idx].telefono, MAX_CARATTERI, stdin);
        fornitori[idx].telefono[strcspn(fornitori[idx].telefono, "\n")] = 0; // RIMUOVI \n

        printf("EMAIL: ");
        fgets(fornitori[idx].email, MAX_CARATTERI, stdin);
        fornitori[idx].email[strcspn(fornitori[idx].email, "\n")] = 0; // RIMUOVI \n
    }

    *nFornitori += nuovi;
    printf("\n%d fornitori aggiunti con successo.\n", nuovi);
    return 0;
}

int modificaFornitore(t_Fornitore fornitori[], int nFornitori, char *nome){
    int i;
    for (i = 0; i < nFornitori; i++){
        if (strcmp(fornitori[i].nome, nome) == 0){
            printf("MODIFICA I DATI DEL FORNITORE %s:\n", nome);
            getchar();

            printf("PARTITA IVA: ");
            scanf("%49s", fornitori[i].partita_iva);
            getchar();

            printf("INDIRIZZO: ");
            fgets(fornitori[i].indirizzo, MAX_CARATTERI, stdin);
            fornitori[i].indirizzo[strcspn(fornitori[i].indirizzo, "\n")] = 0;

            printf("TELEFONO: ");
            fgets(fornitori[i].telefono, 50, stdin);
            fornitori[i].telefono[strcspn(fornitori[i].telefono, "\n")] = 0;

            printf("EMAIL: ");
            fgets(fornitori[i].email, 50, stdin);
            fornitori[i].email[strcspn(fornitori[i].email, "\n")] = 0;
            return 0;
        }
    }
    return -1;
}

int eliminaFornitore(t_Fornitore fornitori[], int *nFornitori, char *nome){
    int i, j;
    for (i = 0; i < *nFornitori; i++){
        if (strcmp(fornitori[i].nome, nome) == 0){
            for (j = i; j < *nFornitori - 1; j++){
                fornitori[j] = fornitori[j + 1];
            }
            (*nFornitori)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaFornitore(t_Fornitore fornitori[], int nFornitori){
    int i;
    printf("ELENCO FORNITORI:\n");
    for (i = 0; i < nFornitori; i++){
        printf("NOME: %s, PARTITA IVA: %s, INDIRIZZO: %s, TELEFONO: %s, EMAIL: %s\n",
               fornitori[i].nome, fornitori[i].partita_iva, fornitori[i].indirizzo,
               fornitori[i].telefono, fornitori[i].email);
    }
    return 0;
}

int cercaFornitore(t_Fornitore fornitori[], int nFornitori, char *ricerca){
    int i, trovati = 0;
    for (i = 0; i < nFornitori; i++){
        if (strcmp(fornitori[i].partita_iva, ricerca) == 0){
            printf("NOME: %s, PARTITA IVA: %s, INDIRIZZO: %s, TELEFONO: %s, EMAIL: %s\n",
                   fornitori[i].nome, fornitori[i].partita_iva, fornitori[i].indirizzo,
                   fornitori[i].telefono, fornitori[i].email);
            trovati++;
        }
    }

    if (trovati == 0)
        printf("NESSUN FORNITORE TROVATO.\n");
    else
        printf("TOTALE FORNITORI TROVATI: %d\n", trovati);
    return trovati;
}

int menuMagazzino(){
    int scelta;
    printf("\n--- MENU MAGAZZINO ---\n");
    printf("1. CONTROLLO MAGAZZINO (valore/metraggio/numero rotoli)\n");
    printf("2. VISUALIZZA MAGAZZINO\n");
    printf("3. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

// Funzione per controllare il magazzino
void controlloMagazzino(t_Rotolo rotoli[], int nRotoli){
    float valoreTotale = 0, metraggioTotale = 0;
    int numeroRotoli = nRotoli;
    int i;

    for (i = 0; i < nRotoli; i++){
        valoreTotale += (rotoli[i].lunghezza_attuale / 100.0) * rotoli[i].costo_metro;
        metraggioTotale += rotoli[i].lunghezza_attuale / 100.0; // ← FIX: converti in metri
    }
    printf("Valore Totale: %.2f Euro\n", valoreTotale);
    printf("Metraggio Totale: %.2f m\n", metraggioTotale); // ← Aggiungi unità "m"
    printf("Numero di Rotoli: %d\n", numeroRotoli);
}

// Funzione per visualizzare il magazzino
void visualizzaMagazzino(t_Rotolo rotoli[], int nRotoli){
    int i;
    printf("ELENCO ROTOLO:\n");
    for (i = 0; i < nRotoli; i++){
        printf("ID: %s, TIPO: %s, COLORE: %s, LUNGHEZZA TOTALE: %.2f m, COSTO AL METRO: %.2f\n",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].lunghezza_totale, rotoli[i].costo_metro);
    }
}

int controlloData(t_Data data){
    int bisestile;
    if (data.anno < ANNO_MIN || data.anno > ANNO_MAX) return 0;
    if (data.mese < 1 || data.mese > 12) return 0;
    if (data.giorno < 1 || data.giorno > 31) return 0;
    if (data.mese == 2){
        bisestile = (data.anno % 4 == 0 && data.anno % 100 != 0) || (data.anno % 400 == 0);
        if (bisestile && data.giorno > 29) return 0;
        if (!bisestile && data.giorno > 28) return 0;
    }else if (data.mese == 4 || data.mese == 6 || data.mese == 9 || data.mese == 11){
        if (data.giorno > 30) return 0;
    }
    return 1;
}

int creaRitaglioAutomatico(t_Ritaglio ritagli[], int *nRitagli, t_Rotolo *rotolo){
    if (*nRitagli >= MAX_RITAGLI){
        printf("ERRORE: limite ritagli raggiunto.\n");
        return -1;
    }
    sprintf(ritagli[*nRitagli].idRitaglio, "RIT%04d", *nRitagli + 1);
    strcpy(ritagli[*nRitagli].id_rotolo, rotolo->id);
    ritagli[*nRitagli].lunghezza = rotolo->lunghezza_attuale / 100.0; // Converti cm in m
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    ritagli[*nRitagli].data.giorno = tm.tm_mday;
    ritagli[*nRitagli].data.mese = tm.tm_mon + 1;
    ritagli[*nRitagli].data.anno = tm.tm_year + 1900;

    (*nRitagli)++;
    printf("Ritaglio %s creato automaticamente (%.2f m).\n", ritagli[*nRitagli - 1].idRitaglio, ritagli[*nRitagli - 1].lunghezza);
    return 0;
}

int menuProgetti(){
    int scelta;
    printf("\n--- MENU PROGETTI ---\n");
    printf("1. INSERISCI PROGETTO\n");
    printf("2. MODIFICA PROGETTO\n");
    printf("3. ELIMINA PROGETTO\n");
    printf("4. VISUALIZZA PROGETTI\n");
    printf("5. CERCA PROGETTO\n");
    printf("6. TORNA AL MENU PRINCIPALE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int inserisciProgetto(t_Progetto progetti[], int *nProgetti){
    int i, nuovi, idx;
    printf("NUMERO PROGETTI DA AGGIUNGERE: ");
    if (scanf("%d", &nuovi) != 1 || nuovi < 1){
        printf("INPUT NON VALIDO.\n");
        return -1;
    }
    if (*nProgetti + nuovi > MAX_PROGETTI){
        printf("ERRORE: superato limite massimo progetti.\n");
        return -1;
    }
    for (i = 0; i < nuovi; i++){
        idx = *nProgetti + i;
        printf("\n--- Progetto %d di %d ---\n", i + 1, nuovi);
        sprintf(progetti[idx].id, "PRJ%04d", *nProgetti + i + 1);
        printf("ID PROGETTO: %s (auto)\n", progetti[idx].id);

        printf("ID CLIENTE: ");
        scanf("%49s", progetti[idx].idCliente);

        printf("TIPO CAPO: ");
        scanf("%49s", progetti[idx].tipo_capo);

        printf("ID ROTOLO: ");
        scanf("%49s", progetti[idx].idRotolo);

        printf("TESSUTO USATO: ");
        scanf("%49s", progetti[idx].tessuto_usato);

        printf("DATA (GG MM AAAA): ");
        scanf("%d %d %d", &progetti[idx].data.giorno, &progetti[idx].data.mese, &progetti[idx].data.anno);
        if (!controlloData(progetti[idx].data)){
            printf("DATA NON VALIDA. Riprovare.\n");
            i--;
            continue;
        }
    }
    *nProgetti += nuovi;
    printf("\n%d progetti aggiunti con successo.\n", nuovi);
    return 0;
}

int modificaProgetto(t_Progetto progetti[], int nProgetti, char *id){
    int i;
    for (i = 0; i < nProgetti; i++){
        if (strcmp(progetti[i].id, id) == 0){
            printf("MODIFICA I DATI DEL PROGETTO %s:\n", id);
            printf("ID CLIENTE: ");
            scanf("%49s", progetti[i].idCliente);

            printf("TIPO CAPO: ");
            scanf("%49s", progetti[i].tipo_capo);

            printf("ID ROTOLO: ");
            scanf("%49s", progetti[i].idRotolo);

            printf("TESSUTO USATO: ");
            scanf("%49s", progetti[i].tessuto_usato);

            printf("DATA (GG MM AAAA): ");
            scanf("%d %d %d", &progetti[i].data.giorno, &progetti[i].data.mese, &progetti[i].data.anno);
            if (!controlloData(progetti[i].data)) printf("DATA NON VALIDA.\n");
            return 0;
        }
    }
    return -1;
}

int eliminaProgetto(t_Progetto progetti[], int *nProgetti, char *id){
    int i, j;
    for (i = 0; i < *nProgetti; i++){
        if (strcmp(progetti[i].id, id) == 0){
            for (j = i; j < *nProgetti - 1; j++){
                progetti[j] = progetti[j + 1];
            }
            (*nProgetti)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaProgetto(t_Progetto progetti[], int nProgetti){
    int i;
    printf("ELENCO PROGETTI:\n");
    for (i = 0; i < nProgetti; i++){
        printf("ID: %s, CLIENTE: %s, TIPO CAPO: %s, ROTOLO: %s, TESSUTO: %s, DATA: %02d/%02d/%04d\n",
               progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
               progetti[i].idRotolo, progetti[i].tessuto_usato,
               progetti[i].data.giorno, progetti[i].data.mese, progetti[i].data.anno);
    }
    return 0;
}

int cercaProgetto(t_Progetto progetti[], int nProgetti, char *id){
    int i, trovati = 0;
    if (id == NULL || id[0] == '\0'){
        printf("ID non fornito.\n");
        return -1;
    }

    for (i = 0; i < nProgetti; i++){
        if (strcmp(progetti[i].id, id) == 0){
            printf("ID: %s, CLIENTE: %s, TIPO CAPO: %s, ROTOLO: %s, TESSUTO: %s, DATA: %02d/%02d/%04d\n",
                   progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
                   progetti[i].idRotolo, progetti[i].tessuto_usato,
                   progetti[i].data.giorno, progetti[i].data.mese, progetti[i].data.anno);
            trovati++;
            break;
        }
    }

    if (trovati == 0){
        printf("NESSUN PROGETTO TROVATO CON ID %s.\n", id);
    }else{
        printf("PROGETTO TROVATO: %d\n", trovati);
    } 
    return trovati;
}

int SalvaTuttoSuFile(t_Rotolo rotoli[], int nRotoli, t_Progetto progetti[], int nProgetti, t_Fornitore fornitori[], int nFornitori, t_Prelievo prelievi[], int nPrelievi, t_Ritaglio ritagli[], int nRitagli){
    FILE *file = fopen(FNCOMPLETO, "wb");
    if (file == NULL) return -1;

    fwrite(&nRotoli, sizeof(int), 1, file);
    fwrite(rotoli, sizeof(t_Rotolo), nRotoli, file);

    fwrite(&nProgetti, sizeof(int), 1, file);
    fwrite(progetti, sizeof(t_Progetto), nProgetti, file);

    fwrite(&nFornitori, sizeof(int), 1, file);
    fwrite(fornitori, sizeof(t_Fornitore), nFornitori, file);

    fwrite(&nPrelievi, sizeof(int), 1, file);
    fwrite(prelievi, sizeof(t_Prelievo), nPrelievi, file);

    fwrite(&nRitagli, sizeof(int), 1, file);
    fwrite(ritagli, sizeof(t_Ritaglio), nRitagli, file);

    fclose(file);
    return 0;
}

int CaricaTuttoDaFile(t_Rotolo rotoli[], int *nRotoli, t_Progetto progetti[], int *nProgetti, t_Fornitore fornitori[], int *nFornitori, t_Prelievo prelievi[], int *nPrelievi, t_Ritaglio ritagli[], int *nRitagli){
    FILE *file = fopen(FNCOMPLETO, "rb");
    if (file == NULL){
        printf("NESSUN FILE TROVATO.\n");
        *nRotoli = 0;
        *nProgetti = 0;
        *nFornitori = 0;
        *nPrelievi = 0;
        *nRitagli = 0;
        return -1;
    }

    fread(nRotoli, sizeof(int), 1, file);
    fread(rotoli, sizeof(t_Rotolo), *nRotoli, file);

    fread(nProgetti, sizeof(int), 1, file);
    fread(progetti, sizeof(t_Progetto), *nProgetti, file);

    fread(nFornitori, sizeof(int), 1, file);
    fread(fornitori, sizeof(t_Fornitore), *nFornitori, file);

    fread(nPrelievi, sizeof(int), 1, file);
    fread(prelievi, sizeof(t_Prelievo), *nPrelievi, file);

    fread(nRitagli, sizeof(int), 1, file);
    fread(ritagli, sizeof(t_Ritaglio), *nRitagli, file);

    fclose(file);
    printf("CARICAMENTO COMPLETATO.\n");
    return 0;
}

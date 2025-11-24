// Gabriele Rossoni e Marcello Patrini - 4IB
// LINK PROGETTO GITHUB: https://github.com/gabrielerossoni/GestioneSartoria

// ---LIBRERIE---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ---COSTANTI---
#define MAX_CARATTERI 50
#define MAX_ROTOLI 500
#define MAX_PROGETTI 500
#define MAX_FORNITORI 200
#define MAX_PRELIEVI 1000
#define MAX_RITAGLI 1000

// ---NOMI FILE---
#define FNCOMPLETO "backup_sartoria.dat"

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
    float lunghezza_totale; /* metri */
    float residuo;          /* centimetri */
    float costo_metro;
    char fornitore[MAX_CARATTERI];
    char lotto[MAX_CARATTERI];
    t_Data data;
    char stato[MAX_CARATTERI];
    char noteAggiuntive[100];
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
    t_Rotolo rotoli[200];
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
int inserisciRotolo(t_Rotolo[], int);
int modificaRotolo(t_Rotolo[], int, char[]);
int eliminaRotolo(t_Rotolo[], int *, char[]);
int visualizzaRotolo(t_Rotolo[], int);
int cercaRotolo(t_Rotolo[], int);
int menuCercaRotoli();

// Funzioni di gestione RITAGLIO
int visualizzaRitaglio(t_Ritaglio[], int);
int cercaRitaglio(t_Ritaglio[], int);
int menuCercaRitagli();

// Funzioni di gestione PRELIEVO
int eseguiPrelievo(t_Prelievo[], int);
int visualizzaPrelievo(t_Prelievo[], int);
int cercaPrelievo(t_Prelievo[], int);

// Funzioni di gestione FORNITORE
int inserisciFornitore(t_Fornitore[], int);
int modificaFornitore(t_Fornitore[], int, char[]);
int eliminaFornitore(t_Fornitore[], int *, char[]);
int visualizzaFornitore(t_Fornitore[], int);
int cercaFornitore(t_Fornitore[], int);

// Funzioni di gestione PROGETTO
int inserisciProgetto(t_Progetto[], int);
int modificaProgetto(t_Progetto[], int, char[]);
int eliminaProgetto(t_Progetto[], int *, char[]);
int visualizzaProgetto(t_Progetto[], int);
int cercaProgetto(t_Progetto[], int);

// Funzioni di gestione MAGAZZINO
int inserisciRitaglio(t_Ritaglio[], int);
int modificaRitaglio(t_Ritaglio[], int, int);
int eliminaRitaglio(t_Ritaglio[], int *);
int visualizzaRitaglio(t_Ritaglio[], int);
int cercaRitaglio(t_Ritaglio[], int);

// Funzione salvataggio unico di tutto il programma
int SalvaTuttoSuFile(t_Rotolo[], int, t_Progetto[], int, t_Fornitore[], int, t_Prelievo[], int, t_Ritaglio[], int);
int CaricaTuttoDaFile(t_Rotolo[], int *, t_Progetto[], int *, t_Fornitore[], int *, t_Prelievo[], int *, t_Ritaglio[], int *);

// ---MAIN---
int main()
{
    t_Rotolo rotoli[MAX_ROTOLI];
    t_Progetto progetti[MAX_PROGETTI];
    t_Fornitore fornitori[MAX_FORNITORI];
    t_Prelievo prelievi[MAX_PRELIEVI];
    t_Ritaglio ritagli[MAX_RITAGLI];
    int nRotoli = 0, nProgetti = 0, nFornitori = 0, nPrelievi = 0, nRitagli = 0;
    int scelta, flag, nRotoli_count = 0, quit = 0, scelta_sub, ris;
    char id[MAX_CARATTERI], ricerca_partita_iva[MAX_CARATTERI];

    // MENU PRINCIPALE
        // Caricamento dati da file all'avvio del programma
        {
            ris = CaricaTuttoDaFile(rotoli, &nRotoli_count, progetti, &nProgetti, fornitori, &nFornitori, prelievi, &nPrelievi, ritagli, &nRitagli);
            if (ris == 0)
            {
            printf("CARICAMENTO COMPLETATO: %d rotoli, %d progetti, %d fornitori, %d prelievi, %d ritagli\n",
                   nRotoli_count, nProgetti, nFornitori, nPrelievi, nRitagli);
            }
            else
            {
            printf("NESSUN FILE DI BACKUP TROVATO. Inizio con dati vuoti.\n");
            }
        }

        do
        {
            scelta = menuGenerale();
            switch (scelta)
            {
            case 1: // ROTOLO
                do
                {
                    scelta_sub = menuRotoli();
                    switch (scelta_sub)
                    {
                    case 1: // AGGIUNGI ROTOLO
                        inserisciRotolo(rotoli, nRotoli_count);
                        break;
                    case 2: // MODIFICA ROTOLO
                        printf("INSERISCI L'ID DEL ROTOLO DA MODIFICARE: ");
                        scanf("%s", id);
                        modificaRotolo(rotoli, nRotoli_count, id);
                        break;
                    case 3: // ELIMINA ROTOLO
                        printf("INSERISCI L'ID DEL ROTOLO DA ELIMINARE: ");
                        scanf("%s", id);
                        if (eliminaRotolo(rotoli, &nRotoli_count, id) == 1)
                            printf("ROTOLO ELIMINATO CON SUCCESSO.\n");
                        else
                            printf("ROTOLO NON TROVATO.\n");
                        break;
                    case 4: // VISUALIZZA ROTOLO
                        visualizzaRotolo(rotoli, nRotoli_count);
                        break;
                    case 5: // CERCA ROTOLO
                        cercaRotolo(rotoli, nRotoli_count);
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
                break;

            case 2: // RITAGLI
                do
                {
                    scelta_sub = menuRitagli();
                    switch (scelta_sub)
                    {
                    case 1:
                        visualizzaRitaglio(ritagli, nRitagli);
                        break;
                    case 2:
                        cercaRitaglio(ritagli, nRitagli);
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 3 && !quit);
                break;

            case 3: // PRELIEVI
                do
                {
                    scelta_sub = menuPrelievi();
                    switch (scelta_sub)
                    {
                    case 1:
                        eseguiPrelievo(prelievi, nPrelievi);
                        break;
                    case 2:
                        cercaPrelievo(prelievi, nPrelievi);
                        break;
                    case 3:
                        visualizzaPrelievo(prelievi, nPrelievi);
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
                break;

            case 4: // PROGETTI
                do
                {
                    scelta_sub = menuProgetti();
                    printf("\n--- MENU PROGETTI (base) ---\n1 Inserisci\n2 Modifica\n3 Elimina\n4 Visualizza\n5 Cerca\n7 Salva e termina\nSCELTA: ");
                    scanf("%d", &scelta_sub);
                    switch (scelta_sub)
                    {
                    case 1:
                        inserisciProgetto(progetti, nProgetti);
                        break;
                    case 2:
                        printf("INSERISCI L'ID DEL PROGETTO DA MODIFICARE: ");
                        scanf("%s", id);
                        modificaProgetto(progetti, nProgetti, id);
                        break;
                    case 3:
                        printf("INSERISCI L'ID DEL PROGETTO DA ELIMINARE: ");
                        scanf("%s", id);
                        eliminaProgetto(progetti, &nProgetti, id);
                        break;
                    case 4:
                        visualizzaProgetto(progetti, nProgetti);
                        break;
                    case 5:
                        printf("INSERISCI L'ID DEL PROGETTO DA CERCARE: ");
                        scanf("%s", id);
                        flag=cercaProgetto(progetti, nProgetti, id);
                        if(flag==0)
                            printf("PROGETTO NON TROVATO.\n");
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
                break;

            case 5: // MAGAZZINO
                do
                {
                    scelta_sub = menuMagazzino();
                    switch (scelta_sub)
                    {
                    case 1: // CONTROLLO MAGAZZINO (valore/metraggio/numero rotoli)
                        controlloMagazzino(rotoli, nRotoli_count);
                        break;
                    case 2: // VISUALIZZA MAGAZZINO
                        visualizzaRotolo(rotoli, nRotoli_count);
                        break;
                    case 3: // TERMINAZIONE (senza salvataggio)
                        quit = 1;
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 3 && !quit);
                break;

            case 6: // FORNITORI
                do
                {
                    scelta_sub = menuFornitori();
                    switch (scelta_sub)
                    {
                    case 1:
                        inserisciFornitore(fornitori, nFornitori);
                        break;
                    case 2:
                        printf("INSERISCI L'ID DEL FORNITORE DA MODIFICARE: ");
                        scanf("%s", id);
                        modificaFornitore(fornitori, nFornitori, id);
                        break;
                    case 3:
                        printf("INSERISCI L'ID DEL FORNITORE DA ELIMINARE: ");
                        scanf("%s", id);
                        eliminaFornitore(fornitori, &nFornitori, id);
                        break;
                    case 4:
                        visualizzaFornitore(fornitori, nFornitori);
                        break;
                    case 5:
                    printf("INSERISCI PARTITA IVA: ");
                    scanf("%s", ricerca_partita_iva);
                        cercaFornitore(fornitori, nFornitori, ricerca_partita_iva);
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
                break;

            case 7: // SALVA E TERMINA PROGRAMMA
                SalvaTuttoSuFile(rotoli, nRotoli_count, progetti, nProgetti, fornitori, nFornitori, prelievi, nPrelievi, ritagli, nRitagli);
                printf("SALVATAGGIO COMPLETATO. USCITA PROGRAMMA.\n");
                quit = 1;
                break;

            default:
                printf("OPZIONE NON VALIDA. Riprova.\n");
                break;
            }
        } while (!quit);
}

//---FUNZIONI---
int menuGenerale()
{
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

int menuRotoli()
{
    int scelta;
    printf("\n--- MENU ROTOLO ---\n");
    printf("1. AGGIUNGI ROTOLO\n");
    printf("2. MODIFICA ROTOLO\n");
    printf("3. ELIMINA ROTOLO\n");
    printf("4. VISUALIZZA ROTOLO\n");
    printf("5. CERCA ROTOLO PER CATEGORIA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
int inserisciRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int i;
    printf("NUMERO ROTOLI DA AGGIUNGERE: ");
    scanf("%d", &nRotoli);

    for (i = 0; i < nRotoli; i++)
    {
        printf("Inserisci i dati per il rotolo %d:\n", i + 1);
        printf("ID: ");
        scanf("%s", rotoli[i].id);
        printf("TIPO: ");
        scanf("%s", rotoli[i].tipo);
        printf("COLORE: ");
        scanf("%s", rotoli[i].colore);
        printf("FANTASIA: ");
        scanf("%s", rotoli[i].fantasia);
        printf("LUNGHEZZA TOTALE (m): ");
        scanf("%f", &rotoli[i].lunghezza_totale);
        printf("REDIDUO (cm): ");
        scanf("%f", &rotoli[i].residuo);
        printf("COSTO AL METRO: ");
        scanf("%f", &rotoli[i].costo_metro);
        printf("FORNITORE: ");
        scanf("%s", rotoli[i].fornitore);
    }
    return 0;
}

int modificaRotolo(t_Rotolo rotoli[], int nRotoli, char *id)
{
    int i;
    for (i = 0; i < nRotoli; i++)
    {
        if (strcmp(rotoli[i].id, id) == 0)
        {
            printf("MODIFICA I DATI DEL ROTOLO %s:\n", id);
            printf("TIPO: ");
            scanf("%s", rotoli[i].tipo);
            printf("COLORE: ");
            scanf("%s", rotoli[i].colore);
            printf("FANTASIA: ");
            scanf("%s", rotoli[i].fantasia);
            printf("LUNGHEZZA TOTALE (m): ");
            scanf("%f", &rotoli[i].lunghezza_totale);
            printf("RESIDUO (cm): ");
            scanf("%f", &rotoli[i].residuo);
            printf("COSTO AL METRO: ");
            scanf("%f", &rotoli[i].costo_metro);
        }
    }
    return 0;
}

int eliminaRotolo(t_Rotolo rotoli[], int *nRotoli, char *id)
{
    int i, j;
    for (i = 0; i < *nRotoli; i++)
    {
        if (strcmp(rotoli[i].id, id) == 0)
        {
            for (j = i; j < *nRotoli - 1; j++)
            {
                rotoli[j] = rotoli[j + 1];
            }
            (*nRotoli)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int i;
    printf("ELENCO ROTOLO:\n");
    for (i = 0; i < nRotoli; i++)
    {
        printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA TOTALE: %.2f m, RESIDUO: %.2f cm, COSTO AL METRO: %.2f, FORNITORE: %s\n",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
               rotoli[i].lunghezza_totale, rotoli[i].residuo,
               rotoli[i].costo_metro, rotoli[i].fornitore);
    }
    return 0;
}
int menuCercaRotoli()
{
    int scelta;
    printf("\n--- RICERCA ROTOLO ---\n");
    printf("1. Cerca per CODICE ROTOLO\n");
    printf("2. Cerca per STATO\n");
    printf("3. Cerca per DISPONIBILITA' MINIMA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
int cercaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float disponibilitaMin;

    scelta = menuCercaRotoli();

    switch (scelta)
    {
    case 1: // Cerca per CODICE ROTOLO
        printf("INSERISCI IL CODICE ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].id, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].costo_metro,
                       rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 2: // Cerca per STATO
        printf("INSERISCI STATO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].stato, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].costo_metro,
                       rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 3: // Cerca per DISPONIBILITA' MINIMA
        printf("INSERISCI DISPONIBILITA' MINIMA (m): ");
        scanf("%f", &disponibilitaMin);
        for (i = 0; i < nRotoli; i++)
        {
            if (rotoli[i].lunghezza_totale >= disponibilitaMin)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, COSTO: %.2f, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].costo_metro,
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

int menuPrelievi()
{
    int scelta;
    printf("\n--- MENU PRELIEVI ---\n");
    printf("1. ESEGUI PRELIEVO\n");
    printf("2. CERCA PRELIEVO\n");
    printf("3. VISUALIZZA PRELIEVI\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
int eseguiPrelievo(t_Prelievo prelievi[], int nPrelievi)
{
    printf("NUMERO PRELIEVI DA AGGIUNGERE: ");
    scanf("%d", &nPrelievi);

    for (int i = 0; i < nPrelievi; i++)
    {
        printf("Inserisci i dati per il prelievo %d:\n", i + 1);
        printf("ID PRELIEVO: ");
        scanf("%s", prelievi[i].id);
        printf("ID ROTOLO: ");
        scanf("%s", prelievi[i].id_rotolo);
        printf("METRAGGIO PRELEVATO (m): ");
        scanf("%f", &prelievi[i].metraggio_prelevato);
        printf("DATA (GG MM AAAA): ");
        scanf("%d %d %d", &prelievi[i].data.giorno, &prelievi[i].data.mese, &prelievi[i].data.anno);
        if (!controlloData(prelievi[i].data))
        {
            printf("DATA NON VALIDA. Riprovare.\n");
            i--;
            continue;
        }
        printf("OPERATORE: ");
        scanf("%s", prelievi[i].operatore);
    }
    return 0;
}

int cercaPrelievo(t_Prelievo prelievi[], int nPrelievi)
{
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float metraggioMin;
    t_Data dataRic;

   scelta= menuCercaPrelievi();
    switch (scelta)
    {
    case 1: // Cerca per CODICE PRELIEVO
        printf("INSERISCI IL CODICE PRELIEVO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nPrelievi; i++)
        {
            if (strcmp(prelievi[i].id, ricerca) == 0)
            {
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
        if (scanf("%d %d %d", &dataRic.giorno, &dataRic.mese, &dataRic.anno) != 3)
        {
            printf("INPUT DATA NON VALIDO.\n");
            return -1;
        }
        if (!controlloData(dataRic))
        {
            printf("DATA NON VALIDA.\n");
            return -1;
        }
        for (i = 0; i < nPrelievi; i++)
        {
            if (prelievi[i].data.giorno == dataRic.giorno &&
                prelievi[i].data.mese == dataRic.mese &&
                prelievi[i].data.anno == dataRic.anno)
            {
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
int menuCercaPrelievo()
{
    int scelta;
    printf("\n--- RICERCA PRELIEVO ---\n");
    printf("1. Cerca per CODICE PRELIEVO\n");
    printf("2. Cerca per DATA\n");
    scanf("%d", &scelta);
    return scelta;
}

int visualizzaPrelievo(t_Prelievo prelievi[], int nPrelievi)
{
    int i;
    printf("ELENCO PRELIEVI:\n");
    for (i = 0; i < nPrelievi; i++)
    {
        printf("ID: %s, ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
               prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
               prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
               prelievi[i].operatore);
    }
    return 0;
}

int menuRitagli()
{
    int scelta;
    printf("\n--- MENU RITAGLI ---\n");
    printf("1. VISUALIZZA RITAGLI\n");
    printf("2. CERCA RITAGLIO\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
void visualizzaRitaglio(t_Ritaglio ritagli[], int nRitagli)
{
    int i;
    printf("ELENCO RITAGLI:\n");
    for (i = 0; i < nRitagli; i++)
    {
        printf("IDRITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
               ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
               ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
    }
}

int menuCercaRitaglio()
{
    int scelta;
    printf("\n--- RICERCA RITAGLI ---\n");
    printf("1. Cerca per CODICE RITAGLIO\n");
    printf("2. Cerca per CODICE ROTOLO\n");
    printf("3. Cerca per LUNGHEZZA MINIMA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
int cercaRitaglio(t_Ritaglio ritagli[], int nRitagli)
{
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float lunghezzaMin;

    scelta = menuCercaRitagli();

    switch (scelta)
    {
    case 1: // Cerca per CODICE RITAGLIO
        printf("INSERISCI IL CODICE RITAGLIO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRitagli; i++)
        {
            if (strcmp(ritagli[i].idRitaglio, ricerca) == 0)
            {
                printf("IDRITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
                       ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
                       ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
                trovati++;
            }
        }
        break;
    case 2: // Cerca per CODICE ROTOLO
        printf("INSERISCI IL CODICE ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRitagli; i++)
        {
            if (strcmp(ritagli[i].id_rotolo, ricerca) == 0)
            {
                printf("IDRITAGLIO: %s, ID_ROTOLO: %s, LUNGHEZZA: %.2f m, DATA: %02d/%02d/%04d\n",
                       ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza,
                       ritagli[i].data.giorno, ritagli[i].data.mese, ritagli[i].data.anno);
                trovati++;
            }
        }
        break;
    case 3: // Cerca per LUNGHEZZA MINIMA
        printf("INSERISCI LUNGHEZZA MINIMA (m): ");
        scanf("%f", &lunghezzaMin);
        for (i = 0; i < nRitagli; i++)
        {
            if (ritagli[i].lunghezza > lunghezzaMin)
            {
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

int menuFornitori()
{
    int scelta;
    printf("\n--- MENU FORNITORI ---\n");
    printf("1. INSERISCI FORNITORE\n");
    printf("2. MODIFICA FORNITORE\n");
    printf("3. ELIMINA FORNITORE\n");
    printf("4. VISUALIZZA FORNITORI\n");
    printf("5. CERCA FORNITORE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
int inserisciFornitore(t_Fornitore fornitori[], int nFornitori)
{
    int i;
    printf("NUMERO FORNITORI DA AGGIUNGERE: ");
    scanf("%d", &nFornitori);

    for (i = 0; i < nFornitori; i++)
    {
        printf("Inserisci i dati per il fornitore %d:\n", i + 1);
        printf("NOME: ");
        scanf("%s", fornitori[i].nome);
        printf("PARTITA IVA: ");
        scanf("%s", fornitori[i].partita_iva);
        printf("INDIRIZZO: ");
        scanf("%s", fornitori[i].indirizzo);
        printf("TELEFONO: ");
        scanf("%s", fornitori[i].telefono);
        printf("EMAIL: ");
        scanf("%s", fornitori[i].email);
    }
    return 0;
}

int modificaFornitore(t_Fornitore fornitori[], int nFornitori, char *nome)
{
    int i;
    for (i = 0; i < nFornitori; i++)
    {
        if (strcmp(fornitori[i].nome, nome) == 0)
        {
            printf("MODIFICA I DATI DEL FORNITORE %s:\n", nome);
            printf("PARTITA IVA: ");
            scanf("%s", fornitori[i].partita_iva);
            printf("INDIRIZZO: ");
            scanf("%s", fornitori[i].indirizzo);
            printf("TELEFONO: ");
            scanf("%s", fornitori[i].telefono);
            printf("EMAIL: ");
            scanf("%s", fornitori[i].email);
            return 0;
        }
    }
    printf("FORNITORE NON TROVATO.\n");
    return -1;
}

int eliminaFornitore(t_Fornitore fornitori[], int *nFornitori, char *nome)
{
    int i, j;
    for (i = 0; i < *nFornitori; i++)
    {
        if (strcmp(fornitori[i].nome, nome) == 0)
        {
            for (j = i; j < *nFornitori - 1; j++)
            {
                fornitori[j] = fornitori[j + 1];
            }
            (*nFornitori)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaFornitore(t_Fornitore fornitori[], int nFornitori)
{
    int i;
    printf("ELENCO FORNITORI:\n");
    for (i = 0; i < nFornitori; i++)
    {
        printf("NOME: %s, PARTITA IVA: %s, INDIRIZZO: %s, TELEFONO: %s, EMAIL: %s\n",
               fornitori[i].nome, fornitori[i].partita_iva, fornitori[i].indirizzo,
               fornitori[i].telefono, fornitori[i].email);
    }
    return 0;
}

int cercaFornitore(t_Fornitore fornitori[], int nFornitori, char *ricerca)
{
    int i, trovati = 0;
    char ricerca_local[MAX_CARATTERI];
    for (i = 0; i < nFornitori; i++)
    {
        if (strcmp(fornitori[i].partita_iva, ricerca) == 0)
        {
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

int menuMagazzino()
{
    int scelta;
    printf("\n--- MENU MAGAZZINO ---\n");
    printf("1. CONTROLLO MAGAZZINO (valore/metraggio/numero rotoli)\n");
    printf("2. VISUALIZZA MAGAZZINO\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
// Funzione per controllare il magazzino
void controlloMagazzino(t_Rotolo rotoli[], int nRotoli)
{
    float valoreTotale = 0;
    float metraggioTotale = 0;
    int numeroRotoli = nRotoli;

    for (int i = 0; i < nRotoli; i++)
    {
        valoreTotale += rotoli[i].lunghezza_totale * rotoli[i].costo_metro;
        metraggioTotale += rotoli[i].lunghezza_totale;
    }

    printf("Valore Totale: %.2f\n", valoreTotale);
    printf("Metraggio Totale: %.2f\n", metraggioTotale);
    printf("Numero di Rotoli: %d\n", numeroRotoli);
}

// Funzione per visualizzare il magazzino
void visualizzaMagazzino(t_Rotolo rotoli[], int nRotoli)
{
    printf("ELENCO ROTOLO:\n");
    for (int i = 0; i < nRotoli; i++)
    {
        printf("ID: %s, TIPO: %s, COLORE: %s, LUNGHEZZA TOTALE: %.2f m, COSTO AL METRO: %.2f\n",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].lunghezza_totale, rotoli[i].costo_metro);
    }
}

int controlloData(t_Data data)
{
    int bisestile;
    if (data.anno < 1900 || data.anno > 2100)
        return 0;
    if (data.mese < 1 || data.mese > 12)
        return 0;
    if (data.giorno < 1 || data.giorno > 31)
        return 0;

    if (data.mese == 2)
    {
        bisestile = (data.anno % 4 == 0 && data.anno % 100 != 0) || (data.anno % 400 == 0);
        if (bisestile && data.giorno > 29)
            return 0;
        if (!bisestile && data.giorno > 28)
            return 0;
    }
    else if (data.mese == 4 || data.mese == 6 || data.mese == 9 || data.mese == 11)
    {
        if (data.giorno > 30)
            return 0;
    }
    return 1;
}



int menuProgetti()
{
    int scelta;
    printf("\n--- MENU PROGETTI ---\n");
    printf("1. INSERISCI PROGETTO\n");
    printf("2. MODIFICA PROGETTO\n");
    printf("3. ELIMINA PROGETTO\n");
    printf("4. VISUALIZZA PROGETTI\n");
    printf("5. CERCA PROGETTO\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int inserisciProgetto(t_Progetto progetti[], int nProgetti)
{
    int i;
    printf("NUMERO PROGETTI DA AGGIUNGERE: ");
    scanf("%d", &nProgetti);

    for (i = 0; i < nProgetti; i++)
    {
        printf("Inserisci i dati per il progetto %d:\n", i + 1);
        printf("ID PROGETTO: ");
        scanf("%s", progetti[i].id);
        printf("ID CLIENTE: ");
        scanf("%s", progetti[i].idCliente);
        printf("TIPO CAPO: ");
        scanf("%s", progetti[i].tipo_capo);
        printf("ID ROTOLO: ");
        scanf("%s", progetti[i].idRotolo);
        printf("TESSUTO USATO: ");
        scanf("%s", progetti[i].tessuto_usato);
        printf("DATA (GG MM AAAA): ");
        scanf("%d %d %d", &progetti[i].data.giorno, &progetti[i].data.mese, &progetti[i].data.anno);
        if (!controlloData(progetti[i].data))
        {
            printf("DATA NON VALIDA. Riprovare.\n");
            i--;
        }
    }
    return 0;
}

int modificaProgetto(t_Progetto progetti[], int nProgetti, char *id)
{
    int i;
    for (i = 0; i < nProgetti; i++)
    {
        if (strcmp(progetti[i].id, id) == 0)
        {
            printf("MODIFICA I DATI DEL PROGETTO %s:\n", id);
            printf("ID CLIENTE: ");
            scanf("%s", progetti[i].idCliente);
            printf("TIPO CAPO: ");
            scanf("%s", progetti[i].tipo_capo);
            printf("ID ROTOLO: ");
            scanf("%s", progetti[i].idRotolo);
            printf("TESSUTO USATO: ");
            scanf("%s", progetti[i].tessuto_usato);
            printf("DATA (GG MM AAAA): ");
            scanf("%d %d %d", &progetti[i].data.giorno, &progetti[i].data.mese, &progetti[i].data.anno);
            if (!controlloData(progetti[i].data))
                printf("DATA NON VALIDA.\n");
            return 0;
        }
    }
    printf("PROGETTO NON TROVATO.\n");
    return -1;
}

int eliminaProgetto(t_Progetto progetti[], int *nProgetti, char *id)
{
    int i, j;
    for (i = 0; i < *nProgetti; i++)
    {
        if (strcmp(progetti[i].id, id) == 0)
        {
            for (j = i; j < *nProgetti - 1; j++)
            {
                progetti[j] = progetti[j + 1];
            }
            (*nProgetti)--;
            return 1;
        }
    }
    return -1;
}

int visualizzaProgetto(t_Progetto progetti[], int nProgetti)
{
    int i;
    printf("ELENCO PROGETTI:\n");
    for (i = 0; i < nProgetti; i++)
    {
        printf("ID: %s, CLIENTE: %s, TIPO CAPO: %s, ROTOLO: %s, TESSUTO: %s, DATA: %02d/%02d/%04d\n",
               progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
               progetti[i].idRotolo, progetti[i].tessuto_usato,
               progetti[i].data.giorno, progetti[i].data.mese, progetti[i].data.anno);
    }
    return 0;
}

int cercaProgetto(t_Progetto progetti[], int nProgetti, char *id)
{
    int i;
    int trovati = 0;

    if (id == NULL || id[0] == '\0')
    {
        printf("ID non fornito.\n");
        return -1;
    }

    for (i = 0; i < nProgetti; i++)
    {
        if (strcmp(progetti[i].id, id) == 0)
        {
            printf("ID: %s, CLIENTE: %s, TIPO CAPO: %s, ROTOLO: %s, TESSUTO: %s, DATA: %02d/%02d/%04d\n",
                   progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
                   progetti[i].idRotolo, progetti[i].tessuto_usato,
                   progetti[i].data.giorno, progetti[i].data.mese, progetti[i].data.anno);
            trovati++;
            break; // stop after first match (ID should essere unico)
        }
    }

    if (trovati == 0)
        printf("NESSUN PROGETTO TROVATO CON ID %s.\n", id);
    else
        printf("PROGETTO TROVATO: %d\n", trovati);

    return trovati;
}


int SalvaTuttoSuFile(t_Rotolo rotoli[], int nRotoli, t_Progetto progetti[], int nProgetti, t_Fornitore fornitori[], int nFornitori, t_Prelievo prelievi[], int nPrelievi, t_Ritaglio ritagli[], int nRitagli)
{
    FILE *file = fopen(FNCOMPLETO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI DATI SUL FILE.\n");
        return -1;
    }
    
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
    printf("TUTTI I DATI SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaTuttoDaFile(t_Rotolo rotoli[], int *nRotoli, t_Progetto progetti[], int *nProgetti, t_Fornitore fornitori[], int *nFornitori, t_Prelievo prelievi[], int *nPrelievi, t_Ritaglio ritagli[], int *nRitagli)
{
    FILE *file = fopen(FNCOMPLETO, "rb");
    if (file == NULL)
    {
        printf("NESSUN FILE PRECEDENTE TROVATO. INIZIO CON DATI VUOTI.\n");
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
    printf("TUTTI I DATI CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

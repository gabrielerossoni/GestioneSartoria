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
#define FNROTOLO "rotoli.dat"
#define FNPRELIEVO "prelievi.dat"
#define FNRITAGLIO "ritagli.dat"
#define FNFORNITORE "fornitori.dat"
#define FNPROGETTO "progetti.dat"
#define FNMAGAZZINO "magazzino.dat"

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
    char id[50];
    char idCliente[50];
    char tipo_capo[50];
    char idRotolo[50];
    char tessuto_usato[50];
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
int modificaFornitore(t_Fornitore[], int, int);
int eliminaFornitore(t_Fornitore[], int *);
int visualizzaFornitore(t_Fornitore[], int);
int cercaFornitore(t_Fornitore[], int);

// Funzioni di gestione PROGETTO
int inserisciProgetto(t_Progetto[], int);
int modificaProgetto(t_Progetto[], int, int);
int eliminaProgetto(t_Progetto[], int *);
int visualizzaProgetto(t_Progetto[], int);
int cercaProgetto(t_Progetto[], int);

// Funzioni di gestione MAGAZZINO
int inserisciRitaglio(t_Ritaglio[], int);
int modificaRitaglio(t_Ritaglio[], int, int);
int eliminaRitaglio(t_Ritaglio[], int *);
int visualizzaRitaglio(t_Ritaglio[], int);
int cercaRitaglio(t_Ritaglio[], int);

// Funzioni di salvataggio e caricamento su file ROTOLO
int SalvaRotoliSuFile(t_Rotolo[], int);
int CaricaRotoliDaFile(t_Rotolo[], int *);

// Funzioni di salvataggio e caricamento su file PROGETTO
int SalvaProgettiSuFile(t_Progetto[], int);
int CaricaProgettiDaFile(t_Progetto[], int *);

// Funzioni di salvataggio e caricamento su file FORNITORE
int SalvaFornitoriSuFile(t_Fornitore[], int);
int CaricaFornitoriDaFile(t_Fornitore[], int *);

// Funzioni di salvataggio e caricamento su file PRELIEVO
int SalvaPrelieviSuFile(t_Prelievo[], int);
int CaricaPrelieviDaFile(t_Prelievo[], int *);

// Funzioni di salvataggio e caricamento su file RITAGLIO
int SalvaRitagliSuFile(t_Ritaglio[], int);
int CaricaRitagliDaFile(t_Ritaglio[], int *);

// Funzione salvataggio e caricamento su file MAGAZZINO
int SalvaMagazzinoSuFile(t_Magazzino[], int);
int CaricaMagazzinoDaFile(t_Magazzino[], int *);

// Funzione salvataggio e caricamento su file TUTTO IL PROGRAMMA
int SalvaTuttoSuFile();
int CaricaTuttoDaFile();

// ---MAIN---
int main()
{
    t_Rotolo rotoli[MAX_ROTOLI];
    t_Progetto progetti[MAX_PROGETTI];
    t_Fornitore fornitori[MAX_FORNITORI];
    t_Prelievo prelievi[MAX_PRELIEVI];
    t_Ritaglio ritagli[MAX_RITAGLI];
    int nRotoli = 0, nProgetti = 0, nFornitori = 0, nPrelievi = 0, nRitagli = 0;
    int scelta, flag, nRotoli_count = 0, quit = 0, scelta_sub;
    char id[MAX_CARATTERI];

    // MENU PRINCIPALE
    {
        /* contatori locali (non toccare le variabili già dichiarate sopra) */

        // Caricamento dati da file all'avvio del programma
        if (CaricaRotoliDaFile(rotoli, &nRotoli_count) != 0)
            printf("AVVERTENZA: Impossibile caricare i rotoli.\n");
        else
            printf("Rotoli caricati: %d\n", nRotoli_count);

        if (CaricaProgettiDaFile(progetti, &nProgetti) != 0)
            printf("AVVERTENZA: Impossibile caricare i progetti.\n");
        else
            printf("Progetti caricati: %d\n", nProgetti);

        if (CaricaFornitoriDaFile(fornitori, &nFornitori) != 0)
            printf("AVVERTENZA: Impossibile caricare i fornitori.\n");
        else
            printf("Fornitori caricati: %d\n", nFornitori);

        if (CaricaPrelieviDaFile(prelievi, &nPrelievi) != 0)
            printf("AVVERTENZA: Impossibile caricare i prelievi.\n");
        else
            printf("Prelievi caricati: %d\n", nPrelievi);

        if (CaricaRitagliDaFile(ritagli, &nRitagli) != 0)
            printf("AVVERTENZA: Impossibile caricare i ritagli.\n");
        else
            printf("Ritagli caricati: %d\n", nRitagli);
            
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
                    case 7: // SALVA E TERMINA
                        SalvaRotoliSuFile(rotoli, nRotoli_count);
                        quit = 1;
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
                    case 3:
                        SalvaRitagliSuFile(ritagli, nRitagli);
                        quit = 1;
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
                    case 4:
                        SalvaPrelieviSuFile(prelievi, nPrelievi);
                        quit = 1;
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
                        cercaProgetto(progetti, nProgetti);
                        break;
                    case 7:
                        SalvaProgettiSuFile(progetti, nProgetti);
                        quit = 1;
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
                    case 1:
                        // esempio: controllo magazzino (puoi chiamare la funzione relativa)
                        printf("Eseguo controllo magazzino...\n");
                        break;
                    case 2:
                        // aggiungi ritaglio in magazzino
                        printf("Aggiungi ritaglio in magazzino...\n");
                        break;
                    case 3:
                        // rimuovi ritaglio
                        printf("Rimuovi ritaglio da magazzino...\n");
                        break;
                    case 4:
                        // visualizza magazzino
                        printf("Visualizza magazzino...\n");
                        break;
                    case 5:
                        // imposta priorita'
                        printf("Imposta priorita' utilizzo...\n");
                        break;
                    case 7:
                        SalvaMagazzinoSuFile(NULL, 0); // adattare i parametri se necessario
                        quit = 1;
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
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
                        cercaFornitore(fornitori, nFornitori);
                        break;
                    case 7:
                        SalvaFornitoriSuFile(fornitori, nFornitori);
                        quit = 1;
                        break;
                    default:
                        printf("OPZIONE NON VALIDA. Riprova.\n");
                        break;
                    }
                } while (scelta_sub != 7 && !quit);
                break;

            case 7: // SALVA E TERMINA PROGRAMMA
                SalvaRotoliSuFile(rotoli, nRotoli_count);
                SalvaProgettiSuFile(progetti, nProgetti);
                SalvaFornitoriSuFile(fornitori, nFornitori);
                SalvaPrelieviSuFile(prelievi, nPrelievi);
                SalvaRitagliSuFile(ritagli, nRitagli);
                printf("SALVATAGGIO COMPLETATO. USCITA PROGRAMMA.\n");
                quit = 1;
                break;

            default:
                printf("OPZIONE NON VALIDA. Riprova.\n");
                break;
            }
        } while (!quit);
    }
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

int menuFornitori()
{
    int scelta;
    printf("\n--- MENU FORNITORI ---\n");
    printf("1. INSERISCI FORNITORE\n");
    printf("2. MODIFICA FORNITORE\n");
    printf("3. ELIMINA FORNITORE\n");
    printf("4. VISUALIZZA FORNITORI\n");
    printf("5. CERCA FORNITORE\n");
    printf("7. SALVATAGGIO E TERMINAZIONE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

int menuMagazzino()
{
    int scelta;
    printf("\n--- MENU MAGAZZINO ---\n");
    printf("1. CONTROLLO MAGAZZINO (valore/metraggio/numero rotoli)\n");
    printf("2. AGGIUNGI RITAGLIO IN MAGAZZINO\n");
    printf("3. RIMUOVI RITAGLIO\n");
    printf("4. VISUALIZZA MAGAZZINO\n");
    printf("5. IMPOSTA PRIORITA' UTILIZZO\n");
    printf("7. SALVATAGGIO E TERMINAZIONE\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
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

int modificaRotolo(t_Rotolo rotoli[], int nRotoli, char id)
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
    printf("2. Cerca per TIPO TESSUTO\n");
    printf("3. Cerca per COLORE\n");
    printf("4. Cerca per FORNITORE\n");
    printf("5. Cerca per STATO\n");
    printf("6. Cerca per DISPONIBILITA' MINIMA\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}


int menuCercaRitagli()
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

int cercaPrelievo(t_Prelievo prelievi[], int nPrelievi)
{
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float metraggioMin;
    t_Data dataRic;

    printf("\n--- RICERCA PRELIEVO ---\n");
    printf("1. Cerca per CODICE PRELIEVO\n");
    printf("2. Cerca per CODICE ROTOLO\n");
    printf("3. Cerca per OPERATORE\n");
    printf("4. Cerca per DATA\n");
    printf("5. Cerca per METRAGGIO MINIMO\n");
    printf("SCELTA: ");
    if (scanf("%d", &scelta) != 1)
    {
        printf("INPUT NON VALIDO.\n");
        return -1;
    }

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
    case 2: // Cerca per CODICE ROTOLO
        printf("INSERISCI IL CODICE ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nPrelievi; i++)
        {
            if (strcmp(prelievi[i].id_rotolo, ricerca) == 0)
            {
                printf("ID: %s, ID_ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
                       prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
                       prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
                       prelievi[i].operatore);
                trovati++;
            }
        }
        break;
    case 3: // Cerca per OPERATORE
        printf("INSERISCI NOME OPERATORE: ");
        scanf("%s", ricerca);
        for (i = 0; i < nPrelievi; i++)
        {
            if (strcmp(prelievi[i].operatore, ricerca) == 0)
            {
                printf("ID: %s, ID_ROTOLO: %s, METRAGGIO: %.2f m, DATA: %02d/%02d/%04d, OPERATORE: %s\n",
                       prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
                       prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
                       prelievi[i].operatore);
                trovati++;
            }
        }
        break;
    case 4: // Cerca per DATA
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
    case 5: // Cerca per METRAGGIO MINIMO
        printf("INSERISCI METRAGGIO MINIMO (m): ");
        if (scanf("%f", &metraggioMin) != 1)
        {
            printf("INPUT NON VALIDO.\n");
            return -1;
        }
        for (i = 0; i < nPrelievi; i++)
        {
            if (prelievi[i].metraggio_prelevato >= metraggioMin)
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
    printf("2. Cerca per CODICE ROTOLO\n");
    printf("3. Cerca per OPERATORE\n");
    printf("4. Cerca per DATA\n");
    printf("5. Cerca per METRAGGIO MINIMO\n");
    return scelta;
}

int SalvaRotoliSuFile(t_Rotolo rotoli[], int nRotoli)
{
    FILE *file = fopen(FNROTOLO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI ROTOLO SUL FILE.\n");
        return -1;
    }
    fwrite(&nRotoli, sizeof(int), 1, file);
    fwrite(rotoli, sizeof(t_Rotolo), nRotoli, file);
    fclose(file);
    printf("ROTOLO SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaRotoliDaFile(t_Rotolo rotoli[], int *nRotoli)
{
    FILE *file = fopen(FNROTOLO, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEI ROTOLO DAL FILE.\n");
        return -1;
    }
    fread(nRotoli, sizeof(int), 1, file);
    fread(rotoli, sizeof(t_Rotolo), *nRotoli, file);
    fclose(file);
    printf("ROTOLO CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

int SalvaProgettiSuFile(t_Progetto progetti[], int nProgetti)
{
    FILE *file = fopen(FNPROGETTO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI PROGETTI SUL FILE.\n");
        return -1;
    }
    fwrite(&nProgetti, sizeof(int), 1, file);
    fwrite(progetti, sizeof(t_Progetto), nProgetti, file);
    fclose(file);
    printf("PROGETTI SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaProgettiDaFile(t_Progetto progetti[], int *nProgetti)
{
    FILE *file = fopen(FNPROGETTO, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEI PROGETTI DAL FILE.\n");
        return -1;
    }
    fread(nProgetti, sizeof(int), 1, file);
    fread(progetti, sizeof(t_Progetto), *nProgetti, file);
    fclose(file);
    printf("PROGETTI CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

int SalvaFornitoriSuFile(t_Fornitore fornitori[], int nFornitori)
{
    FILE *file = fopen(FNFORNITORE, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI FORNITORI SUL FILE.\n");
        return -1;
    }
    fwrite(&nFornitori, sizeof(int), 1, file);
    fwrite(fornitori, sizeof(t_Fornitore), nFornitori, file);
    fclose(file);
    printf("FORNITORI SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaFornitoriDaFile(t_Fornitore fornitori[], int *nFornitori)
{
    FILE *file = fopen(FNFORNITORE, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEI FORNITORI DAL FILE.\n");
        return -1;
    }
    fread(nFornitori, sizeof(int), 1, file);
    fread(fornitori, sizeof(t_Fornitore), *nFornitori, file);
    fclose(file);
    printf("FORNITORI CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

int SalvaPrelieviSuFile(t_Prelievo prelievi[], int nPrelievi)
{
    FILE *file = fopen(FNPRELIEVO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI PRELIEVI SUL FILE.\n");
        return -1;
    }
    fwrite(&nPrelievi, sizeof(int), 1, file);
    fwrite(prelievi, sizeof(t_Prelievo), nPrelievi, file);
    fclose(file);
    printf("PRELIEVI SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaPrelieviDaFile(t_Prelievo prelievi[], int *nPrelievi)
{
    FILE *file = fopen(FNPRELIEVO, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEI PRELIEVI DAL FILE.\n");
        return -1;
    }
    fread(nPrelievi, sizeof(int), 1, file);
    fread(prelievi, sizeof(t_Prelievo), *nPrelievi, file);
    fclose(file);
    printf("PRELIEVI CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

int SalvaRitagliSuFile(t_Ritaglio ritagli[], int nRitagli)
{
    FILE *file = fopen(FNRITAGLIO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEI RITAGLI SUL FILE.\n");
        return -1;
    }
    fwrite(&nRitagli, sizeof(int), 1, file);
    fwrite(ritagli, sizeof(t_Ritaglio), nRitagli, file);
    fclose(file);
    printf("RITAGLI SALVATI CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaRitagliDaFile(t_Ritaglio ritagli[], int *nRitagli)
{
    FILE *file = fopen(FNRITAGLIO, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEI RITAGLI DAL FILE.\n");
        return -1;
    }
    fread(nRitagli, sizeof(int), 1, file);
    fread(ritagli, sizeof(t_Ritaglio), *nRitagli, file);
    fclose(file);
    printf("RITAGLI CARICATI CON SUCCESSO DAL FILE.\n");
    return 0;
}

int SalvaMagazzinoSuFile(t_Magazzino magazzino[], int nMagazzino)
{
    FILE *file = fopen(FNMAGAZZINO, "wb");
    if (file == NULL)
    {
        printf("ERRORE NEL SALVATAGGIO DEL MAGAZZINO SUL FILE.\n");
        return -1;
    }
    fwrite(&nMagazzino, sizeof(int), 1, file);
    fwrite(magazzino, sizeof(t_Magazzino), nMagazzino, file);
    fclose(file);
    printf("MAGAZZINO SALVATO CON SUCCESSO SUL FILE.\n");
    return 0;
}

int CaricaMagazzinoDaFile(t_Magazzino magazzino[], int *nMagazzino)
{
    FILE *file = fopen(FNMAGAZZINO, "rb");
    if (file == NULL)
    {
        printf("ERRORE NEL CARICAMENTO DEL MAGAZZINO DAL FILE.\n");
        return -1;
    }
    fread(nMagazzino, sizeof(int), 1, file);
    fread(magazzino, sizeof(t_Magazzino), *nMagazzino, file);
    fclose(file);
    printf("MAGAZZINO CARICATO CON SUCCESSO DAL FILE.\n");
    return 0;
}


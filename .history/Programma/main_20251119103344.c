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
#define FNABITI "abiti.dat"
#define FNINVENTARIO "inventario.dat"
#define FNROTOLO "rotoli.dat"

// ---STRUTTURE DATI---
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
typedef struct
{
    int giorno;
    int mese;
    int anno;
} t_Data;

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
int modificaRotolo(t_Rotolo[], int, int);
int eliminaRotolo(t_Rotolo[], int *);
int visualizzaRotolo(t_Rotolo[], int);
int cercaRotolo(t_Rotolo[], int);

// Funzioni di gestione RITAGLIO
int inserisciRitaglio(t_Ritaglio[], int);
int modificaRitaglio(t_Ritaglio[], int, int);
int eliminaRitaglio(t_Ritaglio[], int *);
int visualizzaRitaglio(t_Ritaglio[], int);
int cercaRitaglio(t_Ritaglio[], int);

// Funzioni di gestione PRELIEVO
int inserisciPrelievo(t_Prelievo[], int);
int modificaPrelievo(t_Prelievo[], int, int);
int eliminaPrelievo(t_Prelievo[], int *);
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
    int *nRotoli = 0, nProgetti = 0, nFornitori = 0, nPrelievi = 0, nRitagli = 0;
    int scelta, flag;
    char id[MAX_CARATTERI];
    
    // MENU PRINCIPALE 
    do{
        scelta = menuGenerale();
    
    {
        
        
        
        {
        case 1:
            flag = inserisciRotolo(rotoli, nRotoli);
            break;
        case 2:
            printf("INSERISCI L'ID DEL ROTOLO DA MODIFICARE: ");
            scanf("%s", id);
            modificaRotolo(rotoli, nRotoli, id);
            break;
        case 3:
            printf("INSERISCI L'ID DEL ROTOLO DA MODIFICARE: ");
            scanf("%s", id);
            nRotoli = eliminaRotolo(rotoli, &nRotoli, id);
            if (nRotoli == -1)
            {
                printf("ROTOLO NON TROVATO.\n");
            }
            else
            {
                printf("ROTOLO ELIMINATO CON SUCCESSO.\n");
            }
            break;
        case 4:
            visualizzaRotolo(rotoli, nRotoli);
            break;
        case 5:
            cercaRotolo(rotoli, nRotoli, id);
            break;
        case 7:
            SalvaRotoliSuFile(rotoli, nRotoli);
            printf("USCITA PROGRAMMA.\n");
            break;
        default:
            printf("OPZIONE NON VALIDA. Riprova.\n");
            break;
        }
    } while (scelta != 7);
    return 0;
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

int menuRotolo()
{
    int scelta;
    printf("\n--- MENU SARTORIA ---\n");
    printf("1. AGGIUNGI ROTOLO\n");
    printf("2. MODIFICA ROTOLO\n");
    printf("3. ELIMINA ROTOLO\n");
    printf("4. VISUALIZZA ROTOLO\n");
    printf("5. CERCA ROTOLO PER CATEGORIA\n");
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

int menuRotoli()
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

int modificaRotolo(t_Rotolo rotoli[], int nRotoli, int id)
{
    int i;
    char id[MAX_CARATTERI];
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

int cercaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int scelta, i, trovati = 0;
    char ricerca[MAX_CARATTERI];
    float lunghezzaMin;

    scelta = menuRotoli();

    switch (scelta)
    {
    case 1:
        printf("INSERISCI IL CODICE ROTOLO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].id, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 2:
        printf("INSERISCI IL TIPO TESSUTO: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].tipo, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 3:
        printf("INSERISCI IL COLORE: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].colore, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 4:
        printf("INSERISCI IL FORNITORE: ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].fornitore, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 5:
        printf("INSERISCI LO STATO (NUOVO/IN USO/ESAURITO): ");
        scanf("%s", ricerca);
        for (i = 0; i < nRotoli; i++)
        {
            if (strcmp(rotoli[i].stato, ricerca) == 0)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
                trovati++;
            }
        }
        break;
    case 6:
        printf("INSERISCI LUNGHEZZA MINIMA (m): ");
        scanf("%f", &lunghezzaMin);
        for (i = 0; i < nRotoli; i++)
        {
            if (rotoli[i].lunghezza_totale > lunghezzaMin)
            {
                printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA: %.2f m, RESIDUO: %.2f cm, FORNITORE: %s, STATO: %s\n",
                       rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                       rotoli[i].lunghezza_totale, rotoli[i].residuo, rotoli[i].fornitore, rotoli[i].stato);
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
    FILE *file = fopen("progetti.dat", "wb");
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
    FILE *file = fopen("progetti.dat", "rb");
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
    FILE *file = fopen("fornitori.dat", "wb");
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
    FILE *file = fopen("fornitori.dat", "rb");
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
    FILE *file = fopen("prelievi.dat", "wb");
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
    FILE *file = fopen("prelievi.dat", "rb");
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
    FILE *file = fopen("ritagli.dat", "wb");
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
    FILE *file = fopen("ritagli.dat", "rb");
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
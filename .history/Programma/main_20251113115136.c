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
    t_Data data_acquisto;
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
    t_Data dataCreazione;
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
    t_Data dataConsegna;
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
int menu();
int inserisciRotolo(t_Rotolo[], int);
int modificaRotolo(t_Rotolo[], int);
int eliminaRotolo(t_Rotolo[], int);
int visualizzaRotolo(t_Rotolo[], int);
int cercaRotoloPerCategoria();
int SalvaRotoliSuFile(t_Rotolo[], int);
int CaricaRotoliDaFile(t_Rotolo[], int *);
int SalvaProgettiSuFile(t_Progetto[], int);
int CaricaProgettiDaFile(t_Progetto[], int *);
int SalvaFornitoriSuFile(t_Fornitore[], int);
int CaricaFornitoriDaFile(t_Fornitore[], int *);
int SalvaPrelieviSuFile(t_Prelievo[], int);
int CaricaPrelieviDaFile(t_Prelievo[], int *);
int SalvaRitagliSuFile(t_Ritaglio[], int);
int CaricaRitagliDaFile(t_Ritaglio[], int *);

// ---MAIN---
int main()
{
    t_Rotolo rotoli[MAX_ROTOLI];
    t_Progetto progetti[MAX_PROGETTI];
    t_Fornitore fornitori[MAX_FORNITORI];
    t_Prelievo prelievi[MAX_PRELIEVI];
    t_Ritaglio ritagli[MAX_RITAGLI];
    int nRotoli = 0, nProgetti = 0, nFornitori = 0, nPrelievi = 0, nRitagli = 0;
    int scelta;

    do
    {
        scelta = menu();
        switch (scelta)
        {
        case 1:
            nRotoli = inserisciRotolo(rotoli);
            break;
        case 2:
        printf("INSERISCI L'ID DEL ROTOLO DA ELIMINARE: ");
            scanf("%s", id);
            modificaRotolo(rotoli);
            break;
        case 3:
            nRotoli = eliminaRotolo(rotoli);
            break;
        case 4:
            visualizzaRotolo(rotoli);
            break;
        case 5:
            cercaRotoloPerCategoria();
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
int menu()
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

int modificaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int i;
    char id[MAX_CARATTERI];
    printf("INSERISCI L'ID DEL ROTOLO DA MODIFICARE: ");
    scanf("%s", id);
    for(i=0; i<nRotoli; i++) {
        if(strcmp(rotoli[i].id, id) == 0) {
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

int eliminaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int i, j;
    char id[MAX_CARATTERI];
    for(i=0; i<nRotoli; i++) {
        if(strcmp(rotoli[i].id, id) == 0) {
            for(j=i; j<nRotoli-1; j++) {
                rotoli[j] = rotoli[j+1];
            }
            nRotoli--;
            printf("ROTOLO %s ELIMINATO.\n", id);
            return nRotoli;
         }
    }
    printf("ROTOLO %s NON TROVATO.\n", id);
    return nRotoli;
}

int visualizzaRotolo(t_Rotolo rotoli[], int nRotoli)
{
    int i;
    printf("ELENCO ROTOLO:\n");
    for(i=0; i<nRotoli; i++) {
        printf("ID: %s, TIPO: %s, COLORE: %s, FANTASIA: %s, LUNGHEZZA TOTALE: %.2f m, RESIDUO: %.2f cm, COSTO AL METRO: %.2f, FORNITORE: %s\n",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
               rotoli[i].lunghezza_totale, rotoli[i].residuo,
               rotoli[i].costo_metro, rotoli[i].fornitore);
    }
    return 0;
}

int cercaRotoloPerColore()
{
    char categoria[MAX_CARATTERI];
    printf("INSERISCI LA CATEGORIA DA CERCARE: ");
    scanf("%s", categoria);
    // Implementare la logica di ricerca in base alla categoria
    printf("FUNZIONE DI RICERCA NON ANCORA IMPLEMENTATA.\n");
    return 0;
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



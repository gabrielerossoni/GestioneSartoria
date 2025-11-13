// Gabriele Rossoni e Marcello Patrini - 4IB
// LINK PROGETTO GITHUB: https://github.com/gabrielerossoni/GestioneSartoria

// ---LIBRERIE---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---COSTANTI---
#define MAX_CARATTERI 50
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
    char nome[50];
    char partita_iva[20];
    char indirizzo[100];
    char telefono[20];
    char email[50];
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
int inserisciRotolo(t_Rotolo[]);
int modificaRotolo(t_Rotolo[]);
int eliminaRotolo(t_Rotolo[] );
int visualizzaRotolo();
int cercaRotoloPerCategoria();
int SalvaRotoliSuFile(t_Rotolo[], int);
int CaricaRotoliDaFile(t_Rotolo[], int*);
int SalvaProgettiSuFile(t_Progetto[], int);
int CaricaProgettiDaFile(t_Progetto[], int*);
int Salva

// ---MAIN---
int main()
{
    int scelta;
    do
    {
        scelta = menu();
        switch (scelta)
        {
        case 1:

            inserisciRotolo();
            break;
        case 2:
            modificaRotolo();
            break;
        case 3:
            eliminaRotolo();
            break;
        case 4:
            ();
            break;
        case 5:
            cercaAbitoPerTaglia();
            break;
        case 6:
            cercaAbitoPerPrezzo();
            break;
        case 7:
            printf("Uscita dal programma.\n");
            break;
        default:
            printf("Opzione non valida. Riprova.\n");
        }
    } while (scelta != 7);
    return 0;
}

//---FUNZIONI---
int menu(){
    int scelta;
    printf("\n--- MENU SARTORIA ---\n");
    printf("1. AGGIUNGI ROTOLO\n");
    printf("2. MODIFICA ROTOLO\n");
    printf("3. ELIMINA ROTOLO\n"); 
    printf("4. VISUALIZZA ROTOLO\n");
    printf("5. CERCA ROTOLO PER CATEGORIA\n");
    printf("7. ESCI\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
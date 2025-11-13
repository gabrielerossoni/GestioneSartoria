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
typedef struct{
    char id[MAX_CARATTERI]; // auto incrementale
    char tipo[MAX_CARATTERI];
    char colore[MAX_CARATTERI];
    char fantasia[MAX_CARATTERI];
    float lunghezza_totale;   /* metri */
    float residuo; /* centimetri */
    float costo_metro;
    char fornitore[MAX_CARATTERI];
    char lotto[MAX_CARATTERI];
    t_Data data_acquisto;
    char stato[MAX_CARATTERI];
    char noteAggiuntive[100];
} t_Rotolo;

typedef struct{
    char id[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float metraggio_prelevato;
    t_Data data;
    char operatore[MAX_CARATTERI];
} t_Prelievo;

typedef struct{
    char idRitaglio[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float lunghezza;
    t_Data dataCreazione;
} t_Ritaglio;
typedef struct{
    char nome[50];
    char partita_iva[20];
    char indirizzo[100];
    char telefono[20];
    char email[50];
} t_Fornitore;

typedef struct{
    id(char[50]) = Codice identificativo univoco del progetto, utile per la 
tracciabilità. 
●  idCliente(char[50]) = Codice fiscale del cliente per collegare il progetto al 
destinatario. 
●  tipo_capo (char[50]) = Specifica il tipo di capo realizzato (camicia, giacca, 
pantalone, ecc.). 
●  idRotolo (char[50]) = Codice del rotolo principale da cui è stato prelevato il 
tessuto. 
●  tessuto_usato(char[50]) = Descrizione del tessuto impiegato nel progetto 
(tipo, colore, fantasia). 
●   dataConsegna(struct data) = Giorno, mese e anno della consegna prevista 
o effettiva del capo.
} t_Proget;
typedef struct{
    int giorno;
    int mese;
    int anno;
} t_Data;

// ---PROTOTIPI FUNZIONI---
int menu();
int inserisciRotolo();
int modificaRotolo();
int eliminaRotolo();

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
            cercaAbitoPerColore();
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
    printf("2. VISUALIZZA ROTOLO\n");
    printf("3. CERCA ROTOLO PER CATEGORIA\n");
    printf("7. ESCI\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}

void aggiungiAbito(){
    Abito abito;
    FILE *file = fopen(FILENAME, "ab");
    if (file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        return;
    }
    printf("Inserisci il tipo di abito: ");
    scanf("%s", abito.tipo);
    printf("Inserisci il colore di abito: ");
    scanf("%s", abito.colore);
    printf("Inserisci la taglia di abito: ");
    scanf("%s", abito.taglia);
    printf("Inserisci il prezzo di abito: ");
    scanf("%f", &abito.prezzo);
    fwrite(&abito, sizeof(Abito), 1, file);
    fclose(file);
    printf("Abito aggiunto con successo.\n");
}
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
    costo_metro (float) = Prezzo del tessuto per metro lineare. 
●  fornitore (struct Fornitore) = Struttura che contiene i dati del fornitore 
(Partita IVA, nome, indirizzo, contatti, ecc.). 
●  lotto (char[50]) = Codice identificativo del lotto, generato automaticamente 
in base al fornitore. 
●  data_acquisto (struct data) = Struttura che memorizza la data di acquisto 
(giorno, mese, anno). 
●  stato (char) = Indica lo stato del rotolo (es. NUOVO, ESAURITO, ecc.). 
●  noteAggiuntive (char[100]) = Campo testuale per eventuali annotazioni
} Rotolo;

typedef struct{
    int id; // auto incrementale
    int rotolo_id; // riferimento al rotolo
    float lunghezza_utilizzata; // metri   
    float costo; // euro
    float
    
} Prelievo;

typedef struct{
    int giorno;
    int mese;
    int anno;
} Data;

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
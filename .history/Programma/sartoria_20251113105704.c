// Marcello Patrini e Gabriele Rossoni - 4IB
// LINK PROGETTO GITHUB: https://github.com/gabrielerossoni/GestioneSartoria

// ---LIBRERIE---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---COSTANTI---
#define MAX_CARATTERI 50
#define FILENAME1 "abiti.dat"
#define FILENAME2 "inventario.dat"
#define FILENAME3 "rotoli.dat"

// ---STRUTTURE DATI---
typedef struct{
    char id[MAX_CARATTERI]; // auto incrementale
    char tipo[MAX_CARATTERI];
    char colore[MAX_CARATTERI];
    char fantasia[MAX_CARATTERI];
    int lunghezza;   /* metri */
    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;

typedef struct {
    char id[50];	// incrementale
    char id_rotolo[50];
    float metraggio_prelevato;
    char operatore[50];	// nome di chi ha effettuato il prelievo
    char data[11];
} Prelievo;

typedef struct
{
    lunghezza; /* metri */

    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;
typedef struct
{
    lunghezza; /* metri */

    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;
typedef struct
{
    char tipo[MAX];
    char colore[MAX];
    char taglia[MAX];
    float;
} Fornitore;

// ---PROTOTIPI FUNZIONI---
int menu();
int inserisciRotolo();
int modificaRotolo();
int cercaAbitoPerTipo();
int cercaAbitoPerColore();
int cercaAbitoPerTaglia();
int cercaAbitoPerPrezzo();

// ---MAIN---   
int main(){
    int scelta;
    do
    {
        scelta = menu();
        switch (scelta)
        {
        case 1:
            aggiungiAbito();
            break;
        case 2:
            visualizzaAbiti();
            break;
        case 3:
            cercaAbitoPerTipo();
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


int manu(){
    int scelta;
    printf("\n--- MENU SARTORIA ---\n");
    printf("1. AGGIUNGI ABITO\n");
    printf("2. VISUALIZZA ABITI\n");
    printf("3. CERCA ABITO PER TIPO\n");
    printf("4. CERCA ABITO PER COLORE\n");
    printf("5. CERCA ABITO PER TAGLIA\n");
    printf("6. CERCA ABITO PER PREZZO\n");
    printf("7. ESCI\n");
    printf("SCELTA: ");
    scanf("%d", &scelta);
    return scelta;
}
void aggiungiAbito()
{
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
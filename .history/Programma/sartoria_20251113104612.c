// Marcello Patrini e Gabriele Rossoni      4IB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[50];	// auto incrementale
    char tipo[50];
    char colore[30];
    char fantasia[30];
    int lunghezza; /* metri */
    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;
typedef struct {
 lunghezza; /* metri */

    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;
typedef struct {
 lunghezza; /* metri */

    float larghezza; /* centimetri */
    float peso;      /* chilogrammi */
    int quantita;
} Rotolo;
typedef struct {
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
    float ;
} Fornitore;

int inseris();
int visualizzaAbiti();
int cercaAbitoPerTipo();
int cercaAbitoPerColore();
int cercaAbitoPerTaglia();
int cercaAbitoPerPrezzo();

int main()
{
    int scelta;
    do
    {
        printf("\n--- Menu Sartoria ---\n");
        printf("1. Aggiungi Abito\n");
        printf("2. Visualizza Abiti\n");
        printf("3. Cerca Abito per Tipo\n");
        printf("4. Cerca Abito per Colore\n");
        printf("5. Cerca Abito per Taglia\n");
        printf("6. Cerca Abito per Prezzo\n");
        printf("7. Esci\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);
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
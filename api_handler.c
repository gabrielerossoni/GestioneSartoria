// api_handler.c - Versione API del programma GestioneSartoria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROTOLI 500
#define MAX_CARATTERI 50
#define FILENAME "rotoli_data.txt"

typedef struct
{
    char id[MAX_CARATTERI];
    char tipo[MAX_CARATTERI];
    char colore[MAX_CARATTERI];
    float lunghezza_totale;
    float costo_metro;
} t_Rotolo;

// Carica rotoli da file
int caricaRotoli(t_Rotolo rotoli[], int *nRotoli)
{
    FILE *f = fopen(FILENAME, "r");
    if (!f)
    {
        *nRotoli = 0;
        return -1;
    }

    *nRotoli = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^|]|%f|%f\n",
                  rotoli[*nRotoli].id,
                  rotoli[*nRotoli].tipo,
                  rotoli[*nRotoli].colore,
                  &rotoli[*nRotoli].lunghezza_totale,
                  &rotoli[*nRotoli].costo_metro) == 5)
    {
        (*nRotoli)++;
        if (*nRotoli >= MAX_ROTOLI)
            break;
    }

    fclose(f);
    return 0;
}

// Salva rotoli su file
int salvaRotoli(t_Rotolo rotoli[], int nRotoli)
{
    FILE *f = fopen(FILENAME, "w");
    if (!f)
        return -1;

    for (int i = 0; i < nRotoli; i++)
    {
        fprintf(f, "%s|%s|%s|%.2f|%.2f\n",
                rotoli[i].id, rotoli[i].tipo, rotoli[i].colore,
                rotoli[i].lunghezza_totale, rotoli[i].costo_metro);
    }

    fclose(f);
    return 0;
}

// Comando LIST: mostra tutti i rotoli in formato JSON
void cmdList()
{
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;

    caricaRotoli(rotoli, &nRotoli);

    printf("[");
    for (int i = 0; i < nRotoli; i++)
    {
        printf("{\"id\":\"%s\",\"tipo\":\"%s\",\"colore\":\"%s\",\"lunghezza\":%.2f,\"costo\":%.2f}",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore,
               rotoli[i].lunghezza_totale, rotoli[i].costo_metro);
        if (i < nRotoli - 1)
            printf(",");
    }
    printf("]\n");
}

// Comando ADD: aggiungi nuovo rotolo
void cmdAdd(char *tipo, char *colore, float lunghezza, float costo)
{
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;

    caricaRotoli(rotoli, &nRotoli);

    if (nRotoli >= MAX_ROTOLI)
    {
        printf("{\"status\":\"error\",\"message\":\"Limite massimo raggiunto\"}\n");
        return;
    }

    // Genera ID
    sprintf(rotoli[nRotoli].id, "R%04d", nRotoli + 1);
    strncpy(rotoli[nRotoli].tipo, tipo, MAX_CARATTERI - 1);
    strncpy(rotoli[nRotoli].colore, colore, MAX_CARATTERI - 1);
    rotoli[nRotoli].lunghezza_totale = lunghezza;
    rotoli[nRotoli].costo_metro = costo;

    nRotoli++;

    if (salvaRotoli(rotoli, nRotoli) == 0)
    {
        printf("{\"status\":\"ok\",\"message\":\"Rotolo aggiunto con successo\"}\n");
    }
    else
    {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

void cmdDelete(char *id) {
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;
    
    caricaRotoli(rotoli, &nRotoli);
    
    int trovato = -1;
    for (int i = 0; i < nRotoli; i++) {
        if (strcmp(rotoli[i].id, id) == 0) {
            trovato = i;
            break;
        }
    }
    
    if (trovato == -1) {
        printf("{\"status\":\"error\",\"message\":\"Rotolo non trovato\"}\n");
        return;
    }
    
    // Sposta tutti gli elementi successivi indietro
    for (int i = trovato; i < nRotoli - 1; i++) {
        rotoli[i] = rotoli[i + 1];
    }
    nRotoli--;
    
    if (salvaRotoli(rotoli, nRotoli) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Rotolo eliminato\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("{\"status\":\"error\",\"message\":\"Comando mancante\"}\n");
        return 1;
    }
    
    if (strcmp(argv[1], "list") == 0) {
        cmdList();
    }
    else if (strcmp(argv[1], "add") == 0 && argc == 6) {
        float lunghezza = atof(argv[4]);
        float costo = atof(argv[5]);
        cmdAdd(argv[2], argv[3], lunghezza, costo);
    }
    else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
        // AGGIUNGI QUESTA PARTE
        cmdDelete(argv[2]);
    }
    else {
        printf("{\"status\":\"error\",\"message\":\"Comando non riconosciuto\"}\n");
        return 1;
    }
    
    return 0;
}


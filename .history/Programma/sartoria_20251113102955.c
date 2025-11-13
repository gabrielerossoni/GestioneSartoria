// Marcello Patrini e Gabriele Rossoni      4IB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILENAME "abiti.txt"
typedef struct {
    char tipo[MAX];
    char colore[MAX];
    char taglia[MAX];
    float prezzo;
} Abito;
void aggi
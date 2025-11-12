#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sartoria.h"
#define MAX_NAME_LENGTH 100
#define MAX_ITEMS 100
typedef struct {
    char name[MAX_NAME_LENGTH];
    float price;
} Item;
typedef struct {
    Item items[MAX_ITEMS];
    int itemCount;
} Sartoria;
Sartoria* createSartoria() {
    Sartoria* sartoria = (Sartoria*)malloc(sizeof(Sartoria));
    sartoria->itemCount = 0;
    return sartoria;
}
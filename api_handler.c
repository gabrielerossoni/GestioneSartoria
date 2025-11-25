// api_handler.c - Versione API completa del programma GestioneSartoria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// COSTANTI
#define MAX_ROTOLI 500
#define MAX_PROGETTI 500
#define MAX_FORNITORI 200
#define MAX_PRELIEVI 1000
#define MAX_RITAGLI 1000
#define MAX_CARATTERI 50
#define SOGLIA_RITAGLIO 0.5

// FILE
#define FILE_ROTOLI "rotoli_data.txt"
#define FILE_PRELIEVI "prelievi_data.txt"
#define FILE_RITAGLI "ritagli_data.txt"
#define FILE_FORNITORI "fornitori_data.txt"
#define FILE_PROGETTI "progetti_data.txt"

// STRUTTURE DATI
typedef struct {
    int giorno;
    int mese;
    int anno;
} t_Data;

typedef struct {
    char id[MAX_CARATTERI];
    char tipo[MAX_CARATTERI];
    char colore[MAX_CARATTERI];
    char fantasia[MAX_CARATTERI];
    float lunghezza_totale;
    float lunghezza_attuale;
    float costo_metro;
    char fornitore[MAX_CARATTERI];
    char lotto[MAX_CARATTERI];
    t_Data data;
    char stato[MAX_CARATTERI];
    char noteAggiuntive[100];
} t_Rotolo;

typedef struct {
    char id[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float metraggio_prelevato;
    t_Data data;
    char operatore[MAX_CARATTERI];
    char cliente[MAX_CARATTERI];
} t_Prelievo;

typedef struct {
    char idRitaglio[MAX_CARATTERI];
    char id_rotolo[MAX_CARATTERI];
    float lunghezza;
    t_Data data;
} t_Ritaglio;

typedef struct {
    char nome[MAX_CARATTERI];
    char partita_iva[MAX_CARATTERI];
    char indirizzo[MAX_CARATTERI];
    char telefono[MAX_CARATTERI];
    char email[MAX_CARATTERI];
} t_Fornitore;

typedef struct {
    char id[MAX_CARATTERI];
    char idCliente[MAX_CARATTERI];
    char tipo_capo[MAX_CARATTERI];
    char idRotolo[MAX_CARATTERI];
    char tessuto_usato[MAX_CARATTERI];
    t_Data data;
} t_Progetto;

// ===== FUNZIONI UTILITA =====
void getCurrentDate(t_Data *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    data->giorno = tm.tm_mday;
    data->mese = tm.tm_mon + 1;
    data->anno = tm.tm_year + 1900;
}

void printDate(t_Data data) {
    printf("\"%02d/%02d/%04d\"", data.giorno, data.mese, data.anno);
}

// ===== ROTOLI =====
int caricaRotoli(t_Rotolo rotoli[], int *nRotoli) {
    FILE *f = fopen(FILE_ROTOLI, "r");
    if (!f) {
        *nRotoli = 0;
        return -1;
    }

    *nRotoli = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%f|%f|%f|%49[^|]|%49[^|]|%d/%d/%d|%49[^|]|%99[^\n]\n",
                  rotoli[*nRotoli].id,
                  rotoli[*nRotoli].tipo,
                  rotoli[*nRotoli].colore,
                  rotoli[*nRotoli].fantasia,
                  &rotoli[*nRotoli].lunghezza_totale,
                  &rotoli[*nRotoli].lunghezza_attuale,
                  &rotoli[*nRotoli].costo_metro,
                  rotoli[*nRotoli].fornitore,
                  rotoli[*nRotoli].lotto,
                  &rotoli[*nRotoli].data.giorno,
                  &rotoli[*nRotoli].data.mese,
                  &rotoli[*nRotoli].data.anno,
                  rotoli[*nRotoli].stato,
                  rotoli[*nRotoli].noteAggiuntive) == 14) {
        (*nRotoli)++;
        if (*nRotoli >= MAX_ROTOLI) break;
    }

    fclose(f);
    return 0;
}

int salvaRotoli(t_Rotolo rotoli[], int nRotoli) {
    FILE *f = fopen(FILE_ROTOLI, "w");
    if (!f) return -1;

    for (int i = 0; i < nRotoli; i++) {
        fprintf(f, "%s|%s|%s|%s|%.2f|%.2f|%.2f|%s|%s|%02d/%02d/%04d|%s|%s\n",
                rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
                rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale, rotoli[i].costo_metro,
                rotoli[i].fornitore, rotoli[i].lotto,
                rotoli[i].data.giorno, rotoli[i].data.mese, rotoli[i].data.anno,
                rotoli[i].stato, rotoli[i].noteAggiuntive);
    }

    fclose(f);
    return 0;
}

void cmdListRotoli() {
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;

    caricaRotoli(rotoli, &nRotoli);

    printf("[");
    for (int i = 0; i < nRotoli; i++) {
        printf("{\"id\":\"%s\",\"tipo\":\"%s\",\"colore\":\"%s\",\"fantasia\":\"%s\",\"lunghezza_totale\":%.2f,\"lunghezza_attuale\":%.2f,\"costo_metro\":%.2f,\"fornitore\":\"%s\",\"lotto\":\"%s\",\"data\":",
               rotoli[i].id, rotoli[i].tipo, rotoli[i].colore, rotoli[i].fantasia,
               rotoli[i].lunghezza_totale, rotoli[i].lunghezza_attuale, rotoli[i].costo_metro,
               rotoli[i].fornitore, rotoli[i].lotto);
        printDate(rotoli[i].data);
        printf(",\"stato\":\"%s\",\"noteAggiuntive\":\"%s\"}", rotoli[i].stato, rotoli[i].noteAggiuntive);
        if (i < nRotoli - 1) printf(",");
    }
    printf("]\n");
}

void cmdAddRotolo(char *tipo, char *colore, char *fantasia, float lunghezza, float costo, char *fornitore, char *lotto, char *stato, char *note) {
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;

    caricaRotoli(rotoli, &nRotoli);

    if (nRotoli >= MAX_ROTOLI) {
        printf("{\"status\":\"error\",\"message\":\"Limite massimo raggiunto\"}\n");
        return;
    }

    sprintf(rotoli[nRotoli].id, "R%04d", nRotoli + 1);
    strncpy(rotoli[nRotoli].tipo, tipo, MAX_CARATTERI - 1);
    strncpy(rotoli[nRotoli].colore, colore, MAX_CARATTERI - 1);
    strncpy(rotoli[nRotoli].fantasia, fantasia, MAX_CARATTERI - 1);
    rotoli[nRotoli].lunghezza_totale = lunghezza;
    rotoli[nRotoli].lunghezza_attuale = lunghezza;
    rotoli[nRotoli].costo_metro = costo;
    strncpy(rotoli[nRotoli].fornitore, fornitore, MAX_CARATTERI - 1);
    strncpy(rotoli[nRotoli].lotto, lotto, MAX_CARATTERI - 1);
    getCurrentDate(&rotoli[nRotoli].data);
    strncpy(rotoli[nRotoli].stato, stato, MAX_CARATTERI - 1);
    strncpy(rotoli[nRotoli].noteAggiuntive, note, 99);

    nRotoli++;

    if (salvaRotoli(rotoli, nRotoli) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Rotolo aggiunto con successo\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

void cmdDeleteRotolo(char *id) {
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

// ===== PRELIEVI =====
int caricaPrelievi(t_Prelievo prelievi[], int *nPrelievi) {
    FILE *f = fopen(FILE_PRELIEVI, "r");
    if (!f) {
        *nPrelievi = 0;
        return -1;
    }

    *nPrelievi = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%f|%d/%d/%d|%49[^|]|%49[^\n]\n",
                  prelievi[*nPrelievi].id,
                  prelievi[*nPrelievi].id_rotolo,
                  &prelievi[*nPrelievi].metraggio_prelevato,
                  &prelievi[*nPrelievi].data.giorno,
                  &prelievi[*nPrelievi].data.mese,
                  &prelievi[*nPrelievi].data.anno,
                  prelievi[*nPrelievi].operatore,
                  prelievi[*nPrelievi].cliente) == 8) {
        (*nPrelievi)++;
        if (*nPrelievi >= MAX_PRELIEVI) break;
    }

    fclose(f);
    return 0;
}

int salvaPrelievi(t_Prelievo prelievi[], int nPrelievi) {
    FILE *f = fopen(FILE_PRELIEVI, "w");
    if (!f) return -1;

    for (int i = 0; i < nPrelievi; i++) {
        fprintf(f, "%s|%s|%.2f|%02d/%02d/%04d|%s|%s\n",
                prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato,
                prelievi[i].data.giorno, prelievi[i].data.mese, prelievi[i].data.anno,
                prelievi[i].operatore, prelievi[i].cliente);
    }

    fclose(f);
    return 0;
}

void cmdListPrelievi() {
    t_Prelievo prelievi[MAX_PRELIEVI];
    int nPrelievi;

    caricaPrelievi(prelievi, &nPrelievi);

    printf("[");
    for (int i = 0; i < nPrelievi; i++) {
        printf("{\"id\":\"%s\",\"id_rotolo\":\"%s\",\"metraggio_prelevato\":%.2f,\"data\":",
               prelievi[i].id, prelievi[i].id_rotolo, prelievi[i].metraggio_prelevato);
        printDate(prelievi[i].data);
        printf(",\"operatore\":\"%s\",\"cliente\":\"%s\"}", prelievi[i].operatore, prelievi[i].cliente);
        if (i < nPrelievi - 1) printf(",");
    }
    printf("]\n");
}

void cmdAddPrelievo(char *id_rotolo, float metraggio, char *operatore, char *cliente) {
    t_Prelievo prelievi[MAX_PRELIEVI];
    t_Rotolo rotoli[MAX_ROTOLI];
    int nPrelievi, nRotoli;

    caricaPrelievi(prelievi, &nPrelievi);
    caricaRotoli(rotoli, &nRotoli);

    // Trova il rotolo
    int rotoloIdx = -1;
    for (int i = 0; i < nRotoli; i++) {
        if (strcmp(rotoli[i].id, id_rotolo) == 0) {
            rotoloIdx = i;
            break;
        }
    }

    if (rotoloIdx == -1) {
        printf("{\"status\":\"error\",\"message\":\"Rotolo non trovato\"}\n");
        return;
    }

    if (rotoli[rotoloIdx].lunghezza_attuale < metraggio) {
        printf("{\"status\":\"error\",\"message\":\"Metraggio insufficiente\"}\n");
        return;
    }

    if (nPrelievi >= MAX_PRELIEVI) {
        printf("{\"status\":\"error\",\"message\":\"Limite prelievi raggiunto\"}\n");
        return;
    }

    // Crea prelievo
    sprintf(prelievi[nPrelievi].id, "P%04d", nPrelievi + 1);
    strncpy(prelievi[nPrelievi].id_rotolo, id_rotolo, MAX_CARATTERI - 1);
    prelievi[nPrelievi].metraggio_prelevato = metraggio;
    getCurrentDate(&prelievi[nPrelievi].data);
    strncpy(prelievi[nPrelievi].operatore, operatore, MAX_CARATTERI - 1);
    strncpy(prelievi[nPrelievi].cliente, cliente, MAX_CARATTERI - 1);
    nPrelievi++;

    // Aggiorna rotolo
    rotoli[rotoloIdx].lunghezza_attuale -= metraggio;
    if (rotoli[rotoloIdx].lunghezza_attuale <= SOGLIA_RITAGLIO) {
        strcpy(rotoli[rotoloIdx].stato, "RITAGLIO");
    }

    if (salvaPrelievi(prelievi, nPrelievi) == 0 && salvaRotoli(rotoli, nRotoli) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Prelievo registrato\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

// ===== RITAGLI =====
int caricaRitagli(t_Ritaglio ritagli[], int *nRitagli) {
    FILE *f = fopen(FILE_RITAGLI, "r");
    if (!f) {
        *nRitagli = 0;
        return -1;
    }

    *nRitagli = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%f|%d/%d/%d\n",
                  ritagli[*nRitagli].idRitaglio,
                  ritagli[*nRitagli].id_rotolo,
                  &ritagli[*nRitagli].lunghezza,
                  &ritagli[*nRitagli].data.giorno,
                  &ritagli[*nRitagli].data.mese,
                  &ritagli[*nRitagli].data.anno) == 6) {
        (*nRitagli)++;
        if (*nRitagli >= MAX_RITAGLI) break;
    }

    fclose(f);
    return 0;
}

void cmdListRitagli() {
    t_Ritaglio ritagli[MAX_RITAGLI];
    int nRitagli;

    caricaRitagli(ritagli, &nRitagli);

    printf("[");
    for (int i = 0; i < nRitagli; i++) {
        printf("{\"idRitaglio\":\"%s\",\"id_rotolo\":\"%s\",\"lunghezza\":%.2f,\"data\":",
               ritagli[i].idRitaglio, ritagli[i].id_rotolo, ritagli[i].lunghezza);
        printDate(ritagli[i].data);
        printf("}");
        if (i < nRitagli - 1) printf(",");
    }
    printf("]\n");
}

// ===== FORNITORI =====
int caricaFornitori(t_Fornitore fornitori[], int *nFornitori) {
    FILE *f = fopen(FILE_FORNITORI, "r");
    if (!f) {
        *nFornitori = 0;
        return -1;
    }

    *nFornitori = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^\n]\n",
                  fornitori[*nFornitori].nome,
                  fornitori[*nFornitori].partita_iva,
                  fornitori[*nFornitori].indirizzo,
                  fornitori[*nFornitori].telefono,
                  fornitori[*nFornitori].email) == 5) {
        (*nFornitori)++;
        if (*nFornitori >= MAX_FORNITORI) break;
    }

    fclose(f);
    return 0;
}

int salvaFornitori(t_Fornitore fornitori[], int nFornitori) {
    FILE *f = fopen(FILE_FORNITORI, "w");
    if (!f) return -1;

    for (int i = 0; i < nFornitori; i++) {
        fprintf(f, "%s|%s|%s|%s|%s\n",
                fornitori[i].nome, fornitori[i].partita_iva, fornitori[i].indirizzo,
                fornitori[i].telefono, fornitori[i].email);
    }

    fclose(f);
    return 0;
}

void cmdListFornitori() {
    t_Fornitore fornitori[MAX_FORNITORI];
    int nFornitori;

    caricaFornitori(fornitori, &nFornitori);

    printf("[");
    for (int i = 0; i < nFornitori; i++) {
        printf("{\"nome\":\"%s\",\"partita_iva\":\"%s\",\"indirizzo\":\"%s\",\"telefono\":\"%s\",\"email\":\"%s\"}",
               fornitori[i].nome, fornitori[i].partita_iva, fornitori[i].indirizzo,
               fornitori[i].telefono, fornitori[i].email);
        if (i < nFornitori - 1) printf(",");
    }
    printf("]\n");
}

void cmdAddFornitore(char *nome, char *piva, char *indirizzo, char *telefono, char *email) {
    t_Fornitore fornitori[MAX_FORNITORI];
    int nFornitori;

    caricaFornitori(fornitori, &nFornitori);

    if (nFornitori >= MAX_FORNITORI) {
        printf("{\"status\":\"error\",\"message\":\"Limite fornitori raggiunto\"}\n");
        return;
    }

    strncpy(fornitori[nFornitori].nome, nome, MAX_CARATTERI - 1);
    strncpy(fornitori[nFornitori].partita_iva, piva, MAX_CARATTERI - 1);
    strncpy(fornitori[nFornitori].indirizzo, indirizzo, MAX_CARATTERI - 1);
    strncpy(fornitori[nFornitori].telefono, telefono, MAX_CARATTERI - 1);
    strncpy(fornitori[nFornitori].email, email, MAX_CARATTERI - 1);
    nFornitori++;

    if (salvaFornitori(fornitori, nFornitori) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Fornitore aggiunto\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

void cmdDeleteFornitore(char *nome) {
    t_Fornitore fornitori[MAX_FORNITORI];
    int nFornitori;

    caricaFornitori(fornitori, &nFornitori);

    int trovato = -1;
    for (int i = 0; i < nFornitori; i++) {
        if (strcmp(fornitori[i].nome, nome) == 0) {
            trovato = i;
            break;
        }
    }

    if (trovato == -1) {
        printf("{\"status\":\"error\",\"message\":\"Fornitore non trovato\"}\n");
        return;
    }

    for (int i = trovato; i < nFornitori - 1; i++) {
        fornitori[i] = fornitori[i + 1];
    }
    nFornitori--;

    if (salvaFornitori(fornitori, nFornitori) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Fornitore eliminato\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

// ===== PROGETTI =====
int caricaProgetti(t_Progetto progetti[], int *nProgetti) {
    FILE *f = fopen(FILE_PROGETTI, "r");
    if (!f) {
        *nProgetti = 0;
        return -1;
    }

    *nProgetti = 0;
    while (fscanf(f, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%d/%d/%d\n",
                  progetti[*nProgetti].id,
                  progetti[*nProgetti].idCliente,
                  progetti[*nProgetti].tipo_capo,
                  progetti[*nProgetti].idRotolo,
                  progetti[*nProgetti].tessuto_usato,
                  &progetti[*nProgetti].data.giorno,
                  &progetti[*nProgetti].data.mese,
                  &progetti[*nProgetti].data.anno) == 8) {
        (*nProgetti)++;
        if (*nProgetti >= MAX_PROGETTI) break;
    }

    fclose(f);
    return 0;
}

int salvaProgetti(t_Progetto progetti[], int nProgetti) {
    FILE *f = fopen(FILE_PROGETTI, "w");
    if (!f) return -1;

    for (int i = 0; i < nProgetti; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%02d/%02d/%04d\n",
                progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
                progetti[i].idRotolo, progetti[i].tessuto_usato,
                progetti[i].data.giorno, progetti[i].data.mese, progetti[i].data.anno);
    }

    fclose(f);
    return 0;
}

void cmdListProgetti() {
    t_Progetto progetti[MAX_PROGETTI];
    int nProgetti;

    caricaProgetti(progetti, &nProgetti);

    printf("[");
    for (int i = 0; i < nProgetti; i++) {
        printf("{\"id\":\"%s\",\"idCliente\":\"%s\",\"tipo_capo\":\"%s\",\"idRotolo\":\"%s\",\"tessuto_usato\":\"%s\",\"data\":",
               progetti[i].id, progetti[i].idCliente, progetti[i].tipo_capo,
               progetti[i].idRotolo, progetti[i].tessuto_usato);
        printDate(progetti[i].data);
        printf("}");
        if (i < nProgetti - 1) printf(",");
    }
    printf("]\n");
}

void cmdAddProgetto(char *idCliente, char *tipo_capo, char *idRotolo, char *tessuto) {
    t_Progetto progetti[MAX_PROGETTI];
    int nProgetti;

    caricaProgetti(progetti, &nProgetti);

    if (nProgetti >= MAX_PROGETTI) {
        printf("{\"status\":\"error\",\"message\":\"Limite progetti raggiunto\"}\n");
        return;
    }

    sprintf(progetti[nProgetti].id, "PRJ%04d", nProgetti + 1);
    strncpy(progetti[nProgetti].idCliente, idCliente, MAX_CARATTERI - 1);
    strncpy(progetti[nProgetti].tipo_capo, tipo_capo, MAX_CARATTERI - 1);
    strncpy(progetti[nProgetti].idRotolo, idRotolo, MAX_CARATTERI - 1);
    strncpy(progetti[nProgetti].tessuto_usato, tessuto, MAX_CARATTERI - 1);
    getCurrentDate(&progetti[nProgetti].data);
    nProgetti++;

    if (salvaProgetti(progetti, nProgetti) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Progetto aggiunto\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

void cmdDeleteProgetto(char *id) {
    t_Progetto progetti[MAX_PROGETTI];
    int nProgetti;

    caricaProgetti(progetti, &nProgetti);

    int trovato = -1;
    for (int i = 0; i < nProgetti; i++) {
        if (strcmp(progetti[i].id, id) == 0) {
            trovato = i;
            break;
        }
    }

    if (trovato == -1) {
        printf("{\"status\":\"error\",\"message\":\"Progetto non trovato\"}\n");
        return;
    }

    for (int i = trovato; i < nProgetti - 1; i++) {
        progetti[i] = progetti[i + 1];
    }
    nProgetti--;

    if (salvaProgetti(progetti, nProgetti) == 0) {
        printf("{\"status\":\"ok\",\"message\":\"Progetto eliminato\"}\n");
    } else {
        printf("{\"status\":\"error\",\"message\":\"Errore salvataggio\"}\n");
    }
}

// ===== INVENTARIO =====
void cmdGetInventario() {
    t_Rotolo rotoli[MAX_ROTOLI];
    int nRotoli;

    caricaRotoli(rotoli, &nRotoli);

    float valoreTotale = 0;
    float metraggioTotale = 0;
    int rotoliDisponibili = 0;

    for (int i = 0; i < nRotoli; i++) {
        valoreTotale += rotoli[i].lunghezza_attuale * rotoli[i].costo_metro;
        metraggioTotale += rotoli[i].lunghezza_attuale;
        if (strcmp(rotoli[i].stato, "DISPONIBILE") == 0 || strcmp(rotoli[i].stato, "Disponibile") == 0) {
            rotoliDisponibili++;
        }
    }

    printf("{\"numeroRotoli\":%d,\"metraggioTotale\":%.2f,\"valoreTotale\":%.2f,\"rotoliDisponibili\":%d}\n",
           nRotoli, metraggioTotale, valoreTotale, rotoliDisponibili);
}

// ===== MAIN =====
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("{\"status\":\"error\",\"message\":\"Comando mancante\"}\n");
        return 1;
    }

    char *cmd = argv[1];

    // ROTOLI
    if (strcmp(cmd, "list_rotoli") == 0) {
        cmdListRotoli();
    }
    else if (strcmp(cmd, "add_rotolo") == 0 && argc >= 10) {
        cmdAddRotolo(argv[2], argv[3], argv[4], atof(argv[5]), atof(argv[6]), argv[7], argv[8], argv[9], argc > 10 ? argv[10] : "");
    }
    else if (strcmp(cmd, "delete_rotolo") == 0 && argc == 3) {
        cmdDeleteRotolo(argv[2]);
    }
    // PRELIEVI
    else if (strcmp(cmd, "list_prelievi") == 0) {
        cmdListPrelievi();
    }
    else if (strcmp(cmd, "add_prelievo") == 0 && argc >= 5) {
        cmdAddPrelievo(argv[2], atof(argv[3]), argv[4], argc > 5 ? argv[5] : "");
    }
    // RITAGLI
    else if (strcmp(cmd, "list_ritagli") == 0) {
        cmdListRitagli();
    }
    // FORNITORI
    else if (strcmp(cmd, "list_fornitori") == 0) {
        cmdListFornitori();
    }
    else if (strcmp(cmd, "add_fornitore") == 0 && argc >= 6) {
        cmdAddFornitore(argv[2], argv[3], argv[4], argv[5], argc > 6 ? argv[6] : "");
    }
    else if (strcmp(cmd, "delete_fornitore") == 0 && argc == 3) {
        cmdDeleteFornitore(argv[2]);
    }
    // PROGETTI
    else if (strcmp(cmd, "list_progetti") == 0) {
        cmdListProgetti();
    }
    else if (strcmp(cmd, "add_progetto") == 0 && argc >= 6) {
        cmdAddProgetto(argv[2], argv[3], argv[4], argv[5]);
    }
    else if (strcmp(cmd, "delete_progetto") == 0 && argc == 3) {
        cmdDeleteProgetto(argv[2]);
    }
    // INVENTARIO
    else if (strcmp(cmd, "get_inventario") == 0) {
        cmdGetInventario();
    }
    // LEGACY (compatibilità)
    else if (strcmp(cmd, "list") == 0) {
        cmdListRotoli();
    }
    else if (strcmp(cmd, "add") == 0 && argc == 6) {
        cmdAddRotolo(argv[2], argv[3], "", atof(argv[4]), atof(argv[5]), "", "", "Disponibile", "");
    }
    else if (strcmp(cmd, "delete") == 0 && argc == 3) {
        cmdDeleteRotolo(argv[2]);
    }
    else {
        printf("{\"status\":\"error\",\"message\":\"Comando non riconosciuto: %s\"}\n", cmd);
        return 1;
    }

    return 0;
}

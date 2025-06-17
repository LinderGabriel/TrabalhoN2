#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000
#define SENSORES 5
#define MAXNOME 100

typedef enum { TIPO_INT, TIPO_BOOL, TIPO_FLOAT, TIPO_STRING } Tipo;

typedef struct {
    long tempo;
    char sensor[MAXNOME];
    Tipo tipo;
    union {
        int vi;
        int vb;
        float vf;
        char vs[17];
    } valor;
} Dado;

Dado dados[MAX];
int qtd = 0;

char sensores_encontrados[SENSORES][MAXNOME];
Tipo tipos_encontrados[SENSORES];
int qtdSensores = 0;

Tipo identifica_tipo(const char *str) {
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0) return TIPO_BOOL;
    char *end;
    long x = strtol(str, &end, 10);
    if (*end == '\0') return TIPO_INT;
    float y = strtof(str, &end);
    if (*end == '\0') return TIPO_FLOAT;
    return TIPO_STRING;
}

void copia_valor(Dado *d, const char *str) {
    switch (d->tipo) {
        case TIPO_INT: d->valor.vi = atoi(str); break;
        case TIPO_BOOL: d->valor.vb = (strcmp(str, "true") == 0); break;
        case TIPO_FLOAT: d->valor.vf = atof(str); break;
        case TIPO_STRING: strncpy(d->valor.vs, str, 16); d->valor.vs[16] = '\0'; break;
    }
}

void ordenar(Dado arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int max = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].tempo > arr[max].tempo)
                max = j;
        }
        if (max != i) {
            Dado tmp = arr[i];
            arr[i] = arr[max];
            arr[max] = tmp;
        }
    }
}

int existe(char *nome) {
    for (int i = 0; i < qtdSensores; i++) {
        if (strcmp(sensores_encontrados[i], nome) == 0)
            return i;
    }
    return -1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s arquivo\n", argv[0]);
        return 1;
    }
    FILE *arq = fopen(argv[1], "r");
    if (arq == NULL) {
        printf("Erro ao abrir\n");
        return 1;
    }

    while (1) {
        long t;
        char nome[MAXNOME];
        char valstr[64];
        if (fscanf(arq, "%ld %99s %63s", &t, nome, valstr) != 3) break;

        int idx = existe(nome);
        if (idx < 0) {
            if (qtdSensores < SENSORES) {
                strcpy(sensores_encontrados[qtdSensores], nome);
                tipos_encontrados[qtdSensores] = identifica_tipo(valstr);
                idx = qtdSensores++;
            } else continue;
        }

        dados[qtd].tempo = t;
        strcpy(dados[qtd].sensor, nome);
        dados[qtd].tipo = tipos_encontrados[idx];
        copia_valor(&dados[qtd], valstr);
        qtd++;
    }

    fclose(arq);

    for (int i = 0; i < qtdSensores; i++) {
        Dado temp[MAX];
        int c = 0;
        for (int j = 0; j < qtd; j++) {
            if (strcmp(dados[j].sensor, sensores_encontrados[i]) == 0)
                temp[c++] = dados[j];
        }

        ordenar(temp, c);

        char nomeArq[105];
        sprintf(nomeArq, "%s.txt", sensores_encontrados[i]);

        FILE *out = fopen(nomeArq, "w");
        if (!out) {
            printf("Erro ao criar %s\n", nomeArq);
            continue;
        }

        for (int k = 0; k < c; k++) {
            fprintf(out, "%ld %s ", temp[k].tempo, temp[k].sensor);
            switch (temp[k].tipo) {
                case TIPO_INT:    fprintf(out, "%d\n", temp[k].valor.vi); break;
                case TIPO_BOOL:   fprintf(out, "%s\n", temp[k].valor.vb ? "true" : "false"); break;
                case TIPO_FLOAT:  fprintf(out, "%.2f\n", temp[k].valor.vf); break;
                case TIPO_STRING: fprintf(out, "%s\n", temp[k].valor.vs); break;
            }
        }
        fclose(out);
    }

    printf("Concluido\n");
    return 0;
}
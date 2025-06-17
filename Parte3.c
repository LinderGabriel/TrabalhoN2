#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_REGISTROS 2000
#define MAX_SENSORES 4 

typedef struct {
    long tempo;
    char sensor[20];
    char tipo[10]; 
} Sensor;

int data_valida(int d, int m, int a, int h, int min, int s) {
    struct tm t;
    t.tm_year = a - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = h;
    t.tm_min = min;
    t.tm_sec = s;
    t.tm_isdst = -1;
    time_t resultado = mktime(&t);
    if (resultado == -1) return 0;
    return (t.tm_mday == d && t.tm_mon == m - 1 && t.tm_year == a - 1900 &&
            t.tm_hour == h && t.tm_min == min && t.tm_sec == s);
}

long gerar_timestamp(long inicio, long fim) {
    return inicio + rand() % (fim - inicio + 1);
}

void gerar_valor(char tipo[], char *saida) {
    if (strcmp(tipo, "CONJ_Z") == 0) {
        sprintf(saida, "%d", rand() % 1000);
    } else if (strcmp(tipo, "CONJ_Q") == 0) {
        float v = (rand() % 10000) / 100.0;
        sprintf(saida, "%.2f", v);
    } else if (strcmp(tipo, "BINARIO") == 0) {
        if (rand() % 2)
            strcpy(saida, "true");
        else
            strcpy(saida, "false");
    } else if (strcmp(tipo, "TEXTO") == 0) {
        char letras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int tam = rand() % 16 + 1;
        for (int i = 0; i < tam; i++) {
            saida[i] = letras[rand() % 26];
        }
        saida[tam] = '\0';
    }
}

int main() {
    srand(time(NULL));

    int di, mi, ai, hi, mini, si;
    int df, mf, af, hf, minf, sf;

    printf("Data e hora de inicio (dd mm aaaa hh mm ss): ");
    do {
        scanf("%d %d %d %d %d %d", &di, &mi, &ai, &hi, &mini, &si);
        if (!data_valida(di, mi, ai, hi, mini, si))
            printf("Data/hora inválida! Tente novamente: ");
    } while (!data_valida(di, mi, ai, hi, mini, si));

    printf("Data e hora de fim (dd mm aaaa hh mm ss): ");
    do {
        scanf("%d %d %d %d %d %d", &df, &mf, &af, &hf, &minf, &sf);
        if (!data_valida(df, mf, af, hf, minf, sf))
            printf("Data/hora inválida! Tente novamente: ");
    } while (!data_valida(df, mf, af, hf, minf, sf));

    struct tm ti, tf;
    ti.tm_year = ai - 1900;
    ti.tm_mon = mi - 1;
    ti.tm_mday = di;
    ti.tm_hour = hi;
    ti.tm_min = mini;
    ti.tm_sec = si;
    ti.tm_isdst = -1;

    tf.tm_year = af - 1900;
    tf.tm_mon = mf - 1;
    tf.tm_mday = df;
    tf.tm_hour = hf;
    tf.tm_min = minf;
    tf.tm_sec = sf;
    tf.tm_isdst = -1;

    long inicio = mktime(&ti);
    long fim = mktime(&tf);

    Sensor sensores[MAX_SENSORES] = {
        {0, "Sensor1", "CONJ_Z"},
        {0, "Sensor2", "CONJ_Q"},
        {0, "Sensor3", "BINARIO"},
        {0, "Sensor4", "TEXTO"}
    };

    FILE *f = fopen("leituras.txt", "w");
    if (!f) {
        printf("Erro ao criar arquivo\n");
        return 1;
    }

    for (int i = 0; i < MAX_SENSORES; i++) {
        for (int j = 0; j < TOTAL_REGISTROS; j++) {
            long t = gerar_timestamp(inicio, fim);
            char valor[50];
            gerar_valor(sensores[i].tipo, valor);
            fprintf(f, "%ld %s %s\n", t, sensores[i].sensor, valor);
        }
    }

    fclose(f);
    printf("Arquivo leituras.txt gerado com sucesso.\n");
    return 0;
}
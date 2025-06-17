#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    long tempo;
    char sensor[100];
    float valor;
} Dado;

int busca_binaria(Dado dados[], int n, long alvo) {
    int ini = 0, fim = n - 1;
    int maisPerto = 0;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;

        if (labs(dados[meio].tempo - alvo) < labs(dados[maisPerto].tempo - alvo)) {
            maisPerto = meio;
        }

        if (dados[meio].tempo > alvo)
            ini = meio + 1;
        else if (dados[meio].tempo < alvo)
            fim = meio - 1;
        else
            return meio;
    }

    return maisPerto;
}

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

int main() {
    char nomeSensor[100];
    printf("Qual sensor? ");
    scanf("%99s", nomeSensor);

    char nomeArq[120];
    sprintf(nomeArq, "%s.txt", nomeSensor);

    FILE *f = fopen(nomeArq, "r");
    if (f == NULL) {
        printf("nao achei o arquivo\n");
        return 1;
    }

    Dado d[10000];
    int qtd = 0;

    while (fscanf(f, "%ld %99s %f", &d[qtd].tempo, d[qtd].sensor, &d[qtd].valor) == 3) {
        qtd++;
    }
    fclose(f);

    int dia, mes, ano, h, m, s;
    do {
        printf("Data e hora (dd mm aaaa hh mm ss): ");
        scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &h, &m, &s);

        if (!data_valida(dia, mes, ano, h, m, s)) {
            printf("Data ou hora inválida. Tente novamente.\n");
        }
    } while (!data_valida(dia, mes, ano, h, m, s));

    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = h;
    t.tm_min = m;
    t.tm_sec = s;
    t.tm_isdst = -1;

    long alvo = mktime(&t);

    int i = busca_binaria(d, qtd, alvo);

    printf("Mais próximo:\n");
    printf("%ld %s %.2f\n", d[i].tempo, d[i].sensor, d[i].valor);

    return 0;
}
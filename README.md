﻿Neste trabalho, que dá seguimento ao trabalho feito na N1, recebeu correções e melhorias.

Sobre as correções, trata-se de arrumar o que foi apontado na N1, pelo professor, que foram:
1. Uso de qsort sem demonstrar conhecimento de algoritmo, então troquei para Selection Sort nos programas.
2. Risco de Buffer Overflow ao ler strings, então corrigi os scanf, fscanf, sscanf com limites de tamanho, usando %99s.
3. Falta de validação de data e hora, então incluí a validação de data/hora nos programas que pedem entrada do usuário usando mktime.

Sobre as melhorias, o que tem nesse trabalho que no outro não tinha:
1. Suporte a múltiplos sensores, fazendo com que gere e separe os dados por sensor.
2. Geração de 2000 registros por sensor, anteriormente sendo apenas 1.000.
3. Controle de intervalo de tempo, então agora o usuário que decide. (Programa 3 pede o intervalo)
4. Busca por data/hora usando busca eficiente, implementei a busca binária no programa 2.
5. Ordenação correta antes da busca, que é confirmada no programa 1.


Todos os três programas trabalham com uma estrutura central chamada Dado, que representa uma leitura de sensor. Ela contém:
Long tempo: Guarda o instante da leitura em formato de tempo Unix (segundos desde 1970). Isso facilita a ordenação e comparação de datas.
Char sensor[100] ou char sensor[10]: Armazena o nome do sensor (exemplos: TEMP, PRES, etc).
Float valor ou float val: Guarda o valor lido pelo sensor.

Programa 1:
Dado dados[MAX]: Vetor que armazena todas as leituras carregadas do arquivo leituras.txt.
Int qtd: Conta o número total de registros lidos.
Char sensores_encontrados[SENSORES][100]: Lista que guarda os nomes únicos dos sensores encontrados no arquivo.
Int qtdSensores: Conta quantos sensores únicos foram encontrados.

Int existe(char *nome): Função que verifica se o nome de um sensor já está na lista de sensores encontrados.
Void selectionSort(Dado *dados, int n): Função de ordenação implementada com o algoritmo Selection Sort, substituindo a antiga qsort. Ela organiza os registros de cada sensor em ordem crescente de tempo.
Main(): Função principal que:
Abre o arquivo de entrada (leituras.txt), lê os dados para o vetor, identifica os sensores únicos, separa os registros por sensor, ordena cada grupo e salva cada grupo em um arquivo separado como TEMP.txt, PRES.txt, etc.

Programa 2:
Char nomeSensor[100]: Armazena o nome do sensor que o usuário deseja consultar.
Char nomeArq[120]: Guarda o nome do arquivo que será aberto, construído com base no nome do sensor (TEMP.txt, por exemplo).
Dado d[10000]: Vetor que guarda as leituras daquele sensor.
Int qtd: Conta o número de registros lidos.
Int dia, mes, ano, h, m, s: Guardam a data e a hora digitadas pelo usuário.
Struct tm t: Estrutura padrão da linguagem C usada para montar a data/hora que o usuário informou.
Long alvo: Converte a data/hora do usuário para o formato de tempo Unix, facilitando a busca.

Int busca_binaria(Dado dados[], int n, long alvo): Realiza uma busca binária no vetor ordenado de leituras para localizar o registro mais próximo do instante alvo.
Main(): Função principal que:
Pede o nome do sensor ao usuário, abre o arquivo correspondente, lê os dados para um vetor, solicita ao usuário uma data e hora (com validação de data/hora), converte a data/hora para o formato Unix, chama a busca binária e mostra o resultado (registro mais próximo).

Programa 3:
Char *sensores[] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"}: Lista de nomes de sensores possíveis.
Dado dados[TOTAL_REGISTROS]: Vetor com os 5000 registros que serão gerados.
Int k: Contador de registros.
Int d, m, a: Guardam a data-base informada pelo usuário.

Long gerar_aleatorio(int dia, int mes, int ano): Função que cria um instante de tempo aleatório dentro daquele dia específico (usando mktime e rand).
Void embaralhar(Dado *dados, int n): Embaralha a ordem dos registros gerados para simular dados coletados em ordem aleatória.
Main(): Função principal que:
Pede a data-base ao usuário, gera 1000 registros para cada um dos 5 sensores, embaralha os registros e salva todos os registros no arquivo leituras.txt.


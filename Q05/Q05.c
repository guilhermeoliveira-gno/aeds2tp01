#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct pra guardar a data separadinha
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

// funcao que converte a string AAAA-MM-DD pra struct Data
Data parseData(char* s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// formata a data no padrao que o enunciado quer: DD/MM/YYYY
void formatData(Data d, char* buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// struct veiculo com todos os campos descritos no diagrama
typedef struct {
    int id;
    char marca[100];
    char modelo[100];
    int ano;
    char categoria[100];
    char combustivel[10][50];
    int numCombustivel;
    int cilindros;
    double cilindrada;
    char transmissao[50];
    char tracao[50];
    double consumoCidade;
    double consumoEstrada;
    double co2;
    bool turbo;
    Data dataRegistro;
} Veiculo;

// funcao auxiliar pra tirar espacos e quebras de linha das pontas
void trim(char* str) {
    int inicio = 0;
    while (str[inicio] == ' ' || str[inicio] == '\t' || str[inicio] == '\r' || str[inicio] == '\n') {
        inicio++;
    }
    if (inicio > 0) {
        int i = 0;
        while (str[inicio + i] != '\0') {
            str[i] = str[inicio + i];
            i++;
        }
        str[i] = '\0';
    }
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }
}

// formata double no estilo java
void formataDouble(char* buffer, double valor) {
    sprintf(buffer, "%lf", valor);
    char* ponto = strchr(buffer, '.');
    if (ponto != NULL) {
        char* fim = buffer + strlen(buffer) - 1;
        while (fim > ponto + 1 && *fim == '0') {
            *fim = '\0';
            fim--;
        }
    }
}

// quebra a linha do csv e preenche o registro veiculo
Veiculo parseVeiculo(char* s) {
    Veiculo v;
    char campos[15][200];
    int c = 0, p = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '\r' || s[i] == '\n') continue;
        if (s[i] == ',') {
            campos[c][p] = '\0';
            trim(campos[c]);
            c++;
            p = 0;
        } else {
            campos[c][p++] = s[i];
        }
    }
    campos[c][p] = '\0';
    trim(campos[c]);

    v.id = atoi(campos[0]);
    strcpy(v.marca, campos[1]);
    strcpy(v.modelo, campos[2]);
    v.ano = atoi(campos[3]);
    strcpy(v.categoria, campos[4]);

    // separa os combustiveis divididos por ';'
    v.numCombustivel = 0;
    char* token = strtok(campos[5], ";");
    while (token != NULL) {
        trim(token);
        strcpy(v.combustivel[v.numCombustivel++], token);
        token = strtok(NULL, ";");
    }

    v.cilindros = atoi(campos[6]);
    v.cilindrada = atof(campos[7]);
    strcpy(v.transmissao, campos[8]);
    strcpy(v.tracao, campos[9]);
    v.consumoCidade = atof(campos[10]);
    v.consumoEstrada = atof(campos[11]);
    v.co2 = atof(campos[12]);
    v.turbo = (strcmp(campos[13], "true") == 0 || strcmp(campos[13], "True") == 0 || strcmp(campos[13], "1") == 0);
    v.dataRegistro = parseData(campos[14]);

    return v;
}

// formata a saida conforme a especificacao
void formatVeiculo(Veiculo v, char* buffer) {
    char dataStr[50];
    formatData(v.dataRegistro, dataStr);

    char combStr[200] = "[";
    for (int i = 0; i < v.numCombustivel; i++) {
        if (i > 0) strcat(combStr, ", ");
        strcat(combStr, v.combustivel[i]);
    }
    strcat(combStr, "]");

    char strCilindrada[30], strCidade[30], strEstrada[30], strCo2[30];
    formataDouble(strCilindrada, v.cilindrada);
    formataDouble(strCidade, v.consumoCidade);
    formataDouble(strEstrada, v.consumoEstrada);
    formataDouble(strCo2, v.co2);

    sprintf(buffer, "[%d ## %s ## %s ## %d ## %s ## %s ## %d ## %s ## %s ## %s ## %s ## %s ## %s ## %s ## %s]",
            v.id, v.marca, v.modelo, v.ano, v.categoria, combStr, v.cilindros,
            strCilindrada, v.transmissao, v.tracao, strCidade, strEstrada, strCo2,
            v.turbo ? "true" : "false", dataStr);
}

// leitura do csv
Veiculo* lerCsv(char* caminhoArquivo, int* n) {
    FILE* f = fopen(caminhoArquivo, "r");
    if (!f) f = fopen("/tmp/veiculos.csv", "r");
    if (!f) f = fopen("veiculos.csv", "r");
    if (!f) return NULL;

    Veiculo* veiculos = (Veiculo*) malloc(1500 * sizeof(Veiculo));
    char linha[500];

    // pula cabecalho
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return NULL;
    }

    *n = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        trim(linha);
        if (strlen(linha) > 0) {
            veiculos[*n] = parseVeiculo(linha);
            (*n)++;
        }
    }

    fclose(f);
    return veiculos;
}

// ordenacao por counting sort usando o atributo cilindros como chave
void countingSort(Veiculo* v, int n) {
    if (n <= 1) return;

    // acha o maior valor de cilindros pra dimensionar o vetor de contagem
    int max = v[0].cilindros;
    for (int i = 1; i < n; i++) {
        if (v[i].cilindros > max) {
            max = v[i].cilindros;
        }
    }

    // aloca e zera o vetor de contagem
    int* count = (int*) calloc(max + 1, sizeof(int));

    // conta a frequencia de cada valor de cilindros
    for (int i = 0; i < n; i++) {
        count[v[i].cilindros]++;
    }

    // acumula as contagens pra saber as posicoes finais
    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }

    // preenche o vetor ordenado de tras pra frente pra manter a estabilidade
    Veiculo* ordenado = (Veiculo*) malloc(n * sizeof(Veiculo));
    for (int i = n - 1; i >= 0; i--) {
        ordenado[--count[v[i].cilindros]] = v[i];
    }

    // copia de volta pro vetor original
    for (int i = 0; i < n; i++) {
        v[i] = ordenado[i];
    }

    free(count);
    free(ordenado);
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    Veiculo selecionados[1500];
    int qtd = 0;

    int id;
    // le os ids da entrada ate encontrar -1
    while (scanf("%d", &id) == 1 && id != -1) {
        for (int i = 0; i < totalVeiculos; i++) {
            if (todos[i].id == id) {
                selecionados[qtd++] = todos[i];
                break;
            }
        }
    }

    // ordena por counting sort pela chave cilindros
    countingSort(selecionados, qtd);

    // imprime os veiculos ordenados
    char buffer[1000];
    for (int i = 0; i < qtd; i++) {
        formatVeiculo(selecionados[i], buffer);
        printf("%s\n", buffer);
    }

    free(todos);
    return 0;
}

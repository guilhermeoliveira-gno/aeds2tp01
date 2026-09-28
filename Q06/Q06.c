#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct da data
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

// conversao de string pra struct Data
Data parseData(char* s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// formatacao da data no padrao DD/MM/YYYY
void formatData(Data d, char* buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// struct veiculo completa
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

// funcao que limpa espacos e quebras de linha
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

// formata numero de ponto flutuante pro formato pedido
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

// preenche os dados do veiculo a partir da linha do csv
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

// formata veiculo para impressao
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

// le o csv completo
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

// counting sort auxiliar usado em cada digito pelo radixsort
void countingSortPorDigito(Veiculo* v, int n, int exp) {
    Veiculo* saida = (Veiculo*) malloc(n * sizeof(Veiculo));
    int count[10] = {0};

    // conta ocorrencias de cada digito (0 a 9)
    for (int i = 0; i < n; i++) {
        int digito = (v[i].ano / exp) % 10;
        count[digito]++;
    }

    // acumula para obter as posicoes reais
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // constroi o array de saida de tras pra frente mantendo estabilidade
    for (int i = n - 1; i >= 0; i--) {
        int digito = (v[i].ano / exp) % 10;
        saida[--count[digito]] = v[i];
    }

    // copia de volta
    for (int i = 0; i < n; i++) {
        v[i] = saida[i];
    }

    free(saida);
}

// algoritmo radix sort que ordena pelo atributo ano
void radixsort(Veiculo* v, int n) {
    if (n <= 1) return;

    // encontra o maior ano pra saber a quantidade de digitos
    int max = v[0].ano;
    for (int i = 1; i < n; i++) {
        if (v[i].ano > max) {
            max = v[i].ano;
        }
    }

    // aplica o counting sort pra cada posicao decimal: unidades, dezenas, centenas...
    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSortPorDigito(v, n, exp);
    }
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    Veiculo selecionados[1500];
    int qtd = 0;

    int id;
    // leitura dos ids ate -1
    while (scanf("%d", &id) == 1 && id != -1) {
        for (int i = 0; i < totalVeiculos; i++) {
            if (todos[i].id == id) {
                selecionados[qtd++] = todos[i];
                break;
            }
        }
    }

    // ordena por radixsort usando o ano
    radixsort(selecionados, qtd);

    // impressao dos dados ordenados
    char buffer[1000];
    for (int i = 0; i < qtd; i++) {
        formatVeiculo(selecionados[i], buffer);
        printf("%s\n", buffer);
    }

    free(todos);
    return 0;
}

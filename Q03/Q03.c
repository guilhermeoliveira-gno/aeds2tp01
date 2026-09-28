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

// formata double estilo java (sem zeros sobrando, mas mantendo .0 se for inteiro)
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

    // separa os tipos de combustivel que sao divididos por ';'
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

// formata a saida exatamente igual o padrao pedido
void formatVeiculo(Veiculo v, char* buffer) {
    char dataStr[50];
    formatData(v.dataRegistro, dataStr);

    // monta a lista de combustiveis entre colchetes
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

// le todo o csv do verde (/tmp/veiculos.csv)
Veiculo* lerCsv(char* caminhoArquivo, int* n) {
    FILE* f = fopen(caminhoArquivo, "r");
    if (!f) f = fopen("/tmp/veiculos.csv", "r");
    if (!f) f = fopen("veiculos.csv", "r");
    if (!f) return NULL;

    Veiculo* veiculos = (Veiculo*) malloc(1500 * sizeof(Veiculo));
    char linha[500];

    // pula a linha de cabecalho
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

// algoritmo classico de selecao por modelo
void selecao(Veiculo* v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            // compara os modelos alfabeticamente usando strcmp
            if (strcmp(v[j].modelo, v[menor].modelo) < 0) {
                menor = j;
            }
        }
        // troca a posicao i com o menor encontrado
        Veiculo tmp = v[i];
        v[i] = v[menor];
        v[menor] = tmp;
    }
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    // vetor pra guardar os veiculos selecionados pela entrada
    Veiculo selecionados[1500];
    int qtd = 0;

    int id;
    // le os ids da entrada ate ler -1
    while (scanf("%d", &id) == 1 && id != -1) {
        // busca sequencial no array de todos os veiculos
        for (int i = 0; i < totalVeiculos; i++) {
            if (todos[i].id == id) {
                selecionados[qtd++] = todos[i];
                break;
            }
        }
    }

    // ordena os selecionados por modelo usando selecao
    selecao(selecionados, qtd);

    // imprime os registros ordenados
    char buffer[1000];
    for (int i = 0; i < qtd; i++) {
        formatVeiculo(selecionados[i], buffer);
        printf("%s\n", buffer);
    }

    free(todos);
    return 0;
}

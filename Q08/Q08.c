#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct pra guardar a data
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

// faz o parse da data AAAA-MM-DD
Data parseData(char* s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// formata data para DD/MM/YYYY
void formatData(Data d, char* buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

// struct veiculo
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

// funcao que remove espacos e \r \n do comeco e fim da string
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

// le a linha do csv e preenche o registro
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

// le todo o csv
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

// ordenacao por selecao pelo atributo modelo (obrigatorio antes da busca binaria)
void selecao(Veiculo* v, int n) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(v[j].modelo, v[menor].modelo) < 0) {
                menor = j;
            }
        }
        Veiculo tmp = v[i];
        v[i] = v[menor];
        v[menor] = tmp;
    }
}

// algoritmo de pesquisa binaria
bool pesquisaBinaria(Veiculo* v, int n, char* modeloBuscado) {
    int esq = 0;
    int dir = n - 1;

    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        int cmp = strcmp(modeloBuscado, v[meio].modelo);

        if (cmp == 0) {
            return true; // achou!
        } else if (cmp < 0) {
            dir = meio - 1; // busca na metade esquerda
        } else {
            esq = meio + 1; // busca na metade direita
        }
    }

    return false; // nao encontrou no arranjo
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    Veiculo selecionados[1500];
    int qtd = 0;

    char linha[200];

    // parte 1: le os ids ate ler -1
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strcmp(linha, "-1") == 0) break;
        if (strlen(linha) == 0) continue;

        int id = atoi(linha);
        for (int i = 0; i < totalVeiculos; i++) {
            if (todos[i].id == id) {
                selecionados[qtd++] = todos[i];
                break;
            }
        }
    }

    // como pedido no enunciado, ordena o arranjo por selecao usando modelo
    selecao(selecionados, qtd);

    // parte 2: pesquisa binaria dos modelos ate ler FIM
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strcmp(linha, "FIM") == 0) break;
        if (strlen(linha) == 0) continue;

        // pesquisa binaria no arranjo ordenado
        if (pesquisaBinaria(selecionados, qtd, linha)) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }

    free(todos);
    return 0;
}

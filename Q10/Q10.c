#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILA 5

// struct da data
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

// converte string para data
Data parseData(char* s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

// formata data DD/MM/YYYY
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

// tira espacos das pontas
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

// formata double estilo java
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

// le linha do csv
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

// formata veiculo
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

// le todo o csv
Veiculo* lerCsv(char* caminhoArquivo, int* n) {
    FILE* f = fopen(caminhoArquivo, "r");
    if (!f) f = fopen("/tmp/veiculos.csv", "r");
    if (!f) f = fopen("veiculos.csv", "r");
    if (!f) return NULL;

    Veiculo* veiculos = (Veiculo*) malloc(1500 * sizeof(Veiculo));
    char linha[500];

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

// busca por id no array geral
Veiculo acharPorId(Veiculo* todos, int total, int id) {
    for (int i = 0; i < total; i++) {
        if (todos[i].id == id) {
            return todos[i];
        }
    }
    Veiculo vazio;
    vazio.id = -1;
    return vazio;
}

// struct da fila circular de tamanho fixo 5
typedef struct {
    Veiculo array[MAX_FILA];
    int primeiro;
    int ultimo;
    int count;
} Fila;

void inicializarFila(Fila* f) {
    f->primeiro = 0;
    f->ultimo = 0;
    f->count = 0;
}

// remove elemento da fila circular (desenfileirar)
Veiculo removerFila(Fila* f) {
    if (f->count == 0) {
        Veiculo vazio;
        vazio.id = -1;
        return vazio;
    }
    Veiculo resp = f->array[f->primeiro];
    f->primeiro = (f->primeiro + 1) % MAX_FILA;
    f->count--;
    return resp;
}

// insere elemento na fila circular (enfileirar)
// se estiver cheia (5 elementos), remove o mais antigo antes de inserir!
void inserirFila(Fila* f, Veiculo v) {
    if (f->count == MAX_FILA) {
        Veiculo rem = removerFila(f);
        printf("(R) %s %s\n", rem.marca, rem.modelo);
    }
    f->array[f->ultimo] = v;
    f->ultimo = (f->ultimo + 1) % MAX_FILA;
    f->count++;
}

// mostra os veiculos da fila do primeiro ao ultimo
void mostrarFila(Fila* f) {
    char buffer[1000];
    int idx = f->primeiro;
    for (int i = 0; i < f->count; i++) {
        formatVeiculo(f->array[idx], buffer);
        printf("%s\n", buffer);
        idx = (idx + 1) % MAX_FILA;
    }
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    Fila fila;
    inicializarFila(&fila);

    char linha[200];

    // parte 1: carrega os ids iniciais ate -1
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strcmp(linha, "-1") == 0) break;
        if (strlen(linha) == 0) continue;

        int id = atoi(linha);
        Veiculo v = acharPorId(todos, totalVeiculos, id);
        if (v.id != -1) {
            inserirFila(&fila, v);
        }
    }

    // parte 2: comandos de enfileirar e desenfileirar
    if (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strlen(linha) > 0) {
            int qtdComandos = atoi(linha);

            for (int i = 0; i < qtdComandos; i++) {
                if (fgets(linha, sizeof(linha), stdin) == NULL) break;
                trim(linha);
                if (strlen(linha) == 0) continue;

                if (linha[0] == 'I') {
                    // comando I id
                    int id = atoi(linha + 2);
                    Veiculo v = acharPorId(todos, totalVeiculos, id);
                    if (v.id != -1) {
                        inserirFila(&fila, v);
                    }
                } else if (linha[0] == 'R') {
                    // comando R
                    Veiculo rem = removerFila(&fila);
                    if (rem.id != -1) {
                        printf("(R) %s %s\n", rem.marca, rem.modelo);
                    }
                }
            }
        }
    }

    // mostra a fila do primeiro ao ultimo
    mostrarFila(&fila);

    free(todos);
    return 0;
}

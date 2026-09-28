#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct pra armazenar a data
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

// formata a data no padrao DD/MM/YYYY
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

// retira espacos e quebras de linha
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

// le registro do veiculo da linha do csv
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

// celula da lista simplesmente encadeada
typedef struct Celula {
    Veiculo elemento;
    struct Celula* prox;
} Celula;

// cria nova celula
Celula* novaCelula(Veiculo elemento) {
    Celula* nova = (Celula*) malloc(sizeof(Celula));
    nova->elemento = elemento;
    nova->prox = NULL;
    return nova;
}

// estrutura da lista com celula cabeca
typedef struct {
    Celula* primeiro;
    Celula* ultimo;
    int tamanho;
} Lista;

void inicializarLista(Lista* l) {
    Veiculo vazio;
    vazio.id = -1;
    l->primeiro = novaCelula(vazio);
    l->ultimo = l->primeiro;
    l->tamanho = 0;
}

// insere no inicio da lista
void inserirInicio(Lista* l, Veiculo v) {
    Celula* tmp = novaCelula(v);
    tmp->prox = l->primeiro->prox;
    l->primeiro->prox = tmp;
    if (l->primeiro == l->ultimo) {
        l->ultimo = tmp;
    }
    l->tamanho++;
}

// insere no fim da lista
void inserirFim(Lista* l, Veiculo v) {
    l->ultimo->prox = novaCelula(v);
    l->ultimo = l->ultimo->prox;
    l->tamanho++;
}

// insere na posicao informada
void inserir(Lista* l, Veiculo v, int pos) {
    if (pos < 0 || pos > l->tamanho) return;

    if (pos == 0) {
        inserirInicio(l, v);
    } else if (pos == l->tamanho) {
        inserirFim(l, v);
    } else {
        Celula* i = l->primeiro;
        for (int j = 0; j < pos; j++, i = i->prox);
        Celula* tmp = novaCelula(v);
        tmp->prox = i->prox;
        i->prox = tmp;
        l->tamanho++;
    }
}

// remove do inicio da lista
Veiculo removerInicio(Lista* l) {
    if (l->primeiro == l->ultimo) {
        Veiculo vazio;
        vazio.id = -1;
        return vazio;
    }
    Celula* tmp = l->primeiro->prox;
    Veiculo resp = tmp->elemento;
    l->primeiro->prox = tmp->prox;
    if (l->primeiro->prox == NULL) {
        l->ultimo = l->primeiro;
    }
    free(tmp);
    l->tamanho--;
    return resp;
}

// remove do fim da lista
Veiculo removerFim(Lista* l) {
    if (l->primeiro == l->ultimo) {
        Veiculo vazio;
        vazio.id = -1;
        return vazio;
    }
    Celula* i = l->primeiro;
    while (i->prox != l->ultimo) {
        i = i->prox;
    }
    Veiculo resp = l->ultimo->elemento;
    free(l->ultimo);
    l->ultimo = i;
    l->ultimo->prox = NULL;
    l->tamanho--;
    return resp;
}

// remove de uma posicao especifica
Veiculo remover(Lista* l, int pos) {
    if (l->primeiro == l->ultimo || pos < 0 || pos >= l->tamanho) {
        Veiculo vazio;
        vazio.id = -1;
        return vazio;
    }
    if (pos == 0) return removerInicio(l);
    if (pos == l->tamanho - 1) return removerFim(l);

    Celula* i = l->primeiro;
    for (int j = 0; j < pos; j++, i = i->prox);

    Celula* tmp = i->prox;
    Veiculo resp = tmp->elemento;
    i->prox = tmp->prox;
    free(tmp);
    l->tamanho--;
    return resp;
}

// mostra todos os elementos da lista do primeiro ao ultimo
void mostrarLista(Lista* l) {
    char buffer[1000];
    for (Celula* i = l->primeiro->prox; i != NULL; i = i->prox) {
        formatVeiculo(i->elemento, buffer);
        printf("%s\n", buffer);
    }
}

int main() {
    int totalVeiculos = 0;
    Veiculo* todos = lerCsv("/tmp/veiculos.csv", &totalVeiculos);

    Lista lista;
    inicializarLista(&lista);

    char linha[200];

    // parte 1: carrega os ids iniciais ate -1
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strcmp(linha, "-1") == 0) break;
        if (strlen(linha) == 0) continue;

        int id = atoi(linha);
        Veiculo v = acharPorId(todos, totalVeiculos, id);
        if (v.id != -1) {
            inserirFim(&lista, v);
        }
    }

    // parte 2: executa a quantidade n de comandos
    if (fgets(linha, sizeof(linha), stdin) != NULL) {
        trim(linha);
        if (strlen(linha) > 0) {
            int qtdComandos = atoi(linha);

            for (int i = 0; i < qtdComandos; i++) {
                if (fgets(linha, sizeof(linha), stdin) == NULL) break;
                trim(linha);
                if (strlen(linha) == 0) continue;

                char cmd[10];
                sscanf(linha, "%s", cmd);

                if (strcmp(cmd, "II") == 0) {
                    int id;
                    sscanf(linha, "%*s %d", &id);
                    Veiculo v = acharPorId(todos, totalVeiculos, id);
                    if (v.id != -1) inserirInicio(&lista, v);
                } else if (strcmp(cmd, "I*") == 0) {
                    int pos, id;
                    sscanf(linha, "%*s %d %d", &pos, &id);
                    Veiculo v = acharPorId(todos, totalVeiculos, id);
                    if (v.id != -1) inserir(&lista, v, pos);
                } else if (strcmp(cmd, "IF") == 0) {
                    int id;
                    sscanf(linha, "%*s %d", &id);
                    Veiculo v = acharPorId(todos, totalVeiculos, id);
                    if (v.id != -1) inserirFim(&lista, v);
                } else if (strcmp(cmd, "RI") == 0) {
                    Veiculo rem = removerInicio(&lista);
                    if (rem.id != -1) {
                        printf("(R) %s %s\n", rem.marca, rem.modelo);
                    }
                } else if (strcmp(cmd, "R*") == 0) {
                    int pos;
                    sscanf(linha, "%*s %d", &pos);
                    Veiculo rem = remover(&lista, pos);
                    if (rem.id != -1) {
                        printf("(R) %s %s\n", rem.marca, rem.modelo);
                    }
                } else if (strcmp(cmd, "RF") == 0) {
                    Veiculo rem = removerFim(&lista);
                    if (rem.id != -1) {
                        printf("(R) %s %s\n", rem.marca, rem.modelo);
                    }
                }
            }
        }
    }

    // mostra a lista final do primeiro ao ultimo
    mostrarLista(&lista);

    free(todos);
    return 0;
}

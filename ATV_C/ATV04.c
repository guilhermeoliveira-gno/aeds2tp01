#include <stdio.h>
#include <string.h>

int ehFim(char palavra[]) { // verifica se a entrada e FIM
    int tamanho = 0;

    while (palavra[tamanho] != '\0' && palavra[tamanho] != '\n') {
        tamanho++; // conta quantos caracteres tem, sem o enter
    }

    if (tamanho != 3) {
        return 0; // so pode ser FIM se tiver 3 letras
    }

    return palavra[0] == 'F' &&
           palavra[1] == 'I' &&
           palavra[2] == 'M'; // verifica se e exatamente FIM
}

void inverter(char string[], int tamanho, char resultado[]) { // funcao que recebe a palavra e devolve ela ao contrario

    for (int i = 0; i < tamanho; i++) {
        resultado[i] = string[tamanho - 1 - i]; // vai pegando as letras do fim pro inicio
    }
    resultado[tamanho] = '\0'; // marca o fim da palavra
}

int main() { //t

    char entrada[1000];
    char invertida[1000];

    while (fgets(entrada, sizeof(entrada), stdin) != NULL) { // fica lendo as palavras ate acabar a entrada
        int tamanho = 0;

        while (entrada[tamanho] != '\0' && entrada[tamanho] != '\n') {
            tamanho++; // conta quantas letras tem, sem contar o enter do final
        }

        if (ehFim(entrada)) {
            break; // para o loop se o usuario digitar FIM
        }

        inverter(entrada, tamanho, invertida); // chama a funcao pra inverter e salva o resultado

        printf("%s\n", invertida); // mostra a palavra invertida na tela
    }

    return 0;
}

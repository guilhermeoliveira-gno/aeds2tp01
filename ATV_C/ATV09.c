#include <stdio.h>

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

int main() {

    char entrada[1000];

    while (fgets(entrada, sizeof(entrada), stdin) != NULL) { // fica lendo as entradas ate acabar

        if (ehFim(entrada)) {
            break; // para o loop se o usuario digitar FIM
        }
        
    }

    return 0;
}
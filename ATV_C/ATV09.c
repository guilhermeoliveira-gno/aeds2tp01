#include <stdio.h>

int ehFim(char palavra[]) {
    int tamanho = 0;

    while (palavra[tamanho] != '\0') {
        tamanho++;
    }

    if (tamanho != 3) {
        return 0;
    }

    return palavra[0] == 'F' &&
           palavra[1] == 'I' &&
           palavra[2] == 'M';
}

int main() {

    helloWorld(); // chama a funcao

    return 0;
}
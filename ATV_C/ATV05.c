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

int somaDigitos(int numero) { // metodo recursivo que soma os digitos do numero

    if (numero < 0) {
        numero = -numero; // garante que funciona com negativos tambem
    }

    if (numero < 10) {
        return numero; // caso base: numero de um digito, retorna ele mesmo
    }

    return (numero % 10) + somaDigitos(numero / 10); // soma o ultimo digito com a chamada recursiva do restante
}

int main() {

    char entrada[1000];

    while (fgets(entrada, sizeof(entrada), stdin) != NULL) { // fica lendo as entradas ate o usuario digitar FIM

        if (ehFim(entrada)) {
            break; // para o loop se o usuario digitar FIM
        }

        int numero = 0;
        int i = 0;
        int negativo = 0;

        if (entrada[0] == '-') { // verifica se e negativo
            negativo = 1;
            i = 1;
        }

        while (entrada[i] >= '0' && entrada[i] <= '9') {
            numero = numero * 10 + (entrada[i] - '0'); // converte a string para inteiro
            i++;
        }

        if (negativo) {
            numero = -numero;
        }

        printf("%d\n", somaDigitos(numero)); // chama a funcao recursiva e imprime o resultado
    }

    return 0;
}

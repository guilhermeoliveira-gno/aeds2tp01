import java.util.Scanner;

public class ATV02 {

    public static boolean somenteVogais(String palavra) { // verifica se sao somente vogais

        if (palavra.length() == 0) {
            return false; // checa se esta vazia
        }

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i); // pega o caractere atual

            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
                c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
                return false; // verifica se tem algum que nao seja vogal
            }
        }

        return true;
    }

    public static boolean somenteConsoantes(String palavra) { // verifica se s'ao somente consoantes

        if (palavra.length() == 0) {
            return false; // se esta vazia
        }

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);

            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                return false; // verifica se e uma letra
            }

            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                return false; // verifica se e uma vogal
            }
        }

        return true; // se passou por tudo retorna que sao so consoantes
    }

    public static boolean somenteInteiro(String palavra) { // verifica se e um numero inteiro

        if (palavra.length() == 0) {
            return false;
        }

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);

            if (c < '0' || c > '9') {
                return false; // verifica se o caractere e um numero
            }
        }

        return true; // se passou por tudo retorna que s'ao so inteiros
    }

    public static boolean somenteReal(String palavra) { // verifica se e um numero real

    if (palavra.length() == 0) {
        return false;  // se esta vazio
    }

    int virgulas = 0;

    for (int i = 0; i < palavra.length(); i++) {
        char c = palavra.charAt(i);

        if (c == ',') {
            virgulas++; // conta a quantidade de virgulas

            if (virgulas > 1) {
                return false; // nao pode ter mais de uma virgula
            }
        } else if (c < '0' || c > '9') {
            return false; // se nao for numero, retorna falso
        }
    }

    return virgulas == 1; // caso sim retorna true, caso nao retorna falso 
}

    public static boolean ehFim(String palavra) { // verifica se a entrada e FIM

        if (palavra.length() != 3) {
            return false; // verifica se sao so 3 algarismos
        }

        if (palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M') { 
            return true; // verifica se e fim
        }

        return false;
    }

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        while (scanner.hasNextLine()) { // le todas as linhas

            String palavra = scanner.nextLine();

            if (ehFim(palavra)) {
                break;
            }

            boolean vogal = somenteVogais(palavra);
            boolean consoante = somenteConsoantes(palavra);
            boolean inteiro = somenteInteiro(palavra);
            boolean real = somenteReal(palavra);

            if (vogal) {
                System.out.print("SIM ");
            } else {
                System.out.print("NAO ");
            }

            if (consoante) {
                System.out.print("SIM ");
            } else {
                System.out.print("NAO ");                    // RETORNO DAS FUNCAO
            }

            if (inteiro) {
                System.out.print("SIM ");
            } else {
                System.out.print("NAO ");
            }

            if (real) {
                System.out.println("SIM");
            } else {
                System.out.println("NAO");
            }
        }

        scanner.close();
    }
}
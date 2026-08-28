import java.util.Scanner;

public class ATV03 {

    public static boolean isVogal(char c) { // metodo auxiliar pra ver se a letra e uma vogal
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
    }

    public static boolean isLetra(char c) { // metodo auxiliar pra saber se e uma letra valida do alfabeto
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    public static boolean somenteVogais(String palavra) { // checa se a palavra e feita so de vogais
        if (palavra.length() == 0) return false;
        
        for (int i = 0; i < palavra.length(); i++) {
            if (!isVogal(palavra.charAt(i))) {
                return false;
            }
        }
        return true;
    }

    public static boolean somenteConsoantes(String palavra) { // checa se a palavra so tem consoantes
        if (palavra.length() == 0) return false;
        
        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);
            if (!isLetra(c) || isVogal(c)) {
                return false;
            }
        }
        return true;
    }

    public static boolean somenteInteiro(String palavra) { // checa se digitaram um numero inteiro
        if (palavra.length() == 0) return false;
        
        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }

    public static boolean somenteReal(String palavra) { // checa se e um numero quebrado/real
        if (palavra.length() == 0) return false;
        
        int separadores = 0;
        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);
            
            if (c == ',' || c == '.') {
                separadores++;
                if (separadores > 1) return false; // barra se tiver mais de uma virgula ou ponto
            } else if (c < '0' || c > '9') {
                return false; 
            }
        }
        return true;
    }

    public static boolean ehFim(String palavra) { // ve se a pessoa digitou a palavra FIM pra parar o loop
        return palavra.length() == 3 && palavra.charAt(0) == 'F' && 
               palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M';
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while (sc.hasNextLine()) {
            String palavra = sc.nextLine();

            if (ehFim(palavra)) {
                break;
            }

            if (palavra.length() == 0) { // pula caso a linha venha em branco
                continue;
            }

            boolean vogal = somenteVogais(palavra);
            boolean consoante = somenteConsoantes(palavra);
            boolean inteiro = somenteInteiro(palavra);
            boolean real = somenteReal(palavra);

            // imprime SIM ou NAO pra cada teste com um espaco entre eles
            System.out.print(vogal ? "SIM " : "NAO ");
            System.out.print(consoante ? "SIM " : "NAO ");
            System.out.print(inteiro ? "SIM " : "NAO ");
            System.out.println(real ? "SIM" : "NAO");
        }

        sc.close();
    }
}
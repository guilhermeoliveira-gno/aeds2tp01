import java.util.Scanner;

public class ATV11 {

    
    public static boolean ehFim(String palavra) { // verifica se a entrada e FIM

        if (palavra.length() != 3) {
            return false; // verifica se sao so 3 caracteres
        }

        return palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M'; // verifica se e exatamente FIM
    }


    public static String inverter(String palavra, int i) { // metodo recursivo para inverter a string

        
        if (i == palavra.length()) {
            return ""; // verifica nulo = fim 
        }
     
        String resultado = inverter(palavra, i + 1); // chama o metodo novamente passando para a proxima posicao
        resultado = resultado + palavra.charAt(i); // adiciona o caractere ao final do resultado

        return resultado; 
    }


    public static void main(String[] args) {

    Scanner sc = new Scanner(System.in);

    while (sc.hasNextLine()) { // continua enquanto houver linha 
        String palavra = sc.nextLine(); // le a palavra digitada

        if (ehFim(palavra)) {
            break; // para o loop se o usuario digitar FIM
        }

        System.out.println(inverter(palavra, 0)); // chama o metodo para inverter e mostra o resultado
    }

    sc.close(); 
    }
}
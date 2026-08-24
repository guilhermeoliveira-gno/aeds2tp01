import java.util.Scanner;

public class ATV11 {

    
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
        System.out.println(inverter(palavra, 0)); // chama o metodo para inverter e mostra o resultado
    }

    sc.close(); 
    }
}
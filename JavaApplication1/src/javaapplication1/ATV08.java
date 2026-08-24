import java.util.Scanner;

public class ATV08 {

    
    public static boolean ehFim(String senha) { // verifica se a entrada e FIM

        if (senha.length() != 3) {
            return false; // verifica se sao so 3 caracteres
        }

        return senha.charAt(0) == 'F' && senha.charAt(1) == 'I' && senha.charAt(2) == 'M'; // verifica se e exatamente FIM
    }


    public static boolean senhaValida(String senha) { // verifica se a senha e valida

        
        if (senha.length() < 8) {
            return false;  // senha precisa ter pelo menos 8 caracteres
        }
   
        boolean maiuscula = false;
        boolean minuscula = false;
        boolean numero = false;
        boolean especial = false;

        
        for (int i = 0; i < senha.length(); i++) {
            char caractere = senha.charAt(i);

            if (caractere >= 'A' && caractere <= 'Z') {
                maiuscula = true; // encontrou uma letra maiuscula
            } else if (caractere >= 'a' && caractere <= 'z') {
                minuscula = true; // encontrou uma letra minuscula
            } else if (caractere >= '0' && caractere <= '9') {
                numero = true; // encontrou um numero
            } else {
                especial = true; // se nao for letra ou numero, e especial
            }
        }

        
        
        return maiuscula && minuscula && numero && especial; // retorna o valor de todas, pela logica
    }


    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while (sc.hasNextLine()) {

            String senha = sc.nextLine();

            if (ehFim(senha)) {
                break; // para o loop se o usuario digitar FIM
            }

            if (senhaValida(senha)) {
                System.out.println("SIM");           // chama a funcao e printa na tela o resultado
            } else {
                System.out.println("NAO");
            }
        }

        sc.close();
    }
}
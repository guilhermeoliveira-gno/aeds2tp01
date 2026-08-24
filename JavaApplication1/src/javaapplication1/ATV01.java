import java.util.Scanner;

public class ATV01 {

    
    public static boolean ehFim(String mensagem) {  // metodo pra verificar se a palavra 'e fim

        
        if (mensagem.length() != 3) {
            return false;   // verifica sse s'ao so tres caracteres, pra nao ser apenas uma mensagem que tenha escrito fim nela
        }

       
        if (mensagem.charAt(0) == 'F' && mensagem.charAt(1) == 'I' && mensagem.charAt(2) == 'M') { 

            return true;   // VERIFICA TODOS OS CARACTERES se estao como fim 
        }

        
        return false;   
    }

   
    public static String cifrar(String mensagem) { // Método responsável por realizar a Cifra de César
        String resultado = "";

        for (int i = 0; i < mensagem.length(); i++) {
            char caractere = mensagem.charAt(i); // pega o caractere da posi'cao que est'a rodando
            caractere = (char) (caractere + 3); // incrementa 3 ao caractere recebido            
            resultado = resultado + caractere; //adiciona  no resultado o caractere obtido
        }

        
        return resultado; // retorna o resultado obtido
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        while (scanner.hasNextLine()) {            
            String mensagem = scanner.nextLine();    // ler todas as linhas at'e onde tenha
           
            if (ehFim(mensagem)) {                                            
                break;                      //verifica se e fim e para se for
            }           
            System.out.println(cifrar(mensagem)); // chama o metodo e imprime o retorno 
        }      
        scanner.close();
    }
}
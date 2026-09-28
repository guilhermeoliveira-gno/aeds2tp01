import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

// classe Data
class Data {
    private int dia, mes, ano;

    public Data() {}

    public Data(int dia, int mes, int ano) {
        this.dia = dia;
        this.mes = mes;
        this.ano = ano;
    }

    public int getDia() { return dia; }
    public int getMes() { return mes; }
    public int getAno() { return ano; }

    public static Data parseData(String s) {
        String[] p = s.split("-");
        return new Data(Integer.parseInt(p[2]), Integer.parseInt(p[1]), Integer.parseInt(p[0]));
    }

    public String format() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }
}

// classe Veiculo
class Veiculo {
    private int id;
    private String marca;
    private String modelo;
    private int ano;
    private String categoria;
    private String[] combustivel;
    private int cilindros;
    private double cilindrada;
    private String transmissao;
    private String tracao;
    private double consumoCidade;
    private double consumoEstrada;
    private double co2;
    private boolean turbo;
    private Data dataRegistro;

    public Veiculo() {}

    public int getId() { return id; }
    public String getMarca() { return marca; }
    public String getModelo() { return modelo; }
    public int getAno() { return ano; }
    public String getCategoria() { return categoria; }
    public String[] getCombustivel() { return combustivel; }
    public int getCilindros() { return cilindros; }
    public double getCilindrada() { return cilindrada; }
    public String getTransmissao() { return transmissao; }
    public String getTracao() { return tracao; }
    public double getConsumoCidade() { return consumoCidade; }
    public double getConsumoEstrada() { return consumoEstrada; }
    public double getCo2() { return co2; }
    public boolean getTurbo() { return turbo; }
    public Data getDataRegistro() { return dataRegistro; }

    public static Veiculo parseVeiculo(String s) {
        Veiculo v = new Veiculo();
        String[] c = s.split(",");
        v.id = Integer.parseInt(c[0].trim());
        v.marca = c[1].trim();
        v.modelo = c[2].trim();
        v.ano = Integer.parseInt(c[3].trim());
        v.categoria = c[4].trim();
        v.combustivel = c[5].trim().split(";");
        v.cilindros = Integer.parseInt(c[6].trim());
        v.cilindrada = Double.parseDouble(c[7].trim());
        v.transmissao = c[8].trim();
        v.tracao = c[9].trim();
        v.consumoCidade = Double.parseDouble(c[10].trim());
        v.consumoEstrada = Double.parseDouble(c[11].trim());
        v.co2 = Double.parseDouble(c[12].trim());
        v.turbo = Boolean.parseBoolean(c[13].trim());
        v.dataRegistro = Data.parseData(c[14].trim());
        return v;
    }

    public String format() {
        StringBuilder comb = new StringBuilder("[");
        for (int i = 0; i < combustivel.length; i++) {
            if (i > 0) comb.append(", ");
            comb.append(combustivel[i].trim());
        }
        comb.append("]");

        return "[" + id + " ## " + marca + " ## " + modelo + " ## " + ano + " ## " +
               categoria + " ## " + comb + " ## " + cilindros + " ## " + cilindrada + " ## " +
               transmissao + " ## " + tracao + " ## " + consumoCidade + " ## " + consumoEstrada +
               " ## " + co2 + " ## " + turbo + " ## " + dataRegistro.format() + "]";
    }
}

// leitor do arquivo de veiculos
class LeitorCsv {
    public static Veiculo[] ler(String caminhoArquivo) {
        Veiculo[] veiculos = new Veiculo[1500];
        int n = 0;
        try {
            BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo));
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                if (!linha.trim().isEmpty()) {
                    veiculos[n++] = Veiculo.parseVeiculo(linha);
                }
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Veiculo[] resultado = new Veiculo[n];
        for (int i = 0; i < n; i++) resultado[i] = veiculos[i];
        return resultado;
    }
}

// no da pilha encadeada
class Celula {
    public Veiculo elemento;
    public Celula prox;

    public Celula(Veiculo elemento) {
        this.elemento = elemento;
        this.prox = null;
    }
}

// implementacao da pilha dinamica
class Pilha {
    private Celula topo;

    public Pilha() {
        this.topo = null;
    }

    // empilha elemento no topo da pilha (comando I)
    public void inserir(Veiculo v) {
        Celula tmp = new Celula(v);
        tmp.prox = topo;
        topo = tmp;
    }

    // desempilha elemento do topo da pilha (comando R)
    public Veiculo remover() {
        if (topo == null) return null;

        Veiculo resp = topo.elemento;
        topo = topo.prox;
        return resp;
    }

    // mostra todos os elementos a partir do topo ate a base
    public void mostrar() {
        for (Celula i = topo; i != null; i = i.prox) {
            System.out.println(i.elemento.format());
        }
    }
}

// questao 12 - pilha flexivel
public class Q12 {

    // busca o veiculo por id no array geral
    public static Veiculo acharPorId(Veiculo[] todos, int id) {
        for (int i = 0; i < todos.length; i++) {
            if (todos[i].getId() == id) {
                return todos[i];
            }
        }
        return null;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Veiculo[] todos = LeitorCsv.ler("/tmp/veiculos.csv");

        Pilha pilha = new Pilha();

        // parte 1: carrega os ids iniciais empilhando ate -1
        while (sc.hasNextLine()) {
            String linha = sc.nextLine().trim();
            if (linha.equals("-1")) break;

            int id = Integer.parseInt(linha);
            Veiculo v = acharPorId(todos, id);
            if (v != null) {
                pilha.inserir(v);
            }
        }

        // parte 2: comandos de empilhar e desempilhar
        if (sc.hasNextLine()) {
            String qtdLinha = sc.nextLine().trim();
            if (!qtdLinha.isEmpty()) {
                int qtdComandos = Integer.parseInt(qtdLinha);

                for (int i = 0; i < qtdComandos; i++) {
                    if (!sc.hasNextLine()) break;
                    String linha = sc.nextLine().trim();
                    if (linha.isEmpty()) continue;

                    String[] partes = linha.split(" ");
                    String cmd = partes[0];

                    if (cmd.equals("I")) {
                        int id = Integer.parseInt(partes[1]);
                        Veiculo v = acharPorId(todos, id);
                        if (v != null) {
                            pilha.inserir(v);
                        }
                    } else if (cmd.equals("R")) {
                        Veiculo rem = pilha.remover();
                        if (rem != null) {
                            System.out.println("(R) " + rem.getMarca() + " " + rem.getModelo());
                        }
                    }
                }
            }
        }

        // mostra os elementos da pilha a partir do topo
        pilha.mostrar();

        sc.close();
    }
}

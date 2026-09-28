import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

// classe pra manipular data
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

// classe veiculo encapsulada
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
            br.readLine(); // pula cabecalho
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

// questao 07 - bucketsort com 10 baldes e insercao em cada um
public class Q07 {

    // ordenacao por insercao usada dentro de cada balde
    public static void insercao(Veiculo[] balde, int n) {
        for (int i = 1; i < n; i++) {
            Veiculo tmp = balde[i];
            int j = i - 1;

            // ordena comparando a cilindrada
            while (j >= 0 && balde[j].getCilindrada() > tmp.getCilindrada()) {
                balde[j + 1] = balde[j];
                j--;
            }
            balde[j + 1] = tmp;
        }
    }

    // algoritmo bucketsort
    public static void bucketSort(Veiculo[] array, int n) {
        if (n <= 1) return;

        int numBaldes = 10;
        Veiculo[][] baldes = new Veiculo[numBaldes][n];
        int[] tamBaldes = new int[numBaldes];

        // distribui os veiculos nos 10 baldes usando a cilindrada normalizada por 8.1
        for (int i = 0; i < n; i++) {
            double normalizado = array[i].getCilindrada() / 8.1;
            int baldeIdx = (int) (normalizado * numBaldes);

            // previne estouro de indice caso o valor seja exatamente 8.1
            if (baldeIdx >= numBaldes) baldeIdx = numBaldes - 1;
            if (baldeIdx < 0) baldeIdx = 0;

            baldes[baldeIdx][tamBaldes[baldeIdx]++] = array[i];
        }

        // ordena cada balde individualmente com insercao e junta tudo
        int k = 0;
        for (int b = 0; b < numBaldes; b++) {
            insercao(baldes[b], tamBaldes[b]);
            for (int i = 0; i < tamBaldes[b]; i++) {
                array[k++] = baldes[b][i];
            }
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        // carrega todos os veiculos
        Veiculo[] todos = LeitorCsv.ler("/tmp/veiculos.csv");

        Veiculo[] selecionados = new Veiculo[1500];
        int qtd = 0;

        // leitura dos ids ate -1
        while (sc.hasNextLine()) {
            String linha = sc.nextLine().trim();
            if (linha.equals("-1")) break;

            int id = Integer.parseInt(linha);

            for (int i = 0; i < todos.length; i++) {
                if (todos[i].getId() == id) {
                    selecionados[qtd++] = todos[i];
                    break;
                }
            }
        }

        // ordena usando bucketsort
        bucketSort(selecionados, qtd);

        // imprime ordenado
        for (int i = 0; i < qtd; i++) {
            System.out.println(selecionados[i].format());
        }

        sc.close();
    }
}

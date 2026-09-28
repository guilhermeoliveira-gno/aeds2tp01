import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

// classe data - guarda dia, mes e ano separados pra poder formatar do jeito certo
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

    // recebe a string "AAAA-MM-DD" e transforma num objeto Data
    public static Data parseData(String s) {
        String[] p = s.split("-");
        return new Data(Integer.parseInt(p[2]), Integer.parseInt(p[1]), Integer.parseInt(p[0]));
    }

    // formata no padrao DD/MM/YYYY que o enunciado pede
    public String format() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }
}

// classe veiculo - cada objeto desse guarda todas as infos de um carro do csv
class Veiculo {
    // tudo privado por causa do encapsulamento que o prof cobra
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

    // getters de tudo - encapsulamento basico
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

    // pega uma linha do csv e transforma num objeto Veiculo
    public static Veiculo parseVeiculo(String s) {
        Veiculo v = new Veiculo();
        String[] c = s.split(",");
        v.id = Integer.parseInt(c[0].trim());
        v.marca = c[1].trim();
        v.modelo = c[2].trim();
        v.ano = Integer.parseInt(c[3].trim());
        v.categoria = c[4].trim();
        // combustivel pode ter mais de um tipo separado por ;
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

    // monta a string de saida no formato que o enunciado pede
    public String format() {
        // junta os tipos de combustivel entre colchetes
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

// classe que le o csv inteiro e monta um array de veiculos
class LeitorCsv {
    public static Veiculo[] ler(String caminhoArquivo) {
        Veiculo[] veiculos = new Veiculo[1000];
        int n = 0;
        try {
            BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo));
            br.readLine(); // pula o cabecalho do csv
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

// questao 01 - le os ids da entrada e faz pesquisa sequencial no csv
public class Q01 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        // carrega todos os veiculos do csv de uma vez
        Veiculo[] veiculos = LeitorCsv.ler("/tmp/veiculos.csv");

        // le cada id da entrada ate encontrar -1
        while (sc.hasNextLine()) {
            String linha = sc.nextLine().trim();
            if (linha.equals("-1")) break;

            int id = Integer.parseInt(linha);

            // pesquisa sequencial - vai percorrendo o array ate achar o id
            for (int i = 0; i < veiculos.length; i++) {
                if (veiculos[i].getId() == id) {
                    System.out.println(veiculos[i].format());
                    break;
                }
            }
        }
        sc.close();
    }
}

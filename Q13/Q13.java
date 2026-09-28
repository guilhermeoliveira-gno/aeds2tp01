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
    double cilindrada;
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

// leitor do csv
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

// no com dois ponteiros para a lista duplamente encadeada
class CelulaDupla {
    public Veiculo elemento;
    public CelulaDupla ant;
    public CelulaDupla prox;

    public CelulaDupla() {
        this(null);
    }

    public CelulaDupla(Veiculo elemento) {
        this.elemento = elemento;
        this.ant = null;
        this.prox = null;
    }
}

// lista duplamente encadeada flexivel com no cabeca
class ListaDupla {
    private CelulaDupla primeiro;
    private CelulaDupla ultimo;
    private int tamanho;

    public ListaDupla() {
        this.primeiro = new CelulaDupla();
        this.ultimo = this.primeiro;
        this.tamanho = 0;
    }

    // insere no comeco da lista dupla
    public void inserirInicio(Veiculo v) {
        CelulaDupla tmp = new CelulaDupla(v);
        tmp.ant = primeiro;
        tmp.prox = primeiro.prox;
        primeiro.prox = tmp;

        if (primeiro == ultimo) {
            ultimo = tmp;
        } else {
            tmp.prox.ant = tmp;
        }
        tamanho++;
    }

    // insere no fim da lista dupla
    public void inserirFim(Veiculo v) {
        ultimo.prox = new CelulaDupla(v);
        ultimo.prox.ant = ultimo;
        ultimo = ultimo.prox;
        tamanho++;
    }

    // insere na posicao informada
    public void inserir(Veiculo v, int pos) {
        if (pos < 0 || pos > tamanho) return;

        if (pos == 0) {
            inserirInicio(v);
        } else if (pos == tamanho) {
            inserirFim(v);
        } else {
            CelulaDupla i = primeiro;
            for (int j = 0; j < pos; j++, i = i.prox);

            CelulaDupla tmp = new CelulaDupla(v);
            tmp.ant = i;
            tmp.prox = i.prox;
            tmp.ant.prox = tmp.prox.ant = tmp;
            tamanho++;
        }
    }

    // remove o primeiro registro da lista
    public Veiculo removerInicio() {
        if (primeiro == ultimo) return null;

        CelulaDupla tmp = primeiro.prox;
        Veiculo resp = tmp.elemento;
        primeiro.prox = tmp.prox;

        if (primeiro.prox == null) {
            ultimo = primeiro;
        } else {
            primeiro.prox.ant = primeiro;
        }

        tmp.prox = tmp.ant = null;
        tamanho--;
        return resp;
    }

    // remove o ultimo registro da lista
    public Veiculo removerFim() {
        if (primeiro == ultimo) return null;

        Veiculo resp = ultimo.elemento;
        ultimo = ultimo.ant;
        ultimo.prox.ant = null;
        ultimo.prox = null;
        tamanho--;
        return resp;
    }

    // remove da posicao especificada
    public Veiculo remover(int pos) {
        if (primeiro == ultimo || pos < 0 || pos >= tamanho) return null;

        if (pos == 0) return removerInicio();
        if (pos == tamanho - 1) return removerFim();

        CelulaDupla i = primeiro.prox;
        for (int j = 0; j < pos; j++, i = i.prox);

        i.ant.prox = i.prox;
        i.prox.ant = i.ant;
        Veiculo resp = i.elemento;
        i.prox = i.ant = null;
        tamanho--;
        return resp;
    }

    // mostra os elementos da lista do primeiro ao ultimo
    public void mostrar() {
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            System.out.println(i.elemento.format());
        }
    }
}

// questao 13 - lista dupla flexivel
public class Q13 {

    // busca no array carregado do csv
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

        ListaDupla lista = new ListaDupla();

        // parte 1: carrega os ids iniciais ate -1
        while (sc.hasNextLine()) {
            String linha = sc.nextLine().trim();
            if (linha.equals("-1")) break;

            int id = Integer.parseInt(linha);
            Veiculo v = acharPorId(todos, id);
            if (v != null) {
                lista.inserirFim(v);
            }
        }

        // parte 2: processa comandos
        if (sc.hasNextLine()) {
            String qtdLinha = sc.nextLine().trim();
            if (!qtdLinha.isEmpty()) {
                int qtdComandos = Integer.parseInt(qtdLinha);

                for (int i = 0; i < qtdComandos; i++) {
                    if (!sc.hasNextLine()) break;
                    String linha = sc.nextLine().trim();
                    if (linha.isEmpty()) continue;

                    String[] partes = linha.split(" ");
                    String comando = partes[0];

                    if (comando.equals("II")) {
                        int id = Integer.parseInt(partes[1]);
                        lista.inserirInicio(acharPorId(todos, id));
                    } else if (comando.equals("I*")) {
                        int pos = Integer.parseInt(partes[1]);
                        int id = Integer.parseInt(partes[2]);
                        lista.inserir(acharPorId(todos, id), pos);
                    } else if (comando.equals("IF")) {
                        int id = Integer.parseInt(partes[1]);
                        lista.inserirFim(acharPorId(todos, id));
                    } else if (comando.equals("RI")) {
                        Veiculo rem = lista.removerInicio();
                        if (rem != null) {
                            System.out.println("(R) " + rem.getMarca() + " " + rem.getModelo());
                        }
                    } else if (comando.equals("R*")) {
                        int pos = Integer.parseInt(partes[1]);
                        Veiculo rem = lista.remover(pos);
                        if (rem != null) {
                            System.out.println("(R) " + rem.getMarca() + " " + rem.getModelo());
                        }
                    } else if (comando.equals("RF")) {
                        Veiculo rem = lista.removerFim();
                        if (rem != null) {
                            System.out.println("(R) " + rem.getMarca() + " " + rem.getModelo());
                        }
                    }
                }
            }
        }

        // mostra a lista do primeiro ao ultimo
        lista.mostrar();

        sc.close();
    }
}

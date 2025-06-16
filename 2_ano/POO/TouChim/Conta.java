import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

public class Conta implements Serializable {
    private int codigo;
    private List<Dispositivo> dispositivos;
    private TreeSet<Cliente>ListaClientes;
    private ArrayList<Comunicacao> comunicacoes;

    public Conta(){
        this.codigo = 0;
    }
    
    //construtor
    public Conta(int codigo) {
        this.codigo = codigo;
        this.ListaClientes = new TreeSet<Cliente>();
        this.dispositivos = new ArrayList<>();
        this.comunicacoes = new ArrayList<>();
        
    }

    public Conta(Conta c){
        this.codigo = c.codigo;
    }
    //metodos
    public int getCodigo() {
        return codigo;
    }

    public TreeSet<Cliente> getListaDeClientes() {
        TreeSet<Cliente> novo = new TreeSet<>();
        for (Cliente c : this.ListaClientes)
            novo.add(c);
        return novo;
    }

    public List<Dispositivo> getDispositivos() {
        return dispositivos;
    }

    public void addDispositivo(Dispositivo dispositivo) {
        this.dispositivos.add(dispositivo);
    }
    public void remove(Dispositivo dispositivo){
        this.dispositivos.remove(dispositivo);
    }

    public void addComunicacao(Comunicacao comunicacao) {
        this.comunicacoes.add(comunicacao);
    }

    public List<Comunicacao> getComunicacoes() {
        return this.comunicacoes;
    }

    private static final double IVA = 0.23;


    public double calcularCusto(Comunicacao comunicacao) {
        // Lógica para calcular o custo da comunicação
        // Isso pode depender do tipo de comunicação, duração, tamanho, etc.
        // Substitua esta lógica pelo que faz sentido no seu sistema.
        double custoComunicacao = 0.0;

        // Exemplo de cálculo simplificado
        if (comunicacao instanceof Chamada_Normal) {
            custoComunicacao = ((Chamada_Normal) comunicacao).getDuracao() * 0.1; // Custo por minuto
        } else if (comunicacao instanceof SMS) {
            custoComunicacao = 0.05; // Custo fixo por SMS
        } else if (comunicacao instanceof Msg_Imagem) {
            custoComunicacao = 1.0; // Custo fixo por mensagem de imagem
        } // Adicione mais casos conforme necessário

        return custoComunicacao;
    }
    public double calcularTotalComunicacoes() {
        double total = 0.0;

        for (Comunicacao comunicacao : this.comunicacoes) {
            total += comunicacao.calcularCusto(); // Suponha que há um método calcularCusto() em Comunicacao
        }
        return total;
    }

    public double calcularIVA() {
        return IVA * calcularTotalComunicacoes();
    }

    // Método para calcular o valor total a ser faturado (com IVA)
    public double calcularTotalFatura() {
        return calcularTotalComunicacoes() + calcularIVA();
    }

    // Método para gerar uma fatura discriminada por tipo de comunicação
    public void gerarFatura() {
        System.out.println("-------- FATURA --------");
        System.out.println("Conta: " + this.getCodigo());
        System.out.println("Detalhes da Fatura:");

        for (Comunicacao comunicacao : this.comunicacoes) {
            System.out.println("Tipo: " + comunicacao.getTipo() + " - Valor: " + comunicacao.calcularCusto());
        }

        System.out.println("Total Comunicações: " + calcularTotalComunicacoes());
        System.out.println("IVA: " + calcularIVA());
        System.out.println("Total Fatura: " + calcularTotalFatura());
        System.out.println("------------------------");
    }

    public boolean equals(Object o){
        if(o!= null){
            if(this.getClass() == o.getClass()){
                Conta c = (Conta) o;
                if(this.getCodigo() == c.getCodigo()){
                    return true;
                }
            }
        }
        return false;
    }

    public String toString(){
        return "Codigo: " + this.codigo;
    }
//FAZER CLONE
    public Conta clone() {
        return new Conta(this);
    }

}


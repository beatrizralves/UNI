import java.util.ArrayList;
import java.io.Serializable;

public class Cliente implements Serializable {
    private int codigo;
    private String nome;
    private ArrayList<Conta> contasCliente;
    private ArrayList<Comunicacao> comunicacoes;

    public Cliente(){
        this.codigo = 0;
        this.nome = "";
    }

    //construtor
    public Cliente(int codigo, String nome) {
        this.codigo = codigo;
        this.nome = nome;
        this.contasCliente = new ArrayList<>();
        this.comunicacoes = new ArrayList<>();
    }

    public Cliente(Cliente c){
        this.codigo = c.getCodigo();
        this.nome = c.getNome();
    }

    //metodos
    public int getCodigo() {
        return codigo;
    }

    public void setCodigo(int codigo){
        this.codigo = codigo;
    }
    public String getNome() {
        return nome;
    }
    public void setNome(String nome) {
        this.nome = nome;
    }

    public ArrayList<Conta> getContaDoCliente(){
        ArrayList<Conta> temp = new ArrayList<>();
        for (Conta c: this.contasCliente){
            temp.add(c.clone());
        }
        return temp;
    }

    public void showContasDoCliente(){
        for(Conta conta: this.contasCliente){
            System.out.println(conta.getCodigo());
        }
    }

    public void addConta(Conta conta) {
        if (contasCliente != null) {
            contasCliente.add(conta);
            System.out.println("Conta adicionada ao cliente " + this.nome);
        } else {
            contasCliente = new ArrayList<>(); // Inicializa a lista se for nula
            contasCliente.add(conta);
            System.out.println("Conta adicionada ao cliente " + this.nome + " após inicialização da lista");
        }
    }
    public Conta getContaById(int contaId) {
        for (Conta conta : contasCliente) {
            if (conta.getCodigo() == contaId) {
                return conta;
            }
        }
        return null;
    }
    public void enviarComunicacao(Cliente destinatario, Comunicacao comunicacao) {
        // Verificar se os clientes têm contas em comum
        Conta contaComum = encontrarContaComum(destinatario);

        if (contaComum != null) {
            // Adicionar a comunicação à conta comum
            contaComum.addComunicacao(comunicacao);
            System.out.println("Comunicação enviada com sucesso!");
            apresentarFatura(contaComum, comunicacao);
        } else {
            // Se não houver conta comum, tentar enviar para uma conta do destinatário
            if (destinatario.getContas().size() > 0) {
                Conta contaDestinatario = destinatario.getContas().get(0); // Use a primeira conta do destinatário
                contaDestinatario.addComunicacao(comunicacao);
                System.out.println("Comunicação enviada com sucesso para a conta do destinatário!");
                apresentarFatura(contaDestinatario, comunicacao);
            } else {
                System.out.println("Os clientes não têm contas em comum e o destinatário não possui contas para enviar a comunicação.");
            }
        }
    }

    private void apresentarFatura(Conta conta, Comunicacao comunicacao) {
        double totalFatura = conta.calcularTotalFatura(); // Exemplo de taxa de IVA de 23%

        System.out.println("********** FATURA **********");
        System.out.println("Cliente: " + this.getNome());
        System.out.println("Conta: " + conta.getCodigo());
        System.out.println("Comunicação Enviada:");
        System.out.println("Tipo: " + comunicacao.getTipo());
        System.out.println("Valor Total: " + totalFatura + " (com IVA)");
        System.out.println("****************************");
    }

    private Conta encontrarContaComum(Cliente outroCliente) {
        // Lógica para encontrar uma conta comum entre dois clientes
        for (Conta minhaConta : this.contasCliente) {
            for (Conta contaOutroCliente : outroCliente.getContas()) {
                if (minhaConta.equals(contaOutroCliente)) {
                    return minhaConta;
                }
            }
        }
        return null; // Retorna null se não houver contas em comum
    }
    public ArrayList<Conta> getContas() {
        return new ArrayList<>(contasCliente);
    }

    public boolean equals(Object obj){
        if(obj == null || this.getClass() != obj.getClass())
            return false;
        Cliente c = (Cliente) obj;

        return this.getCodigo() == c.getCodigo();

    }
    /*public void addComunicacao(Comunicacao comunicacao) {
        this.comunicacoes.add(comunicacao);
    }*/

    public ArrayList<Comunicacao> getComunicacoes() {
        ArrayList<Comunicacao> todasComunicacoes = new ArrayList<>();

        // Adicione as comunicações de todas as contas do cliente
        for (Conta conta : this.contasCliente) {
            todasComunicacoes.addAll(conta.getComunicacoes());
        }

        return todasComunicacoes;
    }

    public String toString(){
        return "codigo: " + this.codigo + "nome: " + this.nome;
    }

    public Cliente clone(){
        return new Cliente(this);
    }


}

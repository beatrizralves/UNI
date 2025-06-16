import java.io.Serializable;

public class Chamada_Normal extends Comunicacao implements Serializable
{
    private double duracao;
    
    public Chamada_Normal(int id, String tipo, int numeroOrigem, int numeroDestino, double duracao){
        super(id, tipo, numeroOrigem, numeroDestino);
        this.duracao = duracao;
    }

    public double getDuracao(){
        return duracao;
    }

    public double calcularCusto() {
        return duracao * 0.1;
    }

    public Chamada_Normal clone(){
        Chamada_Normal n = new Chamada_Normal(super.getId(), super.getTipo(), super.getNumeroOrigem(), super.getNumeroDestino(), this.duracao);
        return n;
    }
}

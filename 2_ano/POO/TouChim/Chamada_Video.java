import java.io.Serializable;

public class Chamada_Video extends Comunicacao implements Serializable
{
    
    private double duracao;
    private String resolucao;
    
    public Chamada_Video(int id, String tipo, int numeroOrigem, int numeroDestino, double duracao, String resolucao){
        super(id, tipo, numeroOrigem, numeroDestino);
        this.duracao = duracao;
        this.resolucao = resolucao;
    
    }

    public double getDuracao(){
        return duracao;
    }
    public String getResolucao(){
        return resolucao;
    }

    public double calcularCusto() {
        return duracao * 0.2;
    }

    public Chamada_Video clone(){
        Chamada_Video cv = new Chamada_Video(super.getId(), super.getTipo(), super.getNumeroOrigem(), super.getNumeroDestino(), this.duracao, this.resolucao);
        return cv;
    }
    
   
}

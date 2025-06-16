import java.io.Serializable;

public class Download extends Comunicacao implements Serializable
{
    private int tamanho_programa;
    private double duracao;
    
    public Download(int id, String tipo, int numeroOrigem, int numeroDestino, int tamanho_programa, double duracao){
        super(id, tipo, numeroOrigem, numeroDestino);
        this.tamanho_programa = tamanho_programa;
        this.duracao = duracao;
    
    }

    public int getTamanho_programa(){
        return tamanho_programa;
    }
    public double getDuracao(){
        return duracao;
    }
    public double calcularCusto() {

        return tamanho_programa * duracao;
    }

    public Download clone(){
        Download d = new Download(super.getId(), super.getTipo(), super.getNumeroOrigem(), super.getNumeroDestino(), this.tamanho_programa, this.duracao);
        return d;
    }
}

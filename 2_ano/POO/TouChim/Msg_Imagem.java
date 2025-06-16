import java.io.Serializable;

public class Msg_Imagem extends Comunicacao implements Serializable
{
    private int tamanhoimg;
    private String formato;
    private String resolucao;
    
    public Msg_Imagem(int id, int numeroOrigem, int numeroDestino, String tipo, int tamanhoimg,String formato,String resolucao){
        super(id, tipo, numeroOrigem, numeroDestino);
        this.tamanhoimg = tamanhoimg;
        this.formato = formato;
        this.resolucao = resolucao;
    }

    public int getTamanhoimg(){
        return tamanhoimg;
    }
    public String getFormato(){
        return formato;
    }
    public String getResolucao(){
        return resolucao;
    }

    public double calcularCusto() {
        return tamanhoimg * 0.05;
    }

    public Msg_Imagem clone(){
        Msg_Imagem m = new Msg_Imagem(super.getId(), super.getNumeroOrigem(), super.getNumeroDestino(), super.getTipo(), this.tamanhoimg, this.formato, this.resolucao);
        return m;
    }
}

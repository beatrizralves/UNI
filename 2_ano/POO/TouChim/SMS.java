import java.io.Serializable;

public class SMS extends Comunicacao implements Serializable {

    private int tamanhoMensagem;
    private String texto;

    public SMS(int id, String tipo, int numeroOrigem, int numeroDestino, int tamanhoMensagem, String texto){
        super(id, tipo, numeroOrigem, numeroDestino);
        this.tamanhoMensagem = tamanhoMensagem;
        this.texto = texto;
    }

    public int getTamanhoMensagem(){
        return tamanhoMensagem;
    }

    public double calcularCusto() {
        return tamanhoMensagem * 0.1;
    }
    public String getTexto(){
        return texto;
    }

    public SMS clone(){
        SMS s = new SMS(super.getId(), super.getTipo(), super.getNumeroOrigem(), super.getNumeroDestino(), this.tamanhoMensagem, this.texto);
        return s;
    }



}

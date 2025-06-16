import java.io.Serializable;

public class Msg_Video extends Comunicacao implements Serializable {
    private double duracao;
    private String formato;
    private String resolucao;
    
    public Msg_Video(int id, int numeroOrigem, int numeroDestino, String tipo, double duracao, String formato, String resolucao) {
        super(id, tipo, numeroOrigem, numeroDestino);
        this.duracao = duracao;
        this.formato = formato;
        this.resolucao = resolucao;
    }

    public double getDuracao() {
        return duracao;
    }

    public String getFormato() {
        return formato;
    }

    public String getResolucao() {
        return resolucao;
    }

    public double calcularCusto() {
        return duracao * 0.15;
    }

    public Msg_Video clone() {
        return new Msg_Video(getId(), getNumeroOrigem(), getNumeroDestino(), getTipo(), this.duracao, this.formato, this.resolucao);
    }
}
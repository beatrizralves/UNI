import java.io.Serializable;

public abstract class Comunicacao implements Serializable {
    private int id;
    private String tipo;
    private int numeroOrigem;
    private int numeroDestino;


    public Comunicacao(){
        this.id = 0;
        this.tipo = "";
        this.numeroOrigem = 0;
        this.numeroDestino = 0;
    }

    public Comunicacao(int id, String tipo, int numeroOrigem, int numeroDestino) {
        this.id = id;
        this.tipo = tipo;
        this.numeroOrigem = numeroOrigem;
        this.numeroDestino = numeroDestino;
    }

    public int getId(){
        return id;
    }
    public String getTipo() {
        return tipo;
    }

    public int getNumeroOrigem() {
        return numeroOrigem;
    }

    public int getNumeroDestino() {
        return numeroDestino;
    }
    public abstract double calcularCusto();

    public boolean equals (Object o){
        if(o!= null){
            if(this.getClass()==o.getClass()){
                Comunicacao c = (Comunicacao) o;
                if(this.getId()==c.getId()){
                    return true;
                }
            }
        }
        return false;

    }

    public abstract Comunicacao clone();
}


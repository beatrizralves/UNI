import java.io.Serializable;
import java.util.HashMap;

public class Tablet extends Dispositivo implements Serializable {
    private double capacidadeMemoria;
    private HashMap<Integer,String>lista_contatos;

    public Tablet(){
        super();
    }

    public Tablet(int numero, String modoFuncionamento, double capacidadeMemoria) {
        super(numero, modoFuncionamento);
        this.capacidadeMemoria = capacidadeMemoria;
        lista_contatos = new HashMap<>();
    }

    public Tablet(Tablet t){
        super(t);
    }
    public double getCapacidadememoria(){
     return capacidadeMemoria;
    }
    public HashMap<Integer, String> getListaContatos() {
        return lista_contatos;
    }

    public Tablet clone(){
        Tablet temp = new Tablet(super.getNumero(), super.getModoFuncionamento(), this.capacidadeMemoria);
        temp.lista_contatos = new HashMap<>(this.lista_contatos);
        return temp;
    }
}
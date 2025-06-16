import java.io.Serializable;
import java.util.HashMap;

public class Telemovel extends Dispositivo implements Serializable {
    
    private HashMap<Integer,String>lista_contatos;
    public Telemovel(){
        super();
    }

    public Telemovel(int numero, String modoFuncionamento) {
        super(numero, modoFuncionamento);
        lista_contatos = new HashMap<>();
    }
    public Telemovel(Telemovel t){
        super(t);
    }


    public HashMap<Integer, String> getListaContatos() {
        return lista_contatos;
    }

    public Telemovel clone(){
        Telemovel temp = new Telemovel(super.getNumero(), super.getModoFuncionamento());
        temp.lista_contatos = new HashMap<>(this.lista_contatos);

        return temp;
    }
    
}


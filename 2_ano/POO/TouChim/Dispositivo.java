import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

// Classe base para Dispositivo
public abstract class Dispositivo implements Serializable {
    private int numero;
    private String modoFuncionamento; // Pode ser "4G" ou "5G"
    private TreeSet<Conta>ListaContas;

    public Dispositivo(){
        this.numero = 0;
        this.modoFuncionamento = "";
    }
    
    //construtor
    public Dispositivo(int numero, String modoFuncionamento) {
        this.numero = numero;
        this.modoFuncionamento = modoFuncionamento;
        this.ListaContas = new TreeSet<Conta>();
    }

    public Dispositivo(Dispositivo a){
        this.numero = a.numero;
        this.modoFuncionamento = a.modoFuncionamento;
    }
    // metodos

    public int getNumero() {
        return numero;
    }

    public String getModoFuncionamento() {
        return modoFuncionamento;
    }


    public TreeSet<Conta> getListaContas() {
        TreeSet<Conta> novo = new TreeSet<>();
        if(this.ListaContas != null) {
            for (Conta c : this.ListaContas)
                novo.add(c);
        }
        return novo;
    }

    public boolean equals (Object o){
        if(o!= null){
            if(this.getClass()==o.getClass()){
                Dispositivo d = (Dispositivo) o;
                if(this.getNumero()== d.getNumero()){
                    return true;
                }
                
            }
        }
        return false;
    }


    public abstract Dispositivo clone();
}



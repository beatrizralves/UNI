import java.io.Serializable;

public class Oldie extends Dispositivo implements Serializable {
    public Oldie(int numero, String modoFuncionamento) {
        super(numero, modoFuncionamento);
    }
    public Oldie clone(){
        Oldie o = new Oldie(super.getNumero(), super.getModoFuncionamento());
        return o;
    }
}
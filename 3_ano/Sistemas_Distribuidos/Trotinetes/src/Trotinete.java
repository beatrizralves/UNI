import java.time.Duration;
import java.time.LocalTime;

/* -----------------------------------------------------------------------------------------------*/
                                        /* TROTINETES */
/*Métodos--> custoViagem*/
public class Trotinete {
    /* Variáveis */
    boolean livre;
    Local coordenadas;
    private LocalTime tempoViagem;

    /* Construtor */
    public Trotinete(Local coordenadas) {
        livre = true;
        this.coordenadas = coordenadas;
        tempoViagem = null;
    }

    /* Métodos */
    public boolean isLivre() {
        return livre;
    }

    public void reservaTrotinete() {
        livre = false;
        tempoViagem = LocalTime.now();
    }

    public void estacionaTrotinete(Local destino) {
        livre = true;
        coordenadas = destino;
        tempoViagem = null;
    }

    public double custoViagem(Local destino) {  /* cálculo do custo */
        int distance = Local.norma(coordenadas, destino);/* distância entre dois locais feitas atraves da distância Manhattan */
        long minutes = Duration.between(tempoViagem, LocalTime.now()).toMinutes();/* tempo da viagem */
        return distance + 0.2 * minutes;
    }
}
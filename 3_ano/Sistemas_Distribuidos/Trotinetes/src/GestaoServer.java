import java.util.*;
import java.util.concurrent.locks.ReentrantLock;

/* -----------------------------------------------------------------------------------------------------*/
                                            /* GestaoServer*/
/*Metodos-->loginUtilizador;registaUtilizador;listatrotinetes;reservaTrotinetes;estacionaTrotinetes:exit*/

public class GestaoServer {


    /* Variaveis */
    public static final int TamanhoMapa = 15;/* mapa 15*15 */
    public static final int NumTrotinetes = 20;/* número máximo de trotinetes */
    public static final int Raio = 2;/* distância fixa D (e.g., D = 2)--enunciado */

    private static List<Trotinete> trotinetesLista; /* lista para guardar as Trotinetes*/
    private static HashMap<Integer, Trotinete> trotinetesReservadas;/* mapa que guarda o código da reserva com a trotinete  */
    private int codeReserva = 0; /* variável para o id da reserva*/
    private static HashMap<String, Utilizador> UtilizadoresRegistados; /* lista que guarda as informações dos useres registados*/

    private ReentrantLock l = new ReentrantLock(); /* criação de lock */

    /*  Construtor */
    public GestaoServer() {
        trotinetesLista = new ArrayList<>();
        Random random = new Random();
        for (int i = 0; i< NumTrotinetes; i++){/* criação de coordenadas aleatórias */
            int x = random.nextInt(TamanhoMapa);/* x random de 0 a 15 */
            int y = random.nextInt(TamanhoMapa);/* y random de 0 a 15*/
            trotinetesLista.add(new Trotinete(new Local(x,y)));/* adiciona então a lista de trotinetes as coordenadas randoms*/
        }
        trotinetesReservadas = new HashMap<>();
        UtilizadoresRegistados = new HashMap<>();
    }

    /* metodos*/
    public void loginUtilizador(String nomeUtilizador, String password) throws Exception {/* verificação da opcao "login" */
        try {
            l.lock();
            Utilizador u = UtilizadoresRegistados.get(nomeUtilizador);/* vai "buscar" o nome do utilizador fornecido ao mapa "UtilizadoresRegistados" */
            /* Vai agora verificar se esse nome nao existe, se a 'pass.' não é a correta ou se o utilizador se encontra loggado*/
            if (u == null || !u.getPassword().equals(password) || u.Loggado) throw new Exception("Parâmetros inválidos, tente novamente!!!");
            u.login();
        } finally {
            l.unlock();
        }
    }
    public void registaUtilizador(String nomeUtilizador, String password) throws Exception {/* adiciona novos utilizadores no mapa */
        try {
            l.lock();
            /* se já estiver em uso o nome de utilizador mostra erro */
            if (UtilizadoresRegistados.containsKey(nomeUtilizador)) throw new Exception("Nome de utilizador indisponível!!!");
            /* se não, adiciona o novo utilizador ao mapa */
            UtilizadoresRegistados.put(nomeUtilizador, new Utilizador(nomeUtilizador, password));
        } finally {
            l.unlock();
        }
    }

    public List<Local> listatrotinetes(Local loc) {/* listagem das trotinetes*/
        try {
            l.lock();

            List<Local> trotinetes_list = new ArrayList<>();

            for (Trotinete t : trotinetesLista){/* percorrer a lista */
                /* verifica as trotinetes que estão livres no raio que definimos */
                if (Local.norma(t.coordenadas,loc) <= Raio && t.isLivre()){
                    trotinetes_list.add(t.coordenadas);
                }
            }
            /* retorna as trotinetes que estão livres ao redor do local dado pelo utilizador */
            return trotinetes_list;
        } finally {
            l.unlock();
        }
    }

    public String reservaTrotinetes(Local l, String username) {/* reserva das trotinetes*/
        Trotinete troti = null;
        Local loc = null;
        int minDistance = Integer.MAX_VALUE;

        try {

            this.l.lock();

            for (Trotinete t : trotinetesLista){/* percorre a lista de trotinetes*/
                if (Local.norma(t.coordenadas,l) <= Raio && t.isLivre()){/* trotinetes livres no raio*/
                    int distance = Local.norma(l, t.coordenadas);/* distância manhattan*/
                    if (distance < minDistance) {   //verifica a trotinete mais perto do local indicado
                        minDistance = distance;
                        troti = t;
                        loc = t.coordenadas;
                    }
                }
            }

            if (troti == null) return "ERRO!!!";
            troti.reservaTrotinete();

            int codeReserva;
            codeReserva = this.codeReserva;
            this.codeReserva++;/* incremento do código de reserva*/
            trotinetesReservadas.put(codeReserva, troti);/* adicionamos então o código e a trotinete ao mapa*/

            try {
                this.l.lock();
                UtilizadoresRegistados.get(username).addCodigo(codeReserva);/* adiciona ao cliente em questão o seu código de reserva*/

            } finally {
                this.l.unlock();
            }
            return loc + " Código: " + codeReserva; 
        } finally {
            this.l.unlock();
        }
    }

    public double estacionaTrotinetes(Local destino, String username, int id) throws Exception {/* entrega das trotinetes*/
        try {
            l.lock();
            if (!trotinetesReservadas.containsKey(id)) /* verifica se o código existe mesmo numa reserva, se não emite o erro*/
                throw new Exception("Código não reservado");
            if (destino.x < 0 || destino.x > TamanhoMapa || destino.y < 0 || destino.y > TamanhoMapa)/* verifica se as coordenadas pertencem [0,15]*/
                throw new Exception("Zona fora do mapa");
            if (!UtilizadoresRegistados.get(username).containsCodigo(id))/* verifica se o utilizador tem esse 'id' de reserva, se não emite erro*/
                throw new Exception("Utilizador não reservou esta trotinete");

            Trotinete t = trotinetesReservadas.get(id);

            double custo = t.custoViagem(destino);/* calculamos o custa através do metodo que implementamos*/

            trotinetesReservadas.remove(id);/* removemos das trotinetes reservadas o código da reserva e dos utilizadores registados*/
            t.estacionaTrotinete(destino);

            UtilizadoresRegistados.get(username).removeCodigo(id);

            return custo;/* retorna o custo da viagem */
        }
        finally {
            l.unlock();
        }
    }

    public void exit(String nomeUtilizador) {/* saída */
        try {
            l.lock();
            /* percorre o mapa e remove todas as trotinetes que estavam reservadas coloca as agora como livres depois e invocado o logoff do cliente*/
            for (int i : UtilizadoresRegistados.get(nomeUtilizador).getCodreservados()) {
                trotinetesReservadas.get(i).livre = true;
                trotinetesReservadas.remove(i);
                UtilizadoresRegistados.get(nomeUtilizador).logoff();
            }
        } finally {
            l.unlock();
        }
    }
}
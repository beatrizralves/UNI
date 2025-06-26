import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;

public class Server {
    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(8000);/*Porta para ligaçao com o Cliente */
        GestaoServer gestor = new GestaoServer();

        while (true) {
            Socket socket = serverSocket.accept();
            Thread s = new Thread(new Servidor(socket, gestor));
            s.start();
        }
    }
}
class Servidor implements Runnable {

    /* Variáveis */
    private final Socket socket;
    private final GestaoServer gestor;

    /* Construtor */
    public Servidor(Socket socket, GestaoServer gestor) {
        this.socket = socket;
        this.gestor = gestor;
    }

    public void run() {
        try {
            DataInputStream in = new DataInputStream(socket.getInputStream());
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            boolean login = false;
            while (!login) {
                String opcao = in.readUTF();
                String username = in.readUTF();
                String password = in.readUTF();
                if (opcao.equals("1")) {
                    try {
                        gestor.loginUtilizador(username, password);
                        out.writeBoolean(login = true);
                        out.writeUTF(username);
                        out.writeUTF("O login foi efetuado com sucesso!");
                        out.flush();
                    } catch (Exception e) {
                        out.writeBoolean(false);
                        out.writeUTF(e.getMessage());
                        out.flush();
                    }
                } else if (opcao.equals("2")) {

                    try {
                        gestor.registaUtilizador(username, password);
                        out.writeUTF("O seu registo foi efetuado com sucesso!");
                    } catch (Exception e) {
                        out.writeUTF(e.getMessage());
                        out.flush();
                    }
                }
            }

            while (true) {
                String opcao = in.readUTF();
                if (opcao.equals("1")) { /* procura trotinete, ou seja, listagem */
                    List<Local> locais_trotis = gestor.listatrotinetes(Local.deserialize(in));
                    out.writeInt(locais_trotis.size());
                    for (Local p : locais_trotis) {
                        p.serialize(out);
                    }
                    out.flush();
                }

                if (opcao.equals("2")) { /* reserva trotinete */
                    String reserva = gestor.reservaTrotinetes(Local.deserialize(in), in.readUTF());
                    out.writeUTF(reserva);
                    out.flush();
                }
                if (opcao.equals("3")) { /* estacionar trotinete */
                    try {
                        double estaciona = gestor.estacionaTrotinetes(Local.deserialize(in), in.readUTF(), in.readInt());
                        out.writeBoolean(true);
                        out.writeDouble(estaciona);
                        out.flush();
                    } catch (Exception e) {
                        out.writeBoolean(false);
                        out.writeUTF(e.toString());
                        out.flush();
                    }
                }
                if (opcao.equals("0")) {
                    gestor.exit(in.readUTF());
                    break;
                }
            }
            socket.shutdownInput();
            socket.shutdownOutput();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
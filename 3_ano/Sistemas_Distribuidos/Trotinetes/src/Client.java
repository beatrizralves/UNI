import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Client {


    public static void main(String[] args) throws IOException {
        System.out.println("Bem-vindo ao GestFrota! ");
        Socket socket = new Socket("localhost", 8000);/*porta para ligação com o Servidor */
        BufferedReader inStdin = new BufferedReader(new InputStreamReader(System.in));/* buffer */

        DataInputStream in = new DataInputStream(socket.getInputStream());/* para receber */
        DataOutputStream out = new DataOutputStream(socket.getOutputStream());/* para enviar */
        String utilizador = null;

        boolean login = false;
        while (!login) {

            System.out.println("1 - Login | 2 - Registar | 0 - Sair");
            String opcao = inStdin.readLine();
            if (opcao.equals("1")) {/* Login utilizador */
                log(inStdin, out, opcao);
                out.flush();
                login = in.readBoolean();
                if (login) {
                    utilizador = in.readUTF();
                    System.out.println(in.readUTF());
                } else System.out.println(in.readUTF());
            }
            if (opcao.equals("2")) {/*Registo do utilizador */
                log(inStdin, out, opcao);
                out.flush();
                System.out.println(in.readUTF());
            }
            if (opcao.equals("0")) {/* Sai do programa*/
                System.out.println("Obrigado pela preferência! Volte sempre!");
                socket.shutdownInput();
                socket.shutdownOutput();
                socket.close();
                return;
            }
        }

        while (true) {/* se o ‘login’ for efetuado com sucesso criamos este ‘loop’ até o utilizador sair*/
            System.out.println("Menu: 1 - Procurar | 2 - Reservar | 3 - Estacionar | 0 - Sair");
            String opcao = inStdin.readLine();
            if (opcao.equals("1")) {  /*Lista as Trotinetes Livres a distância fixa(2)  */
                coordenadas(inStdin, out, opcao);
                out.flush();
                int tamanho = in.readInt();

                if (tamanho > 0 && tamanho < 15) {
                    List<Local> listat = new ArrayList<>();
                    for (int i = 0; i < tamanho; i++) {
                        listat.add(Local.deserialize(in));
                    }
                    System.out.println(listat);
                } else System.out.println("Nenhuma trotinete disponível nesta área");
            }

            if (opcao.equals("2")) { /*Reserva Trotinete */
                coordenadas(inStdin, out, opcao);
                out.writeUTF(utilizador);
                out.flush();
                System.out.println(in.readUTF());
            }

            if (opcao.equals("3")) {  /* Estaciona Trotinete */
                coordenadas(inStdin, out, opcao);
                out.writeUTF(utilizador);
                System.out.println("Escreva o código de reserva:");
                out.writeInt(Integer.parseInt(inStdin.readLine()));
                out.flush();
                boolean estacionar = in.readBoolean();
                if (estacionar) {
                    double custo = in.readDouble();
                    System.out.println("O custo da viagem é: " + Math.abs(custo) + "€");
                } else {
                    System.out.println(in.readUTF());
                }
            }

            if (opcao.equals("0")) {/* Desconectado */
                out.writeUTF(opcao);
                out.writeUTF(utilizador);
                out.flush();
                break;
            }
        }
        /* Sai do programa*/
        System.out.println("Obrigado pela preferência! Volte sempre!");
        socket.shutdownInput();
        socket.shutdownOutput();
        socket.close();
    }


    /* função para fazer o registo e o 'login' */
    private static void log(BufferedReader inStdin, DataOutputStream out, String opcao) throws IOException {
        out.writeUTF(opcao);
        System.out.println("Nome de utilizador:");
        out.writeUTF(inStdin.readLine());
        System.out.println("Palavra-passe:");
        out.writeUTF(inStdin.readLine());
    }

    /* função para indicação das coordenadas */
    private static void coordenadas(BufferedReader inStdin, DataOutputStream out, String opcao) throws IOException {
        out.writeUTF(opcao);
        System.out.println("Digite as coordenadas do local que quer efetuar a operação:");
        System.out.println("x:");
        out.writeInt(Integer.parseInt(inStdin.readLine()));
        System.out.println("y:");
        out.writeInt(Integer.parseInt(inStdin.readLine()));
    }
}
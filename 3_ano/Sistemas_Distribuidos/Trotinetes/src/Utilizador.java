 import java.util.ArrayList;
import java.util.List;

/* -----------------------------------------------------------------------------------------------*/
                                        /* UTILIZADOR*/
public class Utilizador {

        /* Variáveis */
        private final String nomeUtilizador, password;
        boolean Loggado;
        private final List<Integer> codreservados;/* lista dos códigos que já se encontram nas reservas*/

        /* Construtor */
        public Utilizador(String nomeUtilizador, String password) {
            this.nomeUtilizador = nomeUtilizador;
            this.password = password;
            Loggado = false;
            codreservados = new ArrayList<>();
        }

        /* Métodos */

        public String getNomeUtilizador() {return nomeUtilizador;}

        public String getPassword() { return password; }

        public void login() { Loggado = true; }

        public void logoff() { Loggado = false; }

        public boolean Loggado() {return Loggado;}

        public void addCodigo(int code) {
            codreservados.add(code);
        }

        public void removeCodigo(int code) {
            codreservados.remove(Integer.valueOf(code));
        }

        public boolean containsCodigo(int code) {
            return codreservados.contains(code);
        }

        public List<Integer> getCodreservados() {
            return new ArrayList<>(codreservados);
        }
}
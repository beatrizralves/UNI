import java.io.*;

public class Sistema {
    // ... seus métodos e atributos existentes ...

    public void gravarEstadoSistema(String nomeArquivo) {
        try (ObjectOutputStream outputStream = new ObjectOutputStream(new FileOutputStream(nomeArquivo))) {
            outputStream.writeObject(this);
            System.out.println("Estado do sistema gravado com sucesso.");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Erro ao gravar o estado do sistema.");
        }
    }

    public static Sistema carregarEstadoSistema(String nomeArquivo) {
        try (ObjectInputStream inputStream = new ObjectInputStream(new FileInputStream(nomeArquivo))) {
            Sistema sistema = (Sistema) inputStream.readObject();
            System.out.println("Estado do sistema carregado com sucesso.");
            return sistema;
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println("Erro ao carregar o estado do sistema. Criando um novo sistema.");
            return new Sistema(); // Se ocorrer um erro, crie um novo sistema
        }
    }
}

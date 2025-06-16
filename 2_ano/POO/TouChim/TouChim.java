import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.TreeSet;
import java.io.*;

public class TouChim implements Serializable
{

    private static ArrayList<Cliente> cliente;
    private static ArrayList<Dispositivo> dispositivo;
    private static ArrayList<Conta> conta;
    private static HashMap<Cliente, ArrayList<Conta>>ListaClientes;
    private static HashMap<Conta, TreeSet<Cliente>> ListadeContas;
    private static HashMap<Dispositivo, TreeSet<Conta>> ListaDispositivos;

    public TouChim() {

        if (cliente == null){
        cliente = new ArrayList<>();}

        if (dispositivo == null){
        dispositivo = new ArrayList<>();}

        if (conta == null){
        conta = new ArrayList<>();}

        if (ListaClientes == null){
        ListaClientes = new HashMap<>();}

        if (ListadeContas == null){
        ListadeContas = new HashMap<>();}

        if (ListaDispositivos == null){
        ListaDispositivos = new HashMap<>();}

    }

    public static List<Cliente> getCliente(){
        return cliente;
    }

    public static List<Dispositivo> getDispositivo(){
        return dispositivo;
    }

    public static List<Conta> getConta(){
        return conta;
    }

    public static HashMap<Cliente, ArrayList<Conta>> getListaClientes(){
        return ListaClientes;
    }

    public static HashMap<Conta, TreeSet<Cliente>> getListadeContas(){
        return ListadeContas;
    }
    public static HashMap<Dispositivo, TreeSet<Conta>> getListadeDispositivos(){
        return ListaDispositivos;
}

    public List<Conta> getListaConta(){
        ArrayList<Conta> temp = new ArrayList<Conta>();
        for(Conta c: conta)
            temp.add(c);
        return temp;
    }

    public static List<Cliente> getListaCliente(){
        ArrayList<Cliente> temp = new ArrayList<>();
        for(Cliente c: cliente)
            temp.add(c);
        return temp;
    }

    public static List<Dispositivo> getListaDispositivo(){
        ArrayList<Dispositivo> temp = new ArrayList<>();
        for(Dispositivo c: dispositivo)
            temp.add(c);
        return temp;
    }

    public HashMap<Cliente, ArrayList<Conta>> getListaDeClientes(){
        HashMap<Cliente, ArrayList<Conta>> temp = new HashMap<>();
        for(Cliente c: cliente){
            temp.put(c,c.getContaDoCliente());
        }
        return temp;
    }

    public HashMap<Conta, TreeSet<Cliente>> getListaContass(){
        HashMap<Conta, TreeSet<Cliente>> temp = new HashMap<>();
        for(Conta c: conta){
            temp.put(c, c.getListaDeClientes());
        }
        return temp;
    }
    public HashMap<Dispositivo, TreeSet<Conta>> getListadispositivos(){
        HashMap<Dispositivo, TreeSet<Conta>> temp = new HashMap<>();
        for(Dispositivo c: dispositivo){
            temp.put(c, c.getListaContas());
        }
        return temp;
    }
    public void addListaConta(Conta c, TreeSet<Cliente>l){
        if(c != null){
            ListadeContas.put(c,l);
        }
    }

    public void addListaCliente(Cliente c, ArrayList<Conta>l){
        if(c != null){
            ListaClientes.put(c, l);
        }
    }

    public void addListaDispositivo(Dispositivo d, TreeSet<Conta> c){
        if(c != null){
            ListaDispositivos.put(d,c);
        }
    }


    public void addcliente(Cliente c){
        if(c != null)
            cliente.add(c.clone());
    }

    public void addDispositivo(Dispositivo c){
        if(c != null)
            dispositivo.add(c.clone());
    }

    public void addConta(int clienteId, Conta c) {
        Cliente cliente = getClienteById(clienteId);

        if (cliente != null) {
            cliente.addConta(c);
            conta.add(c);
            System.out.println("Conta adicionada com sucesso para o cliente " + cliente.getNome());
        } else {
            System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
        }
    }
    /*public void addConta(Conta c){
        if(c != null)
            conta.add(c.clone());
    }*/

        // ... outras partes da classe
    
    public Cliente getClienteById(int clienteId) {
        for (Cliente cliente : cliente) {
            if (cliente.getCodigo() == clienteId) {
                return cliente;
            }
        }
        return null;
    }

    /*public void addConta(int clienteId, Conta c){
        Cliente cliente = getClienteById(clienteId);
        
        if(c != null){
            cliente.addConta(c);
            conta.add(c.clone());
        }else{
            System.out.println("Cliente nao encontrado");
        }
    }*/


    public void showCLientes(){
        int i = 1;
        for(Cliente c: cliente){
            System.out.println(i + ":" + "\n" + "Nome: " + c.getNome());
            i++;
        }

    }
    /*public void showClientes(){
        for(Cliente c: cliente){
            System.out.println("Nome: " + c.getNome());
            c.showContasDoCliente();
        }
    }*/

    public void showContas(){
        int i = 1;
        for(Conta c: conta){
            System.out.println(i + ":" + "\n" + "Codigo de Conta: " + c.getCodigo());
        }
    }

}


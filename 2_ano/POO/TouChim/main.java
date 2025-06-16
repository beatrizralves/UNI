import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class main {
    

    public static void main(String[] args) throws IOException, FileNotFoundException, ClassNotFoundException{
        TouChim touchim = new TouChim();

        Scanner scanI = new Scanner(System.in);
        Scanner scanS = new Scanner(System.in);
        int idd = 0;

        int option1 = -1;
        

        do{
            clearScreen();
            System.out.println("**********GESTÃO DE COMUNICAÇÕES MÓVEIS**********");
            System.out.println("----------MENU----------");
            System.out.println("..........ESCOLHA A SUA OPÇÃO..........");
            System.out.println("[1] REGISTAR CLIENTE");
            System.out.println("[2] CRIAR CONTA");
            System.out.println("[3] ADICIONAR DISPOSITIVO");
            System.out.println("[4] LISTA DE CLIENTES");
            System.out.println("[5] LISTA DE CONTAS DO CLIENTE");
            System.out.println("[6] LISTA DE DISPOSITIVOS DO CLIENTE");
            System.out.println("[7] ESTABELECER COMUNICAÇÃO ENTRE DOIS CLIENTES");
            System.out.println("[8] MOSTRAR COMUNICAÇOES DE UM CLIENTE");
            System.out.println("[0] SAIR");

            option1 = scanI.nextInt();
            clearScreen();

            switch (option1){


                case 1:
                    clearScreen();
                    System.out.println("REGISTO DE UM NOVO CLIENTE");
                    idd++;
                    System.out.println("Codigo: ");
                    int cCodigo = scanI.nextInt();
                    System.out.println("Nome: ");
                    String cNome = scanS.nextLine();
                    Cliente cli = new Cliente(cCodigo, cNome);
                    touchim.addcliente(cli.clone());
                    touchim.addListaCliente(cli.clone(), cli.getContaDoCliente());
                    break;

                case 2:
                    clearScreen();
                    System.out.println("CRIAÇÃO DE UMA NOVA CONTA");

                    // Solicitar o código do cliente ao usuário
                    System.out.println("Código do Cliente: ");
                    int codCli = scanI.nextInt();

                    // Encontrar o cliente na lista
                    Cliente clienteExistente = touchim.getClienteById(codCli);

                    if (clienteExistente != null) {
                        // Se o cliente existe, criar a nova conta
                        System.out.println("Código de conta: ");
                        int codConta = scanI.nextInt();
                        Conta novaConta = new Conta(codConta);

                        // Adicionar a nova conta ao cliente e à lista geral de contas
                        touchim.addConta(codCli, novaConta);
                    } else {
                        System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
                    }

                break;

                case 3:
                    clearScreen();
                    System.out.println("ADICIONAR DISPOSITIVO A UMA CONTA");

                    // Solicitar o código do cliente ao usuário
                    System.out.println("Código do Cliente: ");
                    int codCliAdicionarDispositivo = scanI.nextInt();

                    // Encontrar o cliente na lista
                    Cliente clienteAdicionarDispositivo = touchim.getClienteById(codCliAdicionarDispositivo);

                    if (clienteAdicionarDispositivo != null) {
                        // Se o cliente existe, mostrar as contas associadas a ele
                        System.out.println("Contas do Cliente " + clienteAdicionarDispositivo.getNome() + ":");

                        for (Conta conta : clienteAdicionarDispositivo.getContas()) {
                            System.out.println("Conta: " + conta.getCodigo());
                        }

                        // Solicitar o código da conta ao usuário
                        System.out.println("Código da Conta: ");
                        int codContaAdicionarDispositivo = scanI.nextInt();

                        // Encontrar a conta na lista
                        Conta contaAdicionarDispositivo = clienteAdicionarDispositivo.getContaById(codContaAdicionarDispositivo);

                        if (contaAdicionarDispositivo != null) {
                            // Se a conta existe, adicionar um dispositivo a ela
                            System.out.println("Número do Dispositivo: ");
                            int numeroDispositivo = scanI.nextInt();
                            System.out.println("Modo de Funcionamento (4G ou 5G): ");
                            String modoFuncionamento = scanS.nextLine();
                            System.out.println("Capacidade de Memória: ");
                            double capacidadeMemoria = scanI.nextDouble();

                            // Escolher entre Telemóvel ou Tablet
                            System.out.println("Escolha o tipo de dispositivo (1 para Telemóvel, 2 para Tablet): ");
                            int tipoDispositivo = scanI.nextInt();

                            Dispositivo novoDispositivo;

                            if (tipoDispositivo == 1) {
                                novoDispositivo = new Telemovel(numeroDispositivo, modoFuncionamento);
                            } else if (tipoDispositivo == 2) {
                                novoDispositivo = new Tablet(numeroDispositivo, modoFuncionamento, capacidadeMemoria);
                            } else {
                                System.out.println("Tipo de dispositivo inválido.");
                                break;
                            }

                            // Adicione o dispositivo à conta
                            contaAdicionarDispositivo.addDispositivo(novoDispositivo);

                            System.out.println("Dispositivo adicionado com sucesso à conta " + contaAdicionarDispositivo.getCodigo());
                        } else {
                            System.out.println("Conta não encontrada. Certifique-se de que o código da conta está correto.");
                        }
                    } else {
                        System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
                    }

                    break;

                case 4:
                    clearScreen();
                    touchim.showCLientes();

                    break;

                case 5:
                    clearScreen();
                    System.out.println("LISTA DE CONTAS DO CLIENTE");

                    // Solicitar o código do cliente ao usuário
                    System.out.println("Código do Cliente: ");
                    int codCliConsulta = scanI.nextInt();

                    // Encontrar o cliente na lista
                    Cliente clienteConsulta = touchim.getClienteById(codCliConsulta);

                    if (clienteConsulta != null) {
                        // Se o cliente existe, mostrar as contas associadas a ele
                        System.out.println("Contas do Cliente " + clienteConsulta.getNome() + ":");
                        clienteConsulta.showContasDoCliente();
                    } else {
                        System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
                    }

                    break;

                case 6:
                    clearScreen();
                    System.out.println("LISTA DE DISPOSITIVOS ASSOCIADOS À CONTA DO CLIENTE");

                    // Solicitar o código do cliente ao usuário
                    System.out.println("Código do Cliente: ");
                    int codCliDispositivos = scanI.nextInt();

                    // Encontrar o cliente na lista
                    Cliente clienteDispositivos = touchim.getClienteById(codCliDispositivos);

                    if (clienteDispositivos != null) {
                        // Se o cliente existe, mostrar os dispositivos associados a ele
                        System.out.println("Dispositivos do Cliente " + clienteDispositivos.getNome() + ":");

                        for (Conta conta : clienteDispositivos.getContas()) {
                            System.out.println("Conta: " + conta.getCodigo());

                            // Verificar se a conta possui dispositivos associados
                            if (conta.getDispositivos().isEmpty()) {
                                System.out.println("Nenhum dispositivo associado a esta conta.");
                            } else {
                                // Mostrar os dispositivos associados à conta
                                System.out.println("Dispositivos associados:");

                                for (Dispositivo dispositivo : conta.getDispositivos()) {
                                    if (dispositivo instanceof Telemovel) {
                                        System.out.println("Telemóvel - Número: " + dispositivo.getNumero() + ", Modo de Funcionamento: " + dispositivo.getModoFuncionamento());
                                    } else if (dispositivo instanceof Tablet) {
                                        System.out.println("Tablet - Número: " + dispositivo.getNumero() + ", Modo de Funcionamento: " + dispositivo.getModoFuncionamento());
                                    }
                                }
                            }
                        }
                    } else {
                        System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
                    }

                    break;

                case 7:
                    clearScreen();
                    System.out.println("ESTABELECER COMUNICAÇÃO ENTRE DOIS CLIENTES");

                    // Solicitar os códigos dos dois clientes ao usuário
                    System.out.println("Código do Cliente 1: ");
                    int codCli1 = scanI.nextInt();
                    System.out.println("Código do Cliente 2: ");
                    int codCli2 = scanI.nextInt();

                    // Encontrar os clientes na lista
                    Cliente cliente1 = touchim.getClienteById(codCli1);
                    Cliente cliente2 = touchim.getClienteById(codCli2);

                    if (cliente1 != null && cliente2 != null) {
                        // Escolher o tipo de comunicação
                        System.out.println("Escolha o tipo de comunicação (1 para SMS, 2 para Mensagem de Imagem, 3 para Mensagem de Vídeo, 4 para Chamada Normal, 5 para Chamada de Vídeo, 6 para Download): ");
                        int tipoComunicacao = scanI.nextInt();

                        switch (tipoComunicacao) {
                            case 1:
                                // Comunicação SMS
                                System.out.println("Tamanho da Mensagem: ");
                                int tamanhoSMS = scanI.nextInt();
                                System.out.println("Texto da Mensagem: ");
                                String textoSMS = scanS.nextLine();

                                SMS sms = new SMS(1, "SMS", cliente1.getCodigo(), cliente2.getCodigo(), tamanhoSMS, textoSMS);
                                cliente1.enviarComunicacao(cliente2, sms);
                                break;

                            case 2:
                                // Mensagem de Imagem
                                System.out.println("Tamanho em Bytes: ");
                                int tamanhoImagem = scanI.nextInt();
                                System.out.println("Formato da Imagem: ");
                                String formatoImagem = scanS.nextLine();
                                System.out.println("Resolução da Imagem: ");
                                String resolucaoImagem = scanS.nextLine();

                                Msg_Imagem imagem = new Msg_Imagem(1, cliente1.getCodigo(), cliente2.getCodigo(), "Mensagem de Imagem", tamanhoImagem, formatoImagem, resolucaoImagem);

                                cliente1.enviarComunicacao(cliente2, imagem);
                                break;

                            case 3:
                                // Mensagem de Vídeo
                                System.out.println("Duração da Mensagem: ");
                                double duracaoVideo = scanI.nextDouble();
                                System.out.println("Formato da Mensagem: ");
                                String formatoVideo = scanS.nextLine();
                                System.out.println("Resolução da Mensagem: ");
                                String resolucaoVideo = scanS.nextLine();

                                Msg_Video video = new Msg_Video(1, cliente1.getCodigo(), cliente2.getCodigo(), "Mensagem de Video", duracaoVideo, formatoVideo, resolucaoVideo);
                                cliente1.enviarComunicacao(cliente2, video);
                                break;

                            case 4:
                                // Chamada Normal
                                System.out.println("Duração da Chamada: ");
                                double duracaoChamada = scanI.nextDouble();

                                Chamada_Normal chamadaNormal = new Chamada_Normal(1, "Chamada Normal", cliente1.getCodigo(), cliente2.getCodigo(), duracaoChamada);
                                cliente1.enviarComunicacao(cliente2, chamadaNormal);
                                break;

                            case 5:
                                // Chamada de Vídeo
                                System.out.println("Duração da Chamada: ");
                                double duracaoChamadaVideo = scanI.nextDouble();
                                System.out.println("Resolução da Chamada: ");
                                String resolucaoChamadaVideo = scanS.nextLine();

                                Chamada_Video chamadaVideo = new Chamada_Video(1, "Chamada de Vídeo", cliente1.getCodigo(), cliente2.getCodigo(), duracaoChamadaVideo, resolucaoChamadaVideo);
                                cliente1.enviarComunicacao(cliente2, chamadaVideo);
                                break;

                            case 6:
                                // Download
                                System.out.println("Tamanho do Download em Bytes: ");
                                int tamanhoDownload = scanI.nextInt();
                                System.out.println("Duração do Download: ");
                                double duracaoDownload = scanI.nextDouble();

                                Download download = new Download(1, "Download", cliente1.getCodigo(), cliente2.getCodigo(), tamanhoDownload, duracaoDownload);
                                cliente1.enviarComunicacao(cliente2, download);
                                break;

                            default:
                                System.out.println("Tipo de comunicação inválido.");
                                break;
                        }
                    } else {
                        System.out.println("Um ou ambos os clientes não foram encontrados. Certifique-se de que os códigos dos clientes estão corretos.");
                    }
                    break;
                case 8:
                    clearScreen();
                    System.out.println("MOSTRAR COMUNICAÇÕES DE UM CLIENTE");

                    // Solicitar o código do cliente ao usuário
                    System.out.println("Código do Cliente: ");
                    int codCliComunicacoes = scanI.nextInt();

                    // Encontrar o cliente na lista
                    Cliente clienteComunicacoes = touchim.getClienteById(codCliComunicacoes);

                    if (clienteComunicacoes != null) {
                        // Mostrar as comunicações associadas ao cliente
                        System.out.println("Comunicações do Cliente " + clienteComunicacoes.getNome() + ":");

                        // Lógica para mostrar as diferentes comunicações
                        for (Comunicacao comunicacao : clienteComunicacoes.getComunicacoes()) {
                            if (comunicacao instanceof SMS) {                                       //se comunicaçao for do tipo SMS
                                SMS sms = (SMS) comunicacao;
                                System.out.println("SMS - Tamanho: " + sms.getTamanhoMensagem() + ", Texto: " + sms.getTexto());
                            } else if (comunicacao instanceof Msg_Imagem) {
                                Msg_Imagem imagem = (Msg_Imagem) comunicacao;
                                System.out.println("Mensagem de Imagem - Tamanho: " + imagem.getTamanhoimg() + ", Formato: " + imagem.getFormato() + ", Resolução: " + imagem.getResolucao());
                            } else if (comunicacao instanceof Chamada_Normal) {
                                Chamada_Normal chamadaNormal = (Chamada_Normal) comunicacao;
                                System.out.println("Chamada Normal - Duração: " + chamadaNormal.getDuracao());
                            } else if (comunicacao instanceof Chamada_Video) {
                                Chamada_Video chamadaVideo = (Chamada_Video) comunicacao;
                                System.out.println("Chamada de Vídeo - Duração: " + chamadaVideo.getDuracao() + ", Resolução: " + chamadaVideo.getResolucao());
                            } else if (comunicacao instanceof Msg_Video) {
                                Msg_Video mensagemVideo = (Msg_Video) comunicacao;
                                System.out.println("Mensagem de Vídeo - Duração: " + mensagemVideo.getDuracao() + ", Formato: " + mensagemVideo.getFormato() + ", Resolução: " + mensagemVideo.getResolucao());
                            } else if (comunicacao instanceof Download) {
                                Download download = (Download) comunicacao;
                                System.out.println("Download - Tamanho: " + download.getTamanho_programa() + ", Duração: " + download.getDuracao());
                            }
                        }
                    } else {
                        System.out.println("Cliente não encontrado. Certifique-se de que o código do cliente está correto.");
                    }

                    break;


                case 0:
                    System.out.print("Obrigado por usar a nossa aplicação!!\n");
                    break;

                default:
                    System.out.println("Escolha uma opção válida!\n");
            }
        }while(option1 !=0);
    }


    // FUNÇAO QUE LIMPA A LINHA DE COMANDOS
    public static void clearScreen(){
        try{
            if (System.getProperty("os.name").contains("Windows"))
                new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            else{
                System.out.print("\033[H\033[2J");
                System.out.flush();
            }
        }
        catch (Exception ex) {}
    }
}

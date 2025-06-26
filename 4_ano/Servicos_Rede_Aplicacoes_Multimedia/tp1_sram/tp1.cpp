#include "tp1.h"

bool tem_debug_ativo = false;

// Funcao para imprimir logs apenas se o debug está ativo
void imprimeLogMensagem(const std::string &str)
{
    if (tem_debug_ativo)
    {
        std::cout << str << std::endl;
    }
}

unsigned int MAX_BYTES_BLOCO = 65536;
unsigned int MAX_TAMANHO_DICIONARIO = 65536;
unsigned int tipo_dicionario = 0;
unsigned int tipo_limpeza_dicionario = 0;

Tempo totalStart;
Tempo totalEnd;

unsigned int qnt_padroes = 0;
unsigned long n_padroes_inseridos = 0;
unsigned long tamanho_padroes_inseridos = 0;
unsigned long padroes_encontrados = 0;
unsigned long tamanho_padroes_encontrados = 0;
unsigned int n_vezes_limite = 0;
unsigned int atualizado_limite = 0;
unsigned int Iteracao = 1;
unsigned int n_blocos = 1;

std::vector<std::streamsize> blockReadBytesArray;

double tempo_total_leitura = 0;
double tempo_total_escrita = 0;

//-------------------------STATS FUNCTIONS----------------------------------------
void statics()
{
    // Tamanho Médio dos Padrões Inseridos no Dicionário
    double tam_medio_padroes = tamanho_padroes_inseridos / n_padroes_inseridos;

    // Número de blocos processados
    unsigned int n_blocos_processados = blockReadBytesArray.size();

    // Tamanho dos blocos processados (todos + último)
    String blocos_processados;

    // Tamanho do ficheiro processado
    unsigned int tamanho_ficheiro_bytes = ObterFicheiroProcessadoBytes(blocos_processados);

    // Tamanho médio dos padrões usados nos códigos de saída
    double tamM_padroes_encontrados = tamanho_padroes_encontrados / padroes_encontrados;

    // Número médio de bytes processados por cada código de saída
    // Não calculado!!

    // Número de vezes que cada código de saída é repetido
    // Não calculado!!

    // Número total de vezes que os códigos de saída mais comuns são usados
    // Não calculado!!

    // Distância média entre os valores dos códigos de saída consecutivos
    // Não calculado!!

    // Distância máxima entre os valores dos códigos de saída consecutivos
    // Não calculado!!

    // Número total de códigos de saída únicos
    // Não calculado!!

    printStatics(tam_medio_padroes, n_blocos_processados, blocos_processados, tamanho_ficheiro_bytes, tamM_padroes_encontrados);
}

unsigned int ObterFicheiroProcessadoBytes(String &blocos_processados)
{
    // tamanho do ficheiro processado
    unsigned int bytes_processados = 0;
    int i = 0;
    int j = 1;

    blocos_processados += "      ";

    for (std::streamsize byte_lido : blockReadBytesArray)
    {
        bytes_processados += byte_lido;

        blocos_processados += std::to_string(j) + ": " + std::to_string(byte_lido) + "  ";

        if (i == 4)
        {
            blocos_processados += "\n      ";
            i = -1;
        }
        i++;
        j++;
    }

    return bytes_processados;
}

double ObterDiferencaTempoSegundos(const Tempo &inicio, const Tempo &fim)
{
    auto diferenca = std::chrono::duration<double>(fim - inicio);
    return diferenca.count(); // em segundos
}

///---------------------------------- PRINTING FUNCTIONS----------------------------
void ImprimirLogotipo()
{
    std::cout << "88           888888888888  I8,        8        ,8I         88  88888888ba\n";
    std::cout << "88                    ,88  `8b       d8b       d8'         88  88      \"8b\n";
    std::cout << "88                  ,88\"    \"8,     ,8\"8,     ,8\"          88  88      ,8P\n";
    std::cout << "88                ,88\"       Y8     8P Y8     8P   ,adPPYb,88  88aaaaaa8P'\n";
    std::cout << "88              ,88\"         `8b   d8' `8b   d8'  a8\"    `Y88  88\"\"\"\"88'\n";
    std::cout << "88            ,88\"            `8a a8'   `8a a8'   8b       88  88    `8b\n";
    std::cout << "88           88\"               `8a8'     `8a8'    \"8a,   ,d88  88     `8b\n";
    std::cout << "88,ad8888ba, 888888888888       `8'       `8'      \"\"8bbdP\"Y8  88      `8b\n";
    std::cout << " d8\"'    \"8b\n";
    std::cout << "d8'\n";
    std::cout << "88              ,adPPYba,   88,dPYba,,adPYba,   8b,dPPYba,   8b,dPPYba,   ,adPPYba,  ,adPPYba,  ,adPPYba,\n";
    std::cout << "88             a8\"     \"8a  88P'   \"88\"    \"8a  88P'    \"8a  88P'   \"Y8  a8P_____88  I8[    \"\"  I8[    \"\"\n";
    std::cout << "Y8,            8b       d8  88      88      88  88       d8  88          8PP\"\"\"\"\"\"   \"\"Y8ba,    \"\"Y8ba,\n";
    std::cout << " Y8a.    .a8P  \"8a,   ,a8\"  88      88      88  88b,   ,a8\"  88          \"8b,   ,aa  aa    ]8I  aa    ]8I\n";
    std::cout << "  \"\"Y8888Y\"'    \"\"YbbdP\"'   88      88      88  88`YbbdP\"'   88           \"\"Ybbd8\"'  \"\"YbbdP\"'  \"\"YbbdP\"'\n";
    std::cout << "                                                88\n";
    std::cout << "                                                88\n";
    std::cout << "";
    std::cout << "";
    std::cout << " ------------------------------------------------------------------------\n";
    std::cout << "| METI                                                                   |\n"; 
    std::cout << "| Serviços de Rede e Aplicações  Multimédia                              |\n"; 
    std::cout << "| Ano letivo 2023/24                                                     |\n";
    std::cout << "| Beatriz Alves PG53682, Bruno Silva PG53706, Ricardo Graça PG54183      |\n";
    std::cout << " ------------------------------------------------------------------------\n";
}

void printParameters(const std::string &inputFileName, const std::string &outputFileName)
{
    std::cout << std::endl;
    std::cout << "Nome Ficheiro Entrada: " << inputFileName << std::endl;
    std::cout << "Nome Ficheiro Saída:   " << outputFileName << std::endl;
    std::cout << "Tamanho Máximo Blocos:     " << MAX_BYTES_BLOCO << " Bytes" << std::endl;
    std::cout << "Tamanho Máximo Dicionário: " << MAX_TAMANHO_DICIONARIO << " Entradas" << std::endl;
    std::cout << "Tipo Dicionário Incial: " << std::endl;
    switch (tipo_dicionario)
    {
    case 0:
        std::cout << "Preenchido com todos os padrões de 1 Byte" << std::endl;
        break;

    case 1:
        std::cout << "Nulo, ou seja, inicialmente vazio" << std::endl;
        break;

    case 2:
        std::cout << "Padões mais prováveis" << std::endl;
        break;

    default:
        break;
    }
    std::cout << "Tipo Limpeza Dicionário: " << std::endl;
    switch (tipo_limpeza_dicionario)
    {
    case 0:
        std::cout << "Nenhum tipo de limpeza" << std::endl;
        break;

    case 1:
        std::cout << "Reset de dicionário" << std::endl;
        break;

    case 2:
        std::cout << "Remove os padrões menos utilizados do dicionário" << std::endl;
        break;

    default:
        break;
    }
}

// função para imprimir mensagem de erro de argumentos
void ImprimeArgErro()
{
    std::cout << "ERRO: Digite os argumentos necessarios:" << std::endl;
    std::cout << "Argumentos: <input_file> <output_file> <tamanho_maximo_bloco> <tamanho_maximo_dicionario> <tipo_dicionario_inicial> <tipo_limpeza_dicionario>" << std::endl;
    std::cout << "Tamanho maximo do bloco deve estar entre ]0, 67109056] bytes." << std::endl;
    std::cout << "Tamanho maximo do dicionario deve estar entre [4096, 16777216]." << std::endl;
    std::cout << "Tipo de dicionario inicial deve ser 0-LWDR, 1-nulo, 2-padroes mais provaveis." << std::endl;
    std::cout << "Tipo de limpeza do dicionario deve ser 0-nenhuma, 1-reset, 2-limpeza dos padroes menos usados." << std::endl;
}

void printTime()
{
    double deltaT_Total = ObterDiferencaTempoSegundos(totalStart, totalEnd);
    double deltaT_R_W = tempo_total_leitura + tempo_total_escrita;
    double deltaT_Compression = deltaT_Total - deltaT_R_W;

    std::cout << std::endl;
    std::cout << "Tempo Total Decorrido:" << deltaT_Total << " segundos (" << deltaT_Total / deltaT_Total * 100 << "%)" << std::endl;
    std::cout << "Tempo Total Leitura/Escrita:" << deltaT_R_W << " segundos (" << deltaT_R_W / deltaT_Total * 100 << "%)" << std::endl;
    std::cout << "Tempo Total Leitura:" << tempo_total_leitura << " segundos (" << tempo_total_leitura / deltaT_Total * 100 << "%)" << std::endl;
    std::cout << "Tempo Total Escrita:" << tempo_total_escrita << " segundos (" << tempo_total_escrita / deltaT_Total * 100 << "%)" << std::endl;
    std::cout << "Tempo Total Compressão:" << deltaT_Compression << " segundos (" << deltaT_Compression / deltaT_Total * 100 << "%)" << std::endl;
    std::cout << std::endl;
}

void imprimeDicionario(const EntradaDicionario *dicionario)
{
    EntradaDicionario entrada_dicionario;

    std::cout << "Dicionário Atual:" << std::endl;

    for (unsigned int i = 0; i < MAX_TAMANHO_DICIONARIO; i++)
    {

        if (dicionario[i].padrao != "")
        {

            entrada_dicionario = dicionario[i];

            std::cout << "Valor de Hash: " << i << std::endl;

            // Imprime o primeiro elemento da lista ligada
            imprimePadrao(entrada_dicionario);

            //  Verifica se entry.next é nulo antes de entrar no loop
            while (entrada_dicionario.next != nullptr)
            {

                //  Avança para o próximo elemento da lista ligada
                entrada_dicionario = *(entrada_dicionario.next);

                //  Imprime o elemento atual da lista ligada
                imprimePadrao(entrada_dicionario);
            }
        }
    }
    std::cout << std::endl;
}

void imprimePadrao(const EntradaDicionario &entrada_dicionario)
{
    std::cout << "  Padrão: " << entrada_dicionario.padrao << " | Código: " << entrada_dicionario.codigo << " | Nº Vezes: " << entrada_dicionario.n_vezes << std::endl;
}

void printStatics(double tam_medio_padroes, unsigned int numOfBlocks, const std::string &processedBlocks, unsigned int tamanho_ficheiro_bytes, double tamM_padroes_encontrados)
{
    std::cout << std::endl;
    std::cout << "Estatísticas" << std::endl;

    // Número de bytes processados
    std::cout << "Número de Bytes Processados:" << tamanho_ficheiro_bytes << " Bytes" << std::endl;
    // Número de vezes que os padrões são encontrados
    std::cout << "Número de Vezes que os Padrões são Encontrados:" << padroes_encontrados << " Vezes" << std::endl;

    // Número de códigos de padrões enviados para o output
    std::cout << "Número de Códigos de Padrões Enviados para o Output:" << padroes_encontrados << " Códigos" << std::endl;

    // Tamanho médio dos padrões inseridos no dicionário
    std::cout << "Tamanho Médio dos Padrões Inseridos no Dicionário:" << tam_medio_padroes << std::endl;

    // Número de vezes que o tamanho máximo do dicionário foi atingido
    std::cout << "Número de Vezes que o Tamanho Máximo do Dicionário foi Atingido:" << n_vezes_limite << " Vezes" << std::endl;

    // Número de blocos processados
    std::cout << "Número de Blocos Processados:" << numOfBlocks << " Blocos" << std::endl;

    // Tamanho dos blocos processados (todos + último)
    std::cout << "Tamanho dos Blocos Processados, em Bytes (Todos + Último):" << processedBlocks << std::endl;

    // Tamanho do ficheiro processado
    std::cout << "Tamanho do Ficheiro Processado:" << tamanho_ficheiro_bytes << " Bytes" << std::endl;

    // Tamanho médio dos padrões usados nos códigos de saída
    std::cout << "Tamanho Médio dos Padrões Usados nos Códigos de Saída:" << tamM_padroes_encontrados << std::endl;

    // Número médio de bytes processados por cada código de saída
    // Não calculado!!

    // Número de vezes que cada código de saída é repetido
    // Não calculado!!

    // Número total de vezes que os códigos de saída mais comuns são usados
    // Não calculado!!

    // Distância média entre os valores dos códigos de saída consecutivos
    // Não calculado!!

    // Distância máxima entre os valores dos códigos de saída consecutivos
    // Não calculado!!

    // Número total de códigos de saída únicos
    // Não calculado!!

    // Número de Padrões Inseridos no Dicionário
    std::cout << "Número de Padrões Inseridos no Dicionário:" << n_padroes_inseridos - 1 << " Padrões" << std::endl;

    // Número de Iterações para Processar a Totalidade do Ficheiro
    std::cout << "Número de Iterações para Processar a Totalidade do Ficheiro:" << Iteracao << " Iterações" << std::endl;
}

//--------------------------AUX FUNCTIONS----------------------------
void LZWDR(std::ifstream &ficheiro_entrada, std::ofstream &ficheiro_saida)
{
    // array de EntradaDicionarios para o dicionario
    EntradaDicionario *dicionario = new EntradaDicionario[65536];

    String buffer_entrada;
    String buffer_saida;

    Tempo tempo_inicio_leitura;
    Tempo tempo_fim_leitura;
    Tempo tempo_inicio_escrita;
    Tempo tempo_fim_escrita;

    InicializarDicionario(dicionario);

    // array de chars para o buffer de leitura
    char *buffer_leitura = new char[65536];

    while (true)
    {

        imprimeLogMensagem("╟-----Estou a processar o " + std::to_string(n_blocos++) + "º bloco-----╢");

        // LEITURA
        imprimeLogMensagem("╟-----Leitura do Bloco-----╢");

        tempo_inicio_leitura = std::chrono::steady_clock::now(); // inicializa timer leitura

        ficheiro_entrada.read(buffer_leitura, MAX_BYTES_BLOCO);    // Lê MAX_BYTES_BLOCO do ficheiro de entrada e copia para buffer de leitura
        std::streamsize n_bytes_lidos = ficheiro_entrada.gcount(); // Numero de bytes lidos

        if (n_bytes_lidos <= 0) // Se nao ha mais bytes p ler sai do loop
        {
            break;
        }

        buffer_entrada.assign(buffer_leitura, n_bytes_lidos); // Atribui os bytes lidos ao buffer de entrada

        blockReadBytesArray.push_back(n_bytes_lidos);

        tempo_fim_leitura = std::chrono::steady_clock::now(); // termina temporizador do tempo de leitura

        tempo_total_leitura += ObterDiferencaTempoSegundos(tempo_inicio_leitura, tempo_fim_leitura);
        // LEITURA

        // COMPRESSÃO
        imprimeLogMensagem("╟-----Compressão do Bloco-----╢");

        Comprimir(dicionario, buffer_entrada, buffer_saida);

        // COMPRESSÃO

        // ESCRITA
        imprimeLogMensagem("╟-----Escrita do Bloco-----╢");

        tempo_inicio_escrita = std::chrono::steady_clock::now(); // inicializa temporizador escrita

        ficheiro_saida.write(buffer_saida.data(), buffer_saida.size()); // Escreve os dados comprimidos no arquivo de saída

        buffer_entrada.clear();
        buffer_saida.clear();

        tempo_fim_escrita = std::chrono::steady_clock::now(); // termina temporizador escrita

        tempo_total_escrita += ObterDiferencaTempoSegundos(tempo_inicio_escrita, tempo_fim_escrita);
        // ESCRITA
    }

    if (tem_debug_ativo)
    {
        imprimeDicionario(dicionario);
    }

    delete[] buffer_leitura; // free memory
    delete[] dicionario;     // free memory
}

void Comprimir(EntradaDicionario *dicionario, const String &buffer_entrada, String &buffer_saida)
{
    unsigned int buffer_entrada_size = buffer_entrada.size(), ind, i, j, t;
    EntradaDicionario *entrada_dicionario;
    String Pa, Pb;

    // 1.-- Inserindo todos os símbolos de A como padrões em D.
    //   -- O primeiro Pa é apenas o primeiro símbolo de S e já está em D.
    // for (i=1 up to K) insert(A[i],D)
    // Pa=S[1,1]; ind=2
    Pa = buffer_entrada.substr(0, 1);
    ind = 2;

    insert(dicionario, Pa); // Caso o dicionário inicial seja nulo, tem que se inserir o primeiro Pa

    i = 1;

    while (true)
    {

        imprimeLogMensagem("╟-----" + std::to_string(Iteracao) + "ª Iteração-----╢");

        // 2.-- Processando S para encontrar o padrão maior Pb após Pa já em D.
        // code=search(Pa,D); Pb=S[ind,1]; i=1
        // while (ind+i≤N & search(concat(Pb,S[ind+i,1]),D))
        //     Pb=concat(Pb,S[ind+i,1]); i=i+1
        Pb = buffer_entrada.substr(ind - 1, 1);
        i = 1;

        imprimeLogMensagem("║  Pa: " + Pa);

        if (tem_debug_ativo)
        {
            imprimePadrao(*search(dicionario, Pa));
        }

        imprimeLogMensagem("║  Pb_inicial: " + Pb);

        String aux;

        while (ind + i <= buffer_entrada_size)
        {
            aux = buffer_entrada.substr(ind + i - 1, 1);

            imprimeLogMensagem("║  Pb_int: " + Pb + " | aux: " + aux);

            if (search(dicionario, concat(Pb, aux)) != nullptr) // Não encontrei o Padrão Pb
            {
                Pb = concat(Pb, aux);
                imprimeLogMensagem("║  Pb_novo: " + Pb);
                i++;
            }
            else // Encontrei o Padrão Pb
            {
                if (search(dicionario, Pb) == nullptr) // No caso do dicionário ser nulo, inserir o primeiro Pb
                {
                    insert(dicionario, Pb);
                }

                break;
            }
        }

        imprimeLogMensagem("║  Pb: " + Pb);

        if (tem_debug_ativo)
        {
            imprimePadrao(*search(dicionario, Pb));
        }

        // 3.-- Envie o código de Pa para a saída.
        // output(code)

        if (n_vezes_limite != atualizado_limite) // Caso o Dicionário seja reiniciado, garantir que se
        {                                        // volta a introduzir o Pa no dicionário para o algoritmo
            insert(dicionario, Pa);              // prosseguir corretamente
            atualizado_limite = n_vezes_limite;  // Atualizar o valor de previous para comparar corretamente

            imprimeLogMensagem("╟-----" + std::to_string(n_vezes_limite) + "ª Limpeza do Dicionário-----╢");
        }

        output(search(dicionario, Pa), buffer_saida);

        // 4.-- Insira em D todos os novos padrões enquanto D não estiver cheio
        //   -- (D atinge o máximo em T entradas).
        // j=1; t=T-1
        // while (j<=i & t<T)
        //     t=insert(concat(Pa,Pb[1,j]),D)
        //     if (t<T) t=insert(reverse(concat(Pa,Pb[1,j])),D)
        //     j=j+1

        j = 1;
        t = MAX_TAMANHO_DICIONARIO - 1;

        while (j <= i && t < MAX_TAMANHO_DICIONARIO)
        {
            String aux = Pb.substr(0, j);

            entrada_dicionario = insert(dicionario, concat(Pa, aux));

            if (entrada_dicionario != nullptr)
            {
                t = entrada_dicionario->codigo;

                if (t < MAX_TAMANHO_DICIONARIO)
                {
                    entrada_dicionario = insert(dicionario, reverse(concat(Pa, aux)));

                    if (entrada_dicionario != nullptr)
                    {
                        t = entrada_dicionario->codigo;
                    }
                }
            }
            j++;
        }

        // 5.-- Atualize as variáveis e retorne para 3. se não for o fim de S,
        //   -- caso contrário terminará enviando para a saída o código de Pb;
        //   -- mesmo que D esteja cheio, vamos continuar processando S…
        // if (ind+i>N) output(search(Pb,D))
        // else
        //     ind=ind+i; Pa=Pb; go to 2.

        if (ind + i > buffer_entrada_size)
        {
            output(search(dicionario, Pb), buffer_saida);

            Iteracao++;

            imprimeLogMensagem("╟-----" + std::to_string(Iteracao) + "ª Iteração-----╢");

            imprimeLogMensagem("║  Pa: " + Pb);

            if (tem_debug_ativo)
            {
                imprimePadrao(*search(dicionario, Pb));
            }

            imprimeLogMensagem("║  Pb: " + std::string("0*"));

            buffer_saida += "0*";

            break;
        }
        else
        {
            ind = ind + i;
            Pa = Pb;
            i = 1;
        }

        Iteracao++;
    }
}

// algoritmo de hashing DJB2
unsigned int ObterHash(const String &padrao)
{
    unsigned int hash = 5381; // valor inicial de hash arbitrário (5381 foi escolhido pois possui boas propriedades de dispersão)

    for (char c : padrao) // percorre cada caractere do padrão
    {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
    }

    return hash % MAX_TAMANHO_DICIONARIO; // para que o valor de hash não exceda os valores limites (índices) da tabela de hash
}

void InicializarDicionario(EntradaDicionario *dicionario)
{
    imprimeLogMensagem("╟-----Inicialização do Dicionário-----╢");

    qnt_padroes = 0; // TODO:O que é?

    for (unsigned int i = 0; i < MAX_TAMANHO_DICIONARIO; i++) // Para garantir que todas as posições do array estão "vazias"
    {
        dicionario[i].padrao = "";
        dicionario[i].codigo = 0;
        dicionario[i].n_vezes = 0;
        dicionario[i].next = nullptr;
    }
    switch (tipo_dicionario)
    {
    case 0:
        for (int i = 0; i < 256; i++)
        {
            String padrao = "";
            padrao += static_cast<char>(i);

            insert(dicionario, padrao);
        }
        break;
    case 1:
        break;
    case 2:
        break;

    default:
        break;
    }
}

void LimparDicionario(EntradaDicionario *dicionario)
{
    imprimeLogMensagem("╟-----Limpeza do Dicionário-----╢");

    n_vezes_limite++;

    if (tipo_limpeza_dicionario == 0) // 0: não efetua qualquer tipo de limpeza
    {
        // Não faz nada
    }
    else if (tipo_limpeza_dicionario == 1) // 1: dá reset ao dicionário, ou seja, regressa ao Dicionário Inicial
    {
        // Limpa cada elemento principal do array e a sua lista ligada
        for (unsigned int i = 0; i < MAX_TAMANHO_DICIONARIO; ++i)
        {
            EntradaDicionario *current = (dicionario[i].next);
            while (current != nullptr)
            {
                EntradaDicionario *next = current->next; // Salva o próximo elemento na lista ligada
                delete current;                          // Liberta a memória do elemento atual
                current = next;                          // Move para o próximo elemento na lista ligada
            }
        }

        // Reinicializa o dicionário após a limpeza
        InicializarDicionario(dicionario);
    }
    else if (tipo_limpeza_dicionario == 2) // 2: remove apenas os padrões menos utilizados do dicionário
    {
        // Não implementado
    }
}

EntradaDicionario *search(EntradaDicionario *dicionario, const String &padrao) // tem que ter este nome
{
    EntradaDicionario *entrada_dicionario = nullptr;

    // Calcula o hash da string pattern
    unsigned int hash_padrao = ObterHash(padrao);

    // Verifica se a entrada no dicionário no índice hashValue não é nula
    if (!dicionario[hash_padrao].padrao.empty())
    {
        // Verifica se o padrão na entrada do dicionário é igual ao padrão fornecido
        if (dicionario[hash_padrao].padrao == padrao)
        {
            entrada_dicionario = &dicionario[hash_padrao]; // Se for, retorna o endereço da entrada do dicionário
        }
        else
        {
            // Senão, percorre a lista ligada para encontrar a correspondência
            EntradaDicionario *proxima_entrada_dicionario = dicionario[hash_padrao].next;
            while (proxima_entrada_dicionario != nullptr)
            {
                if (proxima_entrada_dicionario->padrao == padrao)
                {
                    entrada_dicionario = proxima_entrada_dicionario; // Se encontrar, retorna o endereço do padrão
                    break;
                }
                proxima_entrada_dicionario = proxima_entrada_dicionario->next; // Avança para o próximo elemento na lista ligada
            }
        }
    }

    return entrada_dicionario;
}

EntradaDicionario *insert(EntradaDicionario *dicionario, const String &padrao) // tem que ter este nome
{
    EntradaDicionario *nova_entrada_dicionario_adicionada = nullptr;

    if (qnt_padroes <= MAX_TAMANHO_DICIONARIO) // Se o dicionário ainda não excedeu o limite
    {
        EntradaDicionario *nova_entrada_dicionario = new EntradaDicionario;

        unsigned int hashValue = ObterHash(padrao);

        nova_entrada_dicionario->padrao = padrao;
        nova_entrada_dicionario->codigo = 0;
        nova_entrada_dicionario->n_vezes = 0;
        nova_entrada_dicionario->next = nullptr;

        if (search(dicionario, padrao) == nullptr)
        {
            qnt_padroes++;                                 // Porque é um padrão novo,
            nova_entrada_dicionario->codigo = qnt_padroes; // Então atribui-se o seu número de sequência

            if (dicionario[hashValue].padrao.empty()) // Se for o 1º elemento da linked list da posição do array
            {
                dicionario[hashValue] = *nova_entrada_dicionario;
            }
            else // Caso já haja algum elemento com o mesmo valor de hash
            {
                EntradaDicionario *entrada_atual_dicionario = &dicionario[hashValue];
                while (entrada_atual_dicionario->next != nullptr) // Encontrar a cauda da linked list
                {
                    entrada_atual_dicionario = entrada_atual_dicionario->next;
                }
                entrada_atual_dicionario->next = nova_entrada_dicionario; // Inserir o novo padrão na cauda
            }

            nova_entrada_dicionario_adicionada = nova_entrada_dicionario; // Para retornar o "padrão" que foi adicionado

            if (tem_debug_ativo)
            {
                std::cout << "Inseri: ";
                imprimePadrao(*nova_entrada_dicionario_adicionada);
            }

            n_padroes_inseridos++;
            tamanho_padroes_inseridos += padrao.size();
        }
    }
    else // Quando o dicionário execede o limite, efetua-se a limpeza do dicionário
    {
        LimparDicionario(dicionario);

        if (tipo_limpeza_dicionario != 0) // Só volta a tentar inserir se a limpeza do dicionário for
        {                                 // diferente de "0: Não efetua qualquer tipo de limpeza"
            nova_entrada_dicionario_adicionada = insert(dicionario, padrao);
        }
    }

    return nova_entrada_dicionario_adicionada;
}

void output(EntradaDicionario *padrao, String &buffer_saida) // tem que ter este nome
{
    // codigo do padrao convertido para string
    String codigo_do_padrao = std::to_string(padrao->codigo);

    // Verifica se o tamanho do buffer_saida mais o tamanho de codigo_do_padrao e um caractere extra excedem o limite
    if (buffer_saida.size() + codigo_do_padrao.size() + 1 >= MAX_BYTES_BLOCO)
    {
        size_t newSize = buffer_saida.size() + codigo_do_padrao.size() + 2;
        buffer_saida.reserve(newSize); // Aumenta a capacidade do buffer para acomodar o novo tamanho
    }

    imprimeLogMensagem("║  Output: " + codigo_do_padrao);

    // Adiciona o código ao bufferOUT seguido de uma vírgula
    buffer_saida += codigo_do_padrao + ",";

    padrao->n_vezes++;                                      // Incrementar a quantidade de vezes que Pa foi encontrado
    padroes_encontrados++;                                  // Incrementar a quantidade de vezes que é encontrado um padrão
    tamanho_padroes_encontrados += padrao->padrao.length(); // Incrementar o tamanho de Pa à soma dos tamanhos de padrões encontrados
}

String concat(const String &Pa, const String &Pb) // tem que ter nome concat
{
    // Concatena o Pa e Pb
    String ret = Pa + Pb;
    return ret;
}

String reverse(const String &padrao) // tem que ter este nome
{
    // copia do padrao recebido
    String padrao_copia = padrao;

    // Inverte a cópia
    std::reverse(padrao_copia.begin(), padrao_copia.end());

    return padrao_copia;
}

/// ------------------------------------MAIN--------------------------------------
int main(int argc, char *argv[])
{
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32)) // se for windows
    SetConsoleOutputCP(65001);                                                      // para imprimir acentos no terminal em windows
#endif
    bool is_ficheiro_entrada_ok = false, is_ficheiro_saida_ok = false;
    std::ifstream fd_input;  // file descriptor para ficheiro de entrada
    std::ofstream fd_output; // file descriptor para ficheiro de saida

    String ficheiro_entrada_nome;
    String ficheiro_saida_nome;

    bool tem_erro = false;//erro na insercao de argumentos

    if (argc > 1) // se tiver argumentos
    {

        ficheiro_entrada_nome = argv[1];  // input.txt
        ficheiro_saida_nome = argv[2];    // output.txt
        int bloco = std::atoi(argv[3]);   // 65536
        int max_dic = std::atoi(argv[4]); // 65536
        int tipo = std::atoi(argv[5]);    // 0
        int limpeza = std::atoi(argv[6]); // 0 ou 1

        // handle input file
        fd_input.open(ficheiro_entrada_nome, std::ios::binary);

        if (!fd_input.is_open())
        {
            tem_erro = true;
        }
        else
        {
            is_ficheiro_entrada_ok = true;
        }
        ////

        // handle out file
        fd_output.open(ficheiro_saida_nome, std::ios::binary | std::ios::out | std::ios::trunc);

        if (!fd_output.is_open())
        {
            tem_erro = true;
        }
        else
        {
            is_ficheiro_saida_ok = true;
        }
        ///

        // handle max bloco size
        if (bloco > 0 && bloco <= 67109056)
        {
            MAX_BYTES_BLOCO = bloco;
        }
        else
        {
            tem_erro = true;
        }

        // handle max dictionary size
        if (max_dic >= 4096 && max_dic <= 16777216) // max_dic > 2^12 && max_dic < 2^24
        {
            MAX_TAMANHO_DICIONARIO = max_dic;
        }
        else
        {
            tem_erro = true;
        }

        // handle dic type
        if (tipo >= 0 && tipo <= 1)
        {
            tipo_dicionario = tipo;
        }
        else if (tipo == 2)
        {
            tem_erro = true;
        }
        else
        {
            tem_erro = true;
        }

        // handle clean tipo
        if (limpeza >= 0 && limpeza <= 1)
        {
            tipo_limpeza_dicionario = limpeza;
        }
        else if (limpeza == 2)
        {
            tem_erro = true;
        }
        else
        {
            tem_erro = true;
        }
    }
    if (is_ficheiro_entrada_ok && is_ficheiro_saida_ok)
    {
        ImprimirLogotipo();

        printParameters(ficheiro_entrada_nome, ficheiro_saida_nome);

        totalStart = std::chrono::steady_clock::now();

        LZWDR(fd_input, fd_output);

        totalEnd = std::chrono::steady_clock::now();

        fd_input.close();
        fd_output.close();

        printTime();

        statics();
    }

    if (!(is_ficheiro_entrada_ok && is_ficheiro_saida_ok) && tem_erro)
    {
        ImprimeArgErro();
    }

    return 0;
}

//Resolução do objetivo 1 do projeto pratico de Sistemas Operativos - 2022/2023 (LETI)
//Header-files 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include "satelite.h"
#include "funcoes.h"

//Defines
#define ERRO -1         //Utilizado para um codigo mais claro na leitura
#define BUFFER_SIZE 1024
#define READ 0          //Utilizado para um codigo mais claro na leitura
#define WRITE 1         //Utilizado para um codigo mais claro na leitura
#define ITS_A_FIRE 1    //Retorno quando um pixel satisfizer as condições de alerta de incendio
#define NOT_A_FIRE 0    //Retorno quando um pixxel NÃO satisfizer as condições de alerta de incendio 
#define MAX_PROCESS 4    //Numero maximo de processos filhos a executar concorrentemente ao mesmo tempo

/*Função verificaPixel - responsavel por verificar se um pixel individual satisfaz as condições de incendio enumeradas no enunciado - ira receber como
argumento um elemento da struct pixel (definido em satelite.h)*/
int verificaPixel (struct pixel p)//p stands for pixel
{
    //Declaração de variavels locais 
    float media; //Variavel auxiliar que ira guardar o valor da media dos valores de RGB para futura verificação 
    media = ((p.r + p.g + p.b) / 3); //Calculo da media 
    if(media > 100 && p.ir > 200) //Pixel com valores supeiores aos definidos para ser considerado um fogo ativo
        return ITS_A_FIRE;
    else    
        return NOT_A_FIRE;
}

//Função pesquisaLote - resolução do Objetivo 1 do trabalho pratico - o seu retorno sera o numero de alarmes encontrados 
int pesquisaLote(char* ficheiro, struct coordenada *alarmes, int n){
    /*Abertura do arquivo de texto (somemente para leitura) 
    Declaração de variaveis locais*/
    int fdTXT; //Variavel que ira guardar o descritor de ficheiro após a abertura do ficheiro de texto que foi passado como argumento à função
    
    fdTXT = open(ficheiro, O_RDONLY);//Abertura do ficheiro de texto em modo de leitura 
    if(fdTXT == -1)//O descritor de ficheiro será em caso de sucesso o menor numero inteiro positivo não utilizado, retornando -1 em caso de erro
        return ERRO;
    //Fim do tratamento de erros - abertura do ficheiro de texto 

    /*Obtenção do numero de linhas do ficheiro de texto e respetivo conteudo
    Declaração e inicializaão de variaveis locais */
    char bufferLineTXT[BUFFER_SIZE];//Buffer para onde sera lida cada linha do ficheiro de texto 
    char *temp;/*Variavel auxiliar que irá manipular o valor lido par bufferLineTXT dividindo o seu conteudo pelo delimitador espaço (semelhante à 
    função strtok da biblioteca string.h)*/
    int numberOfLines = 0;//Variavel auxiliar que guardara o numero de linhas do ficheiro de texto (cada linha será um ficheiro binario que sera
    int numeroDeProcessos = 0;// Número de processos em execução
    int numeroDEfogos = 0;//Variavel auxiliar que ira guardar o numero de fogos encontrados nos ficheiros binarios 
    int index;
    

    //percorrer todas as linhas do arquivo de texto
    while(readln(fdTXT, bufferLineTXT, 1024) > 0){//Leitura da linha do ficheiro de texto 
        //Declaração e inicialização de variaveis locasi 
        int pipefd[2];//Variavel que contera os descritores de leitura e de escrita do pipe anonimo
        pid_t pid;//Variavel que guardara o retorno da chamada ao sitema fork para que saibamos em que processo nos encontramos (filho pid = 0 | pai pid > 0 | erro pid < 0)
        
        if(pipe(pipefd) < 0){//Erro na criação do pipe a chamada ao sistema pipe retorna -1 em caso de erro e 0 em caso de sucesso
            write(STDERR_FILENO, "Erro na criação do pipe", sizeof("Erro na criação do pipe"));
            return ERRO;
        }//Fim do tratamento de erros na criação do pipe

        pid = fork();//Criação de processo filho

        if(pid < 0){//Tratamento de erros na criação do processo filho
            write(STDERR_FILENO, "Erro na criação do processo filho\n", sizeof("Erro na criação do processo filho\n"));
            return ERRO;
        }//Fim de tratamento de erros na criação do processos filho

        else if(pid == 0){//Encontramos-nos dentro do processo filho (todo o codigo até ao fecho das chavetas sera executado no processo filho)
    
            close(pipefd[READ]);// Fecha o descritor de leitura do pipe no processo filho
            //Declaração e inicialização de variaveis para armazenar os tokens 
            int latitude, longitude;
            char *caminhoBinario;
            //Variaveis utilizadas para garantir que a escrita so acontece quando o pipe não se encontra cheio
            fd_set writefds; // Conjunto de descritores de ficheiro prontos para escrita

            // Inicializa o conjunto writefds
            FD_ZERO(&writefds);
            FD_SET(pipefd[WRITE], &writefds);
        
            latitude = atoi(strtok(bufferLineTXT, " "));
            longitude = atoi(strtok(NULL, " "));
            caminhoBinario = strtok(NULL, "\0");
          
            //Declaração e inicialização de variaveis locais 
            struct coordenada alarme[n];//Variavel auxiliar que guardara as coordenadas do pixel onde se encontra um fogo para que sejam escritas no pipe
            struct pixel PIXEL;//Variavel auxiliar que guardara os valores de rgb e infra-vermelho do pixel em causa para verificação
            int fdBIN;//Variavel auxiliar que guardara o descritor de ficheiro do ficheiro bianrio devolvido pela função open
            int fire;//Variavel auxiliar que guardara o retorno da função verificaPixel, sabendo assim se um fogo foi detetado ou não
            int nOdLa, nOdLo;//Variaveis auxiliares que irão registar respetivamente o numero de digitos de latitude e longitude 

            fdBIN = open(caminhoBinario, O_RDONLY);//Abertura do ficherio binario em modo leitura apenas
            while(read(fdBIN, &PIXEL, sizeof(PIXEL)) == sizeof(PIXEL)){/*Iremos ler apenas uma linha dai igualarmos o que é lido ao tamanho de apenas um 
            pixel, pois sabemos que segundo o enunciado o ficheiro binario encontra-se formato com um pixel por linha*/
            fire = verificaPixel(PIXEL);//Verifica se o pixel extraido (linha atual do ficheiro binario) é um foco de incendio
                
                if(fire == ITS_A_FIRE){//O pixel verificado realmente correponde a uma coordenada em que existe um fogo 
                    //Armazena as coordenadas do alarme no vetor de alarme
                    alarme->latitude = latitude;//Passagem da latitude atual para a struct coordenada
                    alarme->longitude = longitude;//Passagem da longitude atual para a struct coordenada
                    while(select(pipefd[WRITE] + 1, NULL, &writefds, NULL, NULL) < 0);//Verifica que ninguem escreve para o pipe se este estiver cheio
                    write(pipefd[WRITE], &alarme, sizeof(struct coordenada));//Escrita no pipe da coordenada onde se encontra o incendio
                    numeroDEfogos++;//Incremetno do numero de fogos encontrados 
                    printf("Fogo!%d,%d\n", alarme->latitude,alarme->longitude);
                }
                longitude++;//A longitude é incrementada, pois esta varia em uma unidade por cada linha lida do ficheiro binario, a latitude é constante
            }//Fim de leitura de um pixel (uma linha do ficheiro binario)
            close(pipefd[WRITE]);//Fecho do descritor de escrita do pipe anonimo
        
            _exit(numeroDEfogos);
        } 
        else{//Processo pai (pid diferente de 0 e não negativo )

            close(pipefd[WRITE]);//Fecho do descritor de escrita do pipe anonimo

            while(read(pipefd[READ], &alarmes[numeroDEfogos], sizeof(struct coordenada)) > 0)//Leitura do conteudo escrito no pipe pelos filhos       
                numeroDEfogos++; // Incrementa o número de alarmes encontrado
            close(pipefd[READ]);//Fecho do descritor de leitura do pipe anonimo
            numeroDeProcessos++;//Incremetno do numero de processos a executar em concorrencia (Quantidade ficheiros binarios a serem pesquisados no momento)
        }//Fim do processo pai 
  
        if (numeroDeProcessos == MAX_PROCESS){//Espera que pelo menos um prcesso filho termine para libertar espaço para a 
            //Declaração e inicialização de variaveis locais
            int status;
            pid_t pid;
        
            pid = waitpid(-1, &status, WNOHANG);/*WNOHANG é uma constante que pode ser passada como argumento para a função waitpid(). Ela faz com que a função waitpid() retorne 
        imediatamente se não houver processos filhos prontos para serem esperados.*/
            if (pid > 0)// Processo filho terminou
                numeroDeProcessos--;//Decremento do numero de processos a operar em concorrencia 
        }
    }//Fim da verificação ainda existe algo para ler  
   
    return numeroDEfogos;//Return do numero de fogos encontrados caso nenhum seja encontradoiremos devolver 0 (inicialização de numeroDEfogos)
  }
    

int main(int argc, char* argv[])
{
    //Declaração e inicialização de variaveis locais 
    int numeroDeAlarmes;//Variavel que guardara o segundo argumento da linha de comando
    int nFogos;//Variavel que guardara o retorno da função pesquisaLote (numero de alarmes encontrados - fogos)

    //Verificação se dois argumentos foram recebidos pelo terminal (primerio arquivo de texto e segundo numero maximo de alarmes)
    if(argc != 3){
        write(STDERR_FILENO, "Pelo menos dois argumentos necessarios\n", sizeof("Pelo menos dois argumentos necessarios\n"));
        return ERRO;
    }
    numeroDeAlarmes = atoi(argv[2]);
    //Declaração e inicialização de variaveis locais
    struct coordenada alarmes[numeroDeAlarmes];//Struct que guardara as coordenadas dos alarmes encontrados nas pesquisas 

    nFogos = pesquisaLote(argv[1], alarmes, numeroDeAlarmes);
    printf("Nr de fogos: %d\n", nFogos);
}
//Resolução do objetivo 2 do projeto pratico de Sistemas Operativos - 2022/2023 (LETI)
//Header-files 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include "satelite.c"

//Defines
#define ERRO -1         //Utilizado para um codigo mais claro na leitura
#define BUFFER_SIZE 1024
#define READ 0          //Utilizado para um codigo mais claro na leitura
#define WRITE 1         //Utilizado para um codigo mais claro na leitura


int main(int argc,char *argv[])
{
    //Verificação se 2 argumentos foram recebidos atraves da compilação 
    if(argc != 3){
        write(STDERR_FILENO, "Pelo menos dois argumentos necessarios\n", sizeof("Pelo menos dois argumentos necessarios\n"));
        return ERRO;
    }

    //Declaração e inicialização de variaveis locais 
    int numeroDeAlarmesMAX = atoi(argv[2]);//Variavel que guardara o numero de alarmes passado como o segundo argumento durante a compilação
    int numeroDeAlarmesREC;//Variavel que guardara o retorno da função pesquisaLote (numero de alarmes encontrados )
    int fd[2];//Pipe anonumo para comunicação entre processos
    pid_t pid;//Variavel auxiliar que guardara o retorno da função fork para saber em que processos estamos
    int index;//Variavel auxiliar utilizada par iterar pelos ciclos
    int pipeStatus;//Variavel que guardara o retorno da chama ao sistema pipe 
    struct coordenada alarmes[numeroDeAlarmesMAX];

    numeroDeAlarmesREC = pesquisaLote(argv[1] , alarmes , numeroDeAlarmesMAX);
    pid = fork();//Criação do processo filho
    pipeStatus = pipe(fd);//Criação do pipe para comunicação entre processos

    if(pipeStatus == -1)//Tratametno de erros na criação do pipe anonimo
    {
        write(STDERR_FILENO, "Erro na criação do pipe\n", sizeof("Erro na criação do pipe\n"));
        return ERRO;
    }
    
    if(!pid){//Processo filho responsavel pelo envio do mail. O seu codigo ira ser substituido pelo do script bash
            close(fd[WRITE]);//Fecho do descritor de escrita do pipe anonimo
            dup2(fd[READ], 0);//Copia do descritor de leitura do pipe anonimo para o standart input 
            close(fd[READ]);//Fecho do descritor de leitura do pipe anonimo
            execl("mail.sh","mail.sh","bombeiros@protecao-civil.pt",NULL);/*Substituição do codigo do processo filho pela scrip que envia o email, a 
            partir deste ponto apenas tratamento de dados faz sentido ocorrer*/
            write(STDERR_FILENO, "Erro no envio do email\n", sizeof("Erro no envio do email\n"));//Tratamento de erros 

        }
    else{//Processo pai pela escrita no pipe anonimo das cordenadas que vao sendo encontradas
            close(fd[READ]);//Fecho do descritor de leitura do pipe anonimo

            //Escrita de todas as coordenadas encontrada para o descritor de escrita do pipe anonimo
            for(index = 0; index < numeroDeAlarmesREC; index++)
                write(fd[1],&alarmes[index],sizeof(struct coordenada));
            close(fd[WRITE]);//Fecho do descritor de escrita do pipe anonimo
    }

    return 0;//Termino esperado do programa
}
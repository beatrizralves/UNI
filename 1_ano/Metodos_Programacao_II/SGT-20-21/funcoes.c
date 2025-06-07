#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h> 

#include ".\struct.h"
#include ".\funcoes.h"

void guardar(int i,TAREFA*tarefa){
    FILE* arq = fopen("tarefa.bin", "wb");
    fwrite(&i,sizeof(int),1,arq); 
    
    fwrite(tarefa,sizeof(TAREFA), i,arq );
    fclose(arq);
}
int abrir( TAREFA *tarefa){

  FILE* arq = fopen("tarefa.bin", "rb");
  int i=0;
  if(!arq){
    return i;
  }

    fread(&i,sizeof(int),1, arq);
    fread(tarefa,sizeof(TAREFA), i,arq );  
      
    fclose(arq);
    return i;
}

void menudata(){
        printf("1-Ano.\n");
        printf("2-Mes.\n");
        printf("3-Dia.\n");
        printf("4-Hora.\n");
        printf("5-Minutos.\n");
        printf("0-Fechar.\n");
        printf("Digite um comando para prosseguir.\n");
}
void menu(){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole,
        FOREGROUND_BLUE);
        printf("----------Sistema_de_Gestao_de_Tarefas--------------\n");
        printf("------------------Menu_Principal--------------------\n");
        printf("******************  Tarefa  ************************\n");
        SetConsoleTextAttribute(hConsole,
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("1-Criar tarefa.\n");
        printf("2-Alterar tarefa.\n");
        printf("3-Eliminar tarefa.\n");
        printf("4-Consultar uma tarefa.\n");
        printf("5-Lista de todas as tarefas guardadas no sistema.\n");
        printf("6-Criar um novo tipo de tarefa.\n");
        printf("7-Consulta tipo\n");
        printf("8-Concluir tarefa\n");
        printf("9-Listar Tarefas ja concluidas\n");
        printf("10-Listar Tarefas de Hoje.\n");
        printf("11-Listar Tarefas a partir de um dia.\n");
        printf("12-Listar Tarefas num intervalo de dias.\n");
        SetConsoleTextAttribute(hConsole,
        FOREGROUND_BLUE);
        printf("*****************  Colaborador  *********************\n");
        SetConsoleTextAttribute(hConsole,
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("13-Criar Colaborador.\n");
        printf("14-Lista de Colaboradores.\n");
        printf("15-Eliminar  Colaborador.\n");
        printf("16-Consultar Colaborador.\n");
        printf("17-Alteracao no Colaborador.\n");
        printf("18-Adicionar Colaborador a tarefa.\n");
        printf("0-Fechar Sistema.\n");
        printf("Digite um comando para prosseguir.\n");
    
}

void tipos(char *tipo_array[], size_t n2){//funcao para percorrer o array de tipos e imprimir todos os tipos //n2==numero de strings do array
    printf("----------Tipos_de_tarefas--------\n");
   
    for(int i=0;i<n2;i++){
       printf("[%d]-%s\n",i,tipo_array[i]);
   
    }
}

void criar(int i, TAREFA *tarefa,char *tipo_array[],size_t n2){
	int tipo_escolhido;
    char letra=13;//enter

	printf("------Criacao de Tarefa--------\n"); 
            //tarefa[i].numcolabs=0;
            tarefa[i].verif=1;
            printf("O identificador desta tarefa e: %d\n",i);
            tarefa[i].identificador=i;
            printf("Insira a Descricao da tarefa:\n");
            scanf(" %[^\n]s",(tarefa[i].descricao));
            
            printf("Insira o Tipo da tarefa:\n");
            //printf("tam:%d",n2);//
            tipos(tipo_array,n2);
            scanf("%d",&tipo_escolhido);
            strcpy(tarefa[i].tipo,tipo_array[tipo_escolhido]);

            printf("Insira o Subtipo da tarefa:\n");
            scanf(" %[^\n]s",(tarefa[i].subtipo));
            printf("Insira a data e hora de inicio da tarefa no formato AAAAMMDD-HH:mm\n");
            printf("Ano:");
            scanf("%d",(&tarefa[i].anoi));
            while ( tarefa[i].anoi<0 || tarefa[i].anoi<2020 )
            {
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um ano valido\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Ano:");
                scanf("%d",(&tarefa[i].anoi));
            }
            printf("Mes:");
            scanf("%d",(&tarefa[i].mesi));
             while ( tarefa[i].mesi<0 || tarefa[i].mesi>12 )
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um mes valido\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Mes:");
                scanf("%d",(&tarefa[i].mesi));
            }

            printf("Dia:");
            scanf("%d",(&tarefa[i].diai));
            
            if ( tarefa[i].diai>28 && tarefa[i].mesi==2 )
                {   
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	                SetConsoleTextAttribute(hConsole,
                    FOREGROUND_RED);
                    printf("Insira um dia valido\n");
                    SetConsoleTextAttribute(hConsole,
                    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                    printf("Dia:");
                    scanf("%d",(&tarefa[i].diai));
                }
              while (tarefa[i].diai<0 || tarefa[i].diai>=31 )
            {  
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um dia valido\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Dia:");
                scanf("%d",(&tarefa[i].diai));
            }


            printf("Hora:");
            scanf("%d",(&tarefa[i].horai));
            while ( tarefa[i].horai<0 ||  tarefa[i].horai>=24 )
            {    
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira uma hora valida\n");
                 SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Hora:");
                scanf("%d",(&tarefa[i].horai));
            }
            printf("Minutos:");
            scanf("%d",(&tarefa[i].mini));
            while (tarefa[i].mini<0 || tarefa[i].mini>=60 )
            {    
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira minutos validos\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Minutos:");
                scanf("%d",(&tarefa[i].mini));
            }

            printf("Data de inicio da Tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
            printf("Insira a data e hora do fim da tarefa no formato AAAAMMDD-HH:mm\n");
            printf("Ano:");
            scanf("%d",(&tarefa[i].anof));
            while (tarefa[i].anof<0 || tarefa[i].anof<2020 || tarefa[i].anof<2020)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um ano valido\n");
                 SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Ano:");
                scanf("%d",(&tarefa[i].anof));
            }
            printf("Mes:");
            scanf("%d",(&tarefa[i].mesf));
             while (tarefa[i].mesf<0 || tarefa[i].mesf>12 ||  tarefa[i].mesf<tarefa[i].mesi )
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um mes valido\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Mes:");
                scanf("%d",(&tarefa[i].mesf));
            }

            printf("Dia:");
            scanf("%d",(&tarefa[i].diaf));
            
            if (tarefa[i].diaf>28 && tarefa[i].mesf==2 )
                {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                    printf("Insira um dia valido\n");
                     SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                    printf("Dia:");
                    scanf("%d",(&tarefa[i].diaf));
                }

              while (tarefa[i].diaf<0 || tarefa[i].diaf>=31 || tarefa[i].diaf<tarefa[i].diai)
            {  
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira um dia valido\n");
                 SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Dia:");
                scanf("%d",(&tarefa[i].diaf));
            }

            printf("Hora:");
            scanf("%d",(&tarefa[i].horaf));
            while (tarefa[i].horaf<0 || tarefa[i].horaf>=24   )
            {    
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira uma hora valida\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Hora:");
                scanf("%d",(&tarefa[i].horaf));
            }
            printf("Minutos:");
            scanf("%d",(&tarefa[i].minf));
            while (tarefa[i].minf<0 || tarefa[i].minf>=60 )
            {    
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("Insira minutos validos\n");
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("Minutos:");
                scanf("%d",(&tarefa[i].minf));
            }
             printf("Data de inicio da Tarefa:%d %d %d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
            //if (*tarefa[i].diaf < *tarefa[i].diai )
           // {
            //    printf("Insira datas validas\n");
           // }
            
            printf("Insira o local da tarefa:\n");
            scanf(" %[^\n]s",(tarefa[i].local));
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	        SetConsoleTextAttribute(hConsole,
            FOREGROUND_GREEN);
            strcpy(tarefa[i].estado,"Agendada");

            
            printf("Tarefa criada com sucesso!!\n");
            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();

           //printf("Insira o estado da tarefa(agendada ,concluida):\n");
            //scanf(" %[^\n]s",(tarefa[i].estado));
            system("cls");   

}



void editar( TAREFA *tarefa,char *tipo_array[],size_t n2){
    int cont,edit,tipo_escolhido, d,df;

            printf("------Qual tarefa deseja alterar--------\n");
            scanf("%d",&cont);
            if (strcmp(tarefa[cont].descricao,"null")==0)
            {
                printf("\nNao e possivel alterar uma tarefa que nao foi criada.\n");
                printf("\nCrie a tarefa no menu principal do Sistema.\n");
                printf("\nO Sistema vai voltar ao menu principal automaticamente dentro de 5 segundos. \n");
                Sleep(5000);
                system("cls");
                
            }
            else{
            printf("--------O que deseja alterar na tarefa %d---------\n",cont);
            printf("1 -Descricao\n");
            printf("2 -Tipo\n");
            printf("3 -Subtipo\n");
            printf("4 -Data e hora de inicio da tarefa\n");
            printf("5 -Data e hora do fim da tarefa\n");
            printf("6-Local\n");
            //printf("7 -Estado\n");
            printf("Digite um comando para prosseguir.\n");
            scanf("%d",&edit);
            system("cls");
            
            if (edit<8 && edit>0){
            
                    if (edit==1)
                    {

                        printf("Nova Descricao:\n");
                        scanf(" %[^\n]s",(tarefa[cont].descricao));
                        system("cls");
                        
                    }
                    if (edit==2)
                    {

                        printf("Nova Tipo:\n");
                        tipos(tipo_array,n2);
                        scanf("%d",&tipo_escolhido);
                        strcpy(tarefa[cont].tipo,tipo_array[tipo_escolhido]);
                        
                        
                        system("cls");
                        
                    
                    }
                
                    if (edit==3)
                    {

                        printf("Nova Subipo:\n");
                        scanf("%s",&(tarefa[cont].subtipo));
                        system("cls");
                        
                    }
                    if (edit==4)
                    {
                        printf("Nova data de inicio, o que deseja alterar?:\n");
                        do
                        {
                            menudata();
                            scanf("%d",&d);
                            switch (d)
                            {
                            case 1:
                                printf("Ano:");
                                scanf("%d",(&tarefa[cont].anoi));
                                break;

                            case 2:
                                printf("Mes");
                                scanf("%d",(&tarefa[cont].mesi));
                                break;

                             case 3:
                                printf("Dia");
                                scanf("%d",(&tarefa[cont].diai));
                                break;

                             case 4:
                                printf("Hora");
                                scanf("%d",(&tarefa[cont].horai));
                                break;

                            case 5:
                                printf("Minuto");
                                scanf("%d",(&tarefa[cont].mini));
                                break;
                            
                            
                            default:
                                break;
                            }
                        } while (d);
                        
                        system("cls");
                        
                    }
                    if (edit==5)
                    {

                        printf("Nova data de fim, o que deseja alterar?:\n");
                        do
                        {
                            menudata();
                            scanf("%d",&df);
                            switch (df)
                            {
                            case 1:
                                printf("Ano:");
                                scanf("%d",(&tarefa[cont].anof));
                                break;

                            case 2:
                                printf("Mes");
                                scanf("%d",(&tarefa[cont].mesf));
                                break;

                             case 3:
                                printf("Dia");
                                scanf("%d",(&tarefa[cont].diaf));
                                break;

                             case 4:
                                printf("Hora");
                                scanf("%d",(&tarefa[cont].horaf));
                                break;

                            case 5:
                                printf("Minuto");
                                scanf("%d",(&tarefa[cont].minf));
                                break;
                            
                            
                            default:
                                break;
                            }
                        } while (df);
                        
                        system("cls");
                    }
                    
                    if (edit==6)
                    {

                        printf("Nova local:\n");
                        scanf("%s",&(tarefa[cont].local));
                        system("cls");
                        
                    }
                    
                    /*if (edit==7)
                    {

                        printf("Nova data de inicio:\n");
                        scanf("%s",&(tarefa[cont].estado));
                        
                        
                    }*/

            } else{
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                printf("\n OPCAO INVALIDA!!!\n");//esta mensagem vai aparecer apenas 2 secundo(2000 milisecundos) no programa
                SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                printf("\n O Sistema vai voltar ao MENU_PRINCIPAL\n\n\n");
                Sleep(2000);
                menu();
            }
        }
}

void consultar(TAREFA *tarefa){

    char letra=13;//enter
    int cont;
            printf("--------Qual tarefa quer consultar----------------\n");
            scanf("%d",&cont);

            printf("--------------------tarefa %d---------------------\n",cont);
            printf("Descricao: %s\n",(tarefa[cont].descricao));
            printf("Tipo:%s\n",(tarefa[cont].tipo));
            printf("Subtipo:%s\n",(tarefa[cont].subtipo));
            printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[cont].anoi),(tarefa[cont].mesi),(tarefa[cont].diai),(tarefa[cont].horai),(tarefa[cont].mini));
            printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[cont].anof),(tarefa[cont].mesf),(tarefa[cont].diaf),(tarefa[cont].horaf),(tarefa[cont].minf));
            printf("Local:%s\n",(tarefa[cont].local));
            printf("Estado:%s\n",(tarefa[cont].estado));
            printf("Colaboradores:\n");
            for(int x=0;x<tarefa[cont].numcolabs;x++){
                if(tarefa[cont].utilizador[x]!= NULL){
                    printf("%s\n",tarefa[cont].utilizador[x]);
                }
            }
            printf("--------------------------------------------------\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   

}   
void consultatipo(TAREFA *tarefa, char *tipo_array[], size_t n2, int maxt){

    char letra=13;//enter
    int cont;
    bool vazio=true;
            printf("--------Qual o tipo de tarefa quer consultar----------------\n");
            tipos(tipo_array, n2);
            scanf("%d",&cont);
            for(int i=0;i<maxt;i++){
                if(strcmp(tarefa[i].tipo,tipo_array[cont])==0){
                    vazio=false;
                    printf("--------------------tarefa %d---------------------\n",i);
                    printf("Descricao: %s\n",(tarefa[i].descricao));
                    printf("Tipo:%s\n",(tarefa[i].tipo));
                    printf("Subtipo:%s\n",(tarefa[i].subtipo));
                    printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                    printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                    printf("Local:%s\n",(tarefa[i].local));
                    printf("Estado:%s\n",(tarefa[i].estado));
                    printf("--------------------------------------------------\n"); 

                }
              
                
            }
            if(vazio){
                 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                    printf("Nao existem tarefas deste tipo guardadas no sistema.\n");
                    SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

            }
            printf("******************************************************\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   

}   

void listarconcluidas(TAREFA *tarefa, int maxt){

    char letra=13;//enter
    bool vazio=true;
            
            for(int i=0;i<maxt;i++){
                if (strcmp(tarefa[i].estado,"Concluido")==0 )//imprime todas as tarefas concluidas guardadas no sistema
                {
                    vazio=false;
                printf("--------------------tarefa %d-----------------------\n",i);
                printf("Descricao: %s\n",(tarefa[i].descricao));
                printf("Tipo:%s\n",(tarefa[i].tipo));
                printf("Subtipo:%s\n",(tarefa[i].subtipo));
                printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                printf("Local:%s\n",(tarefa[i].local));
                printf("Estado:%s\n",(tarefa[i].estado));
                }
              
                
            }
            if(vazio){
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED);
                    printf("Nao existem tarefas deste tipo guardadas no sistema.\n");
                    SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            }
            printf("******************************************************\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   

}   


void eliminar(int i, TAREFA *tarefa){
    int limpa;
    int apagados=0;
    char op;

            printf("-------Que tarefa deseja eliminar-----\n");
            scanf("%d",&limpa); 
               
            
            printf("Deseja mesmo eliminar a tarefa? S/N \n");
            scanf(" %c",&op);
            if (op=='n' || op=='N')
            {
                printf("OPERACAO CANCELADA.");
            }
            else if(op=='s' || op=='S'){
            for(int i=limpa;i<100;i++){
                    tarefa[limpa]=tarefa[101];
                    apagados++;
                }  
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	            SetConsoleTextAttribute(hConsole,
                FOREGROUND_GREEN);
                printf("Tarefa eliminada com sucesso!!\n");
                  SetConsoleTextAttribute(hConsole,
                FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            }
            else{
                printf("caracter invalido.\n");
            }
         
            guardar(i,tarefa);
            
}

void lista(char letra,TAREFA *tarefa){
        printf("--------------Lista_de_todas_as_tarefas_------------\n");
            bool vazio = true;
            letra=getchar();
            //printf("tam:%d", strlen(tipo));//
            for (int i = 0; i < 100 ; i++)//perccorrer todas as tarefas 
            {
                if (tarefa[i].verif==1 )//imprime todas as tarefas guardadas no sistema
                {
                    vazio=false;
                printf("--------------------tarefa %d-----------------------\n",i);
                printf("Descricao: %s\n",(tarefa[i].descricao));
                printf("Tipo:%s\n",(tarefa[i].tipo));
                printf("Subtipo:%s\n",(tarefa[i].subtipo));
                printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                printf("Local:%s\n",(tarefa[i].local));
                printf("Estado:%s\n",(tarefa[i].estado));
            
            
                }
            }
            if(vazio==true){ //verifica que nao existem tarefas no sistema
                printf("Nao ha tarefas no sistemas.\n");
                }
             
            printf("---------------------------------------------\n"); 
            printf("Pressione enter para voltar ao menu principal");
            getchar();
            system("cls");

}

void concluir(TAREFA *tarefa){
int conc;
char op;
    printf("-------Que tarefa deseja concluir-----\n");
    scanf("%d",&conc); 
    printf("--------------------tarefa %d---------------------\n",conc);
            printf("Descricao: %s\n",(tarefa[conc].descricao));
            printf("Tipo:%s\n",(tarefa[conc].tipo));
            printf("Subtipo:%s\n",(tarefa[conc].subtipo));
            printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[conc].anoi),(tarefa[conc].mesi),(tarefa[conc].diai),(tarefa[conc].horai),(tarefa[conc].mini));
            printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[conc].anof),(tarefa[conc].mesf),(tarefa[conc].diaf),(tarefa[conc].horaf),(tarefa[conc].minf));
            printf("Local:%s\n",(tarefa[conc].local));
            printf("Estado:%s\n",(tarefa[conc].estado));
            printf("--------------------------------------------------\n"); 

            printf("Deseja mesmo concluir a tarefa? S/N \n");
            scanf(" %c",&op);
            if (op=='n' || op=='N')
            {
                printf("OPERACAO CANCELADA.");
            }
            else if(op=='s' || op=='S'){
                strcpy(tarefa[conc].estado,"Concluido");

                printf("Tarefa terminada com sucesso!!\n");
                printf("Pressione enter para voltar ao menu principal");
                getchar();
           

            }
            else {
                printf("caracter invalido.\n");
            }
            
}

void tarefashoje(TAREFA *tarefa, int diahj, int meshj, int anohj, int maxt){
    printf("--------------Lista_das_tarefas_de_HOJE------------\n");
    char letra=13;//enter
    bool vazio=true;
    for(int i=0;i<maxt;i++){
                if(tarefa[i].diai==diahj && tarefa[i].mesi==meshj && tarefa[i].anoi==anohj){
                    vazio=false;
                    printf("--------------------tarefa %d---------------------\n",i);
                    printf("Descricao: %s\n",(tarefa[i].descricao));
                    printf("Tipo:%s\n",(tarefa[i].tipo));
                    printf("Subtipo:%s\n",(tarefa[i].subtipo));
                    printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                    printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                    printf("Local:%s\n",(tarefa[i].local));
                    printf("Estado:%s\n",(tarefa[i].estado));
                    printf("--------------------------------------------------\n"); 

                }
              
                
            }
            if(vazio){
                    printf("Nao existem tarefas para hoje.\nDesfrute do seu dia :)\n");

            }
            printf("******************************************************\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   
}
void tarefasapartir(TAREFA *tarefa, int maxt){

    int dia, mes, ano;
    char letra=13;//enter
    bool vazio=true;
    printf("--------------Lista_das_tarefas_a_partir_da_data:------------\n");
    printf("Dia:");
    scanf("%d",&dia);
    printf("\n");
    
    
    printf("Mes:");
    scanf("%d",&mes);
    printf("\n");


    printf("Ano:");
    scanf("%d",&ano);
    printf("\n");

    for(int i=0;i<maxt;i++){
                if(tarefa[i].diai >= dia && tarefa[i].mesi >= mes && tarefa[i].anoi >= ano && tarefa[i].verif!=0){
                    vazio=false;
                    printf("--------------------tarefa %d---------------------\n",i);
                    printf("Descricao: %s\n",(tarefa[i].descricao));
                    printf("Tipo:%s\n",(tarefa[i].tipo));
                    printf("Subtipo:%s\n",(tarefa[i].subtipo));
                    printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                    printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                    printf("Local:%s\n",(tarefa[i].local));
                    printf("Estado:%s\n",(tarefa[i].estado));
                    printf("--------------------------------------------------\n"); 

                }
              
                
            }
            if(vazio){
                    printf("Nao existem tarefas para o periodo escolhido.\n");

            }
            printf("******************************************************\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   
}
void tarefasintervalo(TAREFA *tarefa, int maxt){
    int dia1, mes1, ano1, dia2, mes2 ,ano2;
    char letra=13;//enter
    bool vazio=true;
    printf("--------------Lista_das_tarefas_entre_as_datas:------------\n");
    printf("Intervalo de tempo-Desde:\n");
    printf("De_");
    printf("Dia:");
    scanf("%d",&dia1);
    printf("\n");
    printf("Mes:");
    scanf("%d",&mes1);
    printf("\n");
    printf("Ano:");
    scanf("%d",&ano1);
    printf("\n");

    printf("Ate_");
    printf("Dia:");
    scanf("%d",&dia2);
    printf("\n");
    printf("Mes:");
    scanf("%d",&mes2);
    printf("\n");
    printf("Ano:");
    scanf("%d",&ano2);
    printf("\n");

    for(int i=0;i<maxt;i++){
                if(tarefa[i].diai >=dia1 &&  tarefa[i].mesi >= mes1  && tarefa[i].anoi  >= ano1 && tarefa[i].verif!=0){
                    if(tarefa[i].diaf<=dia2 && tarefa[i].mesf <= mes2 && tarefa[i].anof <=ano2 && tarefa[i].verif!=0  )
                    vazio=false;
                    printf("--------------------tarefa %d---------------------\n",i);
                    printf("Descricao: %s\n",(tarefa[i].descricao));
                    printf("Tipo:%s\n",(tarefa[i].tipo));
                    printf("Subtipo:%s\n",(tarefa[i].subtipo));
                    printf("Data e hora de inicio da tarefa:%d%d%d-%d%d\n",(tarefa[i].anoi),(tarefa[i].mesi),(tarefa[i].diai),(tarefa[i].horai),(tarefa[i].mini));
                    printf("Data e hora do fim da tarefa:%d%d%d-%d%d\n",(tarefa[i].anof),(tarefa[i].mesf),(tarefa[i].diaf),(tarefa[i].horaf),(tarefa[i].minf));
                    printf("Local:%s\n",(tarefa[i].local));
                    printf("Estado:%s\n",(tarefa[i].estado));
                    printf("--------------------------------------------------\n"); 

                }
              
                
            }
            if(vazio){
                    printf("Nao existem tarefas para o periodo escolhido.\n");

            }
            printf("******************************************************\n"); 
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");   
}



//********************************************************************************************************
//**************************************funcoes para colaboradores***************************************
//********************************************************************************************************

colab* addColab(int colabID, char nome[],char telemo[],char datanasci[], char email[],char morada[]){
    colab* novocolab;
    novocolab = malloc(sizeof(colab));
    novocolab->colabID = colabID;
    strcpy(novocolab->nome,nome);
    strcpy(novocolab->telemovel,telemo);
    strcpy(novocolab->datanascimento,datanasci);
    strcpy(novocolab->mail,email);
    strcpy(novocolab->morada,morada);

    novocolab->next = NULL;

    return novocolab;
}

colab* addList(colab* list, colab* u){                       //ordena alfabeticamente      
    if (list!=NULL){
        if(strcmp(u->nome,list->nome)>0){
            list->next = addList(list->next,u);
        }else{
            u->next = list;
            list = u;
        }
    }else{
            list = u;
    }
    return list;
}

void printList(colab* list){
    
    if (list!=NULL){
    //printList(list->next);    
    printf("*********************ID:%d*********************\n ",list->colabID);
    //printf("IDColab:%d;\n ",list->colabID);
    printf("Nome:%s\n ",list->nome);
    printf("Telemovel:%s\n ",list->telemovel);
    printf("Data de Nascimento:%s\n ",list->datanascimento);
    printf("Mail:%s\n ",list->mail);
    printf("Morada:%s\n",list->morada);
    printf("***********************************************\n");

    printList(list->next);
    }
    
  
}
// função que procura colab na lista

colab* procura(colab* list, int ID) {
    colab* p = list;
    while (p!=NULL && p->colabID!=ID)
        p = p->next;
    return p;
}

void printcolab(colab* list){
   
    
    printf("*******COLABORADOR %d**************\n ",list->colabID);  
    //printf("id:%d, ",list->colabID);
    printf("Nome:%s\n ",list->nome);
    printf("Telemovel:%s\n ",list->telemovel);
    printf("Data de Nascimento:%s\n ",list->datanascimento);
    printf("Mail:%s\n ",list->mail);
    printf("Morada:%s\n",list->morada);
    printf("***********************************\n");
   
}

// função que remove o colab da lista

colab* remocao(colab* list, int ID,int flag) {
    colab* p, *prev;
    for (prev=NULL,p=list; p!=NULL && p->colabID!=ID; p=p->next)
        prev=p;

        if (p != NULL){        /** colaborador com colabID=ID existe na lista        */
        if (prev != NULL)  /** colaborador com cOLABID=ID nao e' cabeca da lista */
        prev->next = p->next;
        else{
            if(flag){
                    
               free(list);
            return NULL;

            }
            else{
            list = p->next; /** nova cabeca da lista                    */
           free(p);           /** liberta a memoria do elemento a remover */
           }
        }
    }
     return list;            /** devolve a cabeca da lista atualizada    */
}

// função que retorna o tamanho da lista
int tamanho_lista(colab* list){

   
	colab *aux = list;
	int tam = 0;
	while(aux != NULL)
	{
		tam++;
		aux = aux->next;
	}
	return tam;
}
void ligarcolabtar(TAREFA *tarefa, colab *colaboradores){
            int tar,col;
            int numcolabs=0;
            colab *aux;
            printf("Insira o ID da tarefa que quer adicionar colaboradores:\n");
            scanf("%d",&tar);
            
            numcolabs=tarefa[tar].numcolabs;
            printf("Lista de colabs\n");
            printList(colaboradores);
            printf("Insira o nº do colaborador que quer associar a esta tarefa:\n");
            scanf("%d", &col);
            aux=procura(colaboradores,col);


            strcpy(tarefa[tar].utilizador[numcolabs],aux->nome);
            //numcolabs++;
            tarefa[tar].numcolabs++;

}
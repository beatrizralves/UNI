//SGT-2020-21
//PL1-GRUP03( BEATRIZ ALVES)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>//biblioteca para utilizar a funcao sleep
#include <stdbool.h>//biblioteca para usar as funcoes boleanas(verdadeiro e falso)
#include <time.h> 

#include ".\funcoes.h"
#include ".\funcoes.c"
#include ".\struct.h"

#define MAXT 100
#define a 40
#define b 35
#define t 10
#define d 9

void write_file(colab* list, int colab_total){

   FILE *fich;
   fich = fopen ("Colaboradores.txt", "w");

   if(fich != NULL){

        if(list){
           
        fprintf(fich,"%d\n",colab_total);

           while(list!=NULL){
            
            fprintf(fich,"%d\n",list->colabID); 
            fprintf(fich,"%s\n",list->nome);
            fprintf(fich,"%s\n",list->telemovel);
            fprintf(fich,"%s\n",list->datanascimento);
            fprintf(fich,"%s\n",list->mail);
            

            if(list->next==NULL){

                fprintf(fich,"%s",list->morada);

            }

            else{

                fprintf(fich,"%s\n",list->morada);

            }

            list=list->next;

            }

        }     

    }

    fclose(fich);

}

colab* read_file(colab* list, int* ncolab){

    FILE* read;
    colab* auxi;
    int colab_total=*ncolab;

    read = fopen("Colaboradores.txt", "r");

    if(read){

        fscanf(read,"%d",&colab_total);

        while(!feof(read)){

            auxi=malloc(sizeof(colab));
            //fgets
            fscanf(read,"%d",&auxi->colabID);
            fscanf(read," %[^\n]s",auxi->nome);
            fscanf(read,"%s",auxi->telemovel);
            fscanf(read,"%s",auxi->datanascimento);
            fscanf(read,"%s",auxi->mail);
            fscanf(read," %[^\n]s",auxi->morada);
             

            auxi->next = NULL;
            list = addList(list, auxi);

        }
    }

    fclose(read);
    *ncolab=colab_total;
    return list;

}


int main(){
  
  char letra=13;//enter
  //*********variaveis para tarefas************
    FILE *id_arq;
    FILE *tarefa_arq;

    int escolha;
    int i=0;   //contador tarefas
    TAREFA tarefa[MAXT];//array de tarefas
    i=abrir(tarefa);


    //ponteiro para struct que armazena data e hora  
    struct tm *data_hora_atual;     
  
    //variável do tipo time_t para armazenar o tempo em segundos  
    time_t segundos;
  
     //obtendo o tempo em segundos  
    time(&segundos);   
  
    //para converter de segundos para o tempo local  
    //utilizamos a função localtime  
    data_hora_atual = localtime(&segundos);  

    
   
  //*********variaveis para colaboradores************
   
    char mail[b];
    char morada[b];
    char name[a];
    char tele[t];
    char datnas[d];
    int ID;//consulta
    int ident;//eliminar
    char opcao;// verificar sim ou nao
    int alterar;//menu alterar
    int cid;//alterar
    int flag=0;//para guardar no ficheiro 
    colab* Colaborador;
    colab* aux;
    colab* colaboradores=NULL;
    int ncolab=0;//contador colab
   
//*************fim das declaracoes de variaveis*****************************

    for (int i=0; i<100; i++){ //verificar que o array tarefa está vazio
        tarefa[i].verif=0;
    }

    char *tipo_array[]={"Escolar","Profissional","Pessoal","Familiar","Lazer"};//array para os tipos

    //size_t n = sizeof(tipo_array);//numero de bytes do array
    size_t n2 = sizeof(tipo_array) / sizeof(tipo_array[0]);//numero de strings do array

//*****************************************************
    colaboradores = read_file(colaboradores,&ncolab);

 do
    { 
     menu();
     scanf("%d", &escolha);
     system("cls");
     switch(escolha)
        {

        case 1:
         criar(i,tarefa,tipo_array,n2);//chama a funcao criar
            i++;
             tarefa_arq=fopen("tarefa.bin", "ab");//abre o ficheiro
            if (tarefa_arq==NULL)
            {
                printf("Impossivel abrir ficheiro.");

            }else{
            
            guardar(i,tarefa);
           
            fclose(tarefa_arq);
            }

        break;

        case 2:
        editar( tarefa,tipo_array,n2);
        guardar(i,tarefa);

        break;

        case 3:
        eliminar(i,tarefa);
        guardar(i,tarefa);

        break;

        case 4:
        consultar(tarefa);
        
        break;

        case 5:
        abrir(tarefa);
        lista(letra,tarefa);
        
        break;


        case 6:
                printf("Indique o novo tipo de tarefa que deseja criar.\n");
             
                tipo_array[n2] = malloc(n2 * sizeof(char));//reservar espaço em memória no array de tipos para o novo tipo
                scanf("%s", tipo_array[n2]);
                n2++;//aumentar o tamanho do array +1
                tipos(tipo_array,n2);
                guardar(i,tarefa);
                system("cls");
            
        break;

        case 7:
              consultatipo(tarefa,tipo_array,n2,MAXT);
            
        break;

        case 8:
            concluir(tarefa);
            guardar(i,tarefa);
            system("cls");

        break;
        
        case 9:


        listarconcluidas(tarefa,MAXT);

        break;
        
        case 10:
     
        tarefashoje(tarefa,data_hora_atual->tm_mday, data_hora_atual->tm_mon+1,data_hora_atual->tm_year+1900, MAXT);
        break;

        case 11:
        tarefasapartir(tarefa,MAXT);
        break;
    
        case 12:
        tarefasintervalo(tarefa,MAXT);
        break;

          
       

        //**************************************************************
        //************************colab*********************************
        //**************************************************************
        case 13:
            //criar colab
            printf("************Criacao de Colaborador************\n");
            printf("ID do colaborador:%d\n",ncolab);
            printf("Intoduza o nome do colaborador\n");
            scanf(" %[^\n]s",name);
            printf("Intoduza o numero de telemovel do colaborador\n");
            printf("+351 ");
            scanf("%s",tele);
            while (strlen (tele)>9 ||strlen (tele)<9 )
            {
                printf("Intoduza um numero de telemovel valido\n");
                printf("+351 ");
                scanf("%s",tele);
            }
            printf("Intoduza a sua data de nascimento neste formato:ddmmaaaa\n");
            scanf("%s",datnas);

             while (strlen (datnas)>8 ||strlen (datnas)<8 )
            {
                printf("Intoduza uma data de nascimento valida neste formato:ddmmaaaa\n");
                scanf("%s",datnas);
            }

            printf("Intoduza o mail do colaborador\n");
            scanf("%s",mail);
            printf("Intoduza a morada do colaborador\n");
            scanf(" %[^\n]s",morada);
         
            Colaborador=addColab(ncolab++,name,tele,datnas,mail,morada);
            colaboradores=addList(colaboradores, Colaborador);
            system("cls");
            
        break;
        

        case 14:
        //listar colabs
        printf("Numero de Colaboradores guardados no sistema: %d\n", tamanho_lista(colaboradores));
        printf("******** LISTA_DE_COLABORADORES *************\n");
        printList(colaboradores);
        printf("_____________________________________________\n");
        printf("Pressione enter para voltar ao menu principal"); 
        letra=getchar();
        getchar();
        system("cls");
        
        break;
        

        case 15:
        //elminar colab
        printf("************Remocao de Colaborador************\n");
        printf("Insira o id do colaborador que deseja eliminar:");
        scanf("%d",&ident);
        aux  = procura(colaboradores, ident);
        if(aux != NULL) {
            printf("COLABORADOR ENCONTRADO:\n ");
            printcolab(aux);
            printf("Deseja mesmo eliminar o colaborador? S/N \n");
            scanf(" %c",&opcao);
            if (opcao=='n' || opcao=='N')
            {
                printf("                            Operacao Cancelada.\n");
                printf("*****************************************************************************\n");
                printf("\nO Sistema vai voltar ao menu principal automaticamente dentro de 5 segundos. \n");
                Sleep(5000);
                system("cls");
            }
            else if(opcao=='s' || opcao=='S'){
                if (colaboradores->next==NULL)
                {
                    flag=1;
                }
                
            colaboradores = remocao(colaboradores, ident,flag);
            printf("REMOVEU O COLABORADOR COM ID=%d\n", ident);
            printf("**********************************************\n");
            printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");
         
            }  
            else{
                printf("Caracter invalido.\n");
                printf("\nO Sistema vai voltar ao menu principal automaticamente dentro de 5 segundos. \n");
                Sleep(5000);
                system("cls");
            }
          }
        if(aux == NULL){
        printf("Colaborador com ID=%d NAO ENCONTRADO!\n", ID);
        printf("----------------------------------------------\n");
        printf("Pressione enter para voltar ao menu principal"); 
            letra=getchar();
            getchar();
            system("cls");
        }    
        
            flag=0;
            
        break;

        case 16:
        //procurar colab
        /*printf("Insira o nome do colaborador que deseja procura:");
        scanf(" %[^\n]s",nomep);
        printcolab(procura(Colaborador,"nomep"));*/
        printf("************Consulta de Colaborador************\n");
        printf("Insira o id do colaborador que deseja procura:");
        scanf("%d",&ID);
        aux  = procura(colaboradores, ID);
        if(aux != NULL) {
        printf("COLABORADOR ENCONTRADO:\n ");
        printcolab(aux);
        }
        else
        printf("Colaborador com ID=%d NAO ENCONTRADO!\n", ID);
        printf("----------------------------------------------\n");
        printf("Pressione enter para voltar ao menu principal"); 
        letra=getchar();
        getchar();
        system("cls");

        break;
        

        case 17:
        printf("************Alteracao no Colaborador************\n");
        printf("------Qual colaborador deseja alterar--------\n");
            scanf("%d",&cid);
            aux  = procura(colaboradores,cid);
            if(aux == NULL){
                printf("Colaborador inexistente.\n");
                printf("---------------------------------------------\n");
                printf("Pressione enter para voltar ao menu principal"); 
                letra=getchar();
                getchar();
                system("cls");
            }
            else{
            printf("--------O que deseja alterar no colaborador %d---------\n",cid);
            printcolab(aux);

            printf("1 -Nome\n");
            printf("2 -Telemovel\n");
            printf("3 -Morada\n");
            printf("4 -E-mail\n");
            printf("5 -Data de nascimento\n");
            printf("Digite um comando para prosseguir.\n");
            scanf("%d",&alterar);
            system("cls");
           
            if (alterar<6 && alterar>0){
                    if (alterar==1){
                        //printf("Nome atual:%s\n",colaboradores->nome);
                        printf("Intoduza o novo nome do colaborador\n");
                        scanf(" %[^\n]s",aux->nome);
                        
                    }
                     if (alterar==2){
                        //printf("Numero de telemovel atual:%s\n",aux->telemovel); 
                        printf("Intoduza o novo numero de telemovel do colaborador\n");
                        scanf("%s",aux->telemovel);
                    }
                     if (alterar==3){
                        //printf("Morada atual:%s\n",colaboradores->morada); 
                        printf("Intoduza a nova morada do colaborador\n");
                        scanf(" %[^\n]s",aux->morada);
                    }
                     if (alterar==4){
                        //printf("Mail atual:%s\n",colaboradores->mail); 
                        printf("Intoduza o novo mail do colaborador\n");
                        scanf("%s",aux->mail);
                    }
                    if (alterar==5)
                    {
                        //printf("Data de nascimento atual:%s\n",colaboradores->datanascimento); 
                        printf("Intoduza uma nova data formato ddmmaaaa\n");
                        scanf("%s",aux->datanascimento);
                        
                    }
                    
               }
            }
        
            system("cls");
            
        break;
       
        case 18:
        ligarcolabtar(tarefa,colaboradores);
                printf("Colaborador adicionado com sucesso a tarefa.\n");
                printf("---------------------------------------------\n");
                printf("Pressione enter para voltar ao menu principal"); 
                letra=getchar();
                getchar();
                system("cls");
        break;    

        case 0:
            printf("\n Obrigada por ter usado o nosso sistema,volte sempre!! :)\n\n\n");
            break;

        default:
            printf("Opcao invalida.\n");
            printf("\n O Sistema vai voltar ao MENU_PRINCIPAL automaticamente dentro de 2 segundos.\n\n\n");
            Sleep(2000);
            menu();
            system("cls");
        }

    }while (escolha);

    guardar(i,tarefa);
    write_file(colaboradores,ncolab);
return 0;
}

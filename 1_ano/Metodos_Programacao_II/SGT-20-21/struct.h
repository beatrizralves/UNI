#ifndef HEADER_FILE
#define HEADER_FILE


#define desc 100
#define est  20
#define loc  40
#define ts   15
#define tl   10
#define ml   35
#define sex  3
#define dat  9





typedef struct Tarefa
{
   int identificador;
   char descricao[desc];
   int  verif;
   char tipo[ts];
   char subtipo[ts];
   int  anoi;
   int  diai;
   int  mesi;
   int  horai;
   int  mini;
   int  anof;
   int  diaf;
   int  mesf;
   int  horaf;
   int  minf;
  // char datahi[13];// 05/04/2021-0926  202104050930
  // char datahf[13];// 202104051020
   char local[loc];
   char estado[est];
   char utilizador[100][20];
   int numcolabs;
}TAREFA;


typedef struct Colaborador{
   int colabID;
   char nome[loc];//40
   char telemovel[tl];//10
   char datanascimento[dat];//9
   char mail[ml];//35
   char morada[ml];//35
  
   //char afinidade;
   
   struct Colaborador*next;

}colab;


#endif
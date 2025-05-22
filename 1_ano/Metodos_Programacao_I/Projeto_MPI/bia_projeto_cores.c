//Project MIETI 2020/2021 MPI
//Made by:Beatriz Alves 
//Final Version

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>//library that uses bool function
#include <windows.h>
#include <time.h>//random number

//function that check uppercase 


bool maiusculas(char* str) {

    int   i;
    bool procura_maiusculas = false;
   
    for (int i = 0; str[i] != '\0'; i++) {
    	
         procura_maiusculas= procura_maiusculas || (str[i] >= 'A' && str[i] <= 'Z');

    }

    return (procura_maiusculas);
}

bool apenas_letras(char* str) {

    int   i;
    
    bool apenas_letras = false;
    for (int i = 0; str[i] != '\0'; i++) {
    	apenas_letras = apenas_letras || (str[i] >= '0' && str[i] <= '9' && str[i] >=122  && str[i] <=97 );

    }

    return (apenas_letras);
}
//funcion swap

char trocas(char a[], char min) 
{ 
	char max = 'z';
	int novoi;
    for (int i=1; i<strlen(a); i++){
        if (a[i] > min &&  a[i]<= max){
           // min = a[i];
			//a[i] = a[0];
		 	max=a[i];
			novoi=i;
		} 

	}
	a[novoi] = a[0];
	
    return max; 
} 

void principal(char string[50]){
	int A,B,tam;
	char temp;
	
	if(strlen(string)>50){
		printf("Erro excedeu tamanho maximo.(50 caracteres )\n\n");
		printf("Insira uma sequencia:\n");
		scanf("%s",string);
		principal(string);

	}
	
	if (strcmp(string,"#")!=0){

	
		if(maiusculas(string) ){
	    printf("ERRO!!!\n\n");
	    printf("Insira uma sequencia:\n");
		scanf("%s",string);
		principal(string);

	    }
	    if(apenas_letras(string) ){
	    printf("ERRO!!!\n\n");
	    printf("Insira uma sequencia:\n");
		scanf("%s",string);
		principal(string);

	    }
		
		tam=strlen(string);
		B=tam-1;//POSICAO DA ULTIMA LETRA DA STRING
		A=tam-2;//POSICAO DA PENULTIMA LETRA DA STRING

		while(string[A]>=string[B]){//ENQUANTO LETRA DA POSCICAO A FOR MAIOR OU IGUALQUE A LETRA DA POSICAO B
		
		 	A=A-1;//A VAI TOMAR O VALOR DA POSCICAO ANTERIOR E B TAMBEM
		 	B=B-1;
		 	//printf("A:%c::::B:%c",string[A],string[B]);
		}
		
	    if(string[A]<string[B] && B!=0) {//
	    
		 	//atalho para strings de duas letras
			char strA=string[A];
		 	if(strlen(string)==2){
		 		string[A]=string[B];
		 		string[B]=strA;
		 		printf("%s\n\n",string);
		 		printf("Insira uma sequencia:\n");
				scanf("%s",string);
				principal(string);
			 }
			 //strings com mais de duas letras
			else{
			 	char novoA;
			 	//printf("A->%d,,,B->%d",A, B);
			 	novoA=trocas(&string[A], string[A]);//o & � utilizado para a funcao so utilizar as letras que estao a direita da letra de A
				string[A]=novoA;
				
				//puts in alphabetic order	
			 	for (int i = A+1; i < tam-1; i++) {
					for (int j = i+1; j < tam; j++) {
						if (string[i] > string[j]) {
							temp = string[i];
							string[i] = string[j];
							string[j] = temp;
						}
					}
				}
			 	printf("%s\n\n",string);
			 	printf("Insira uma sequencia:\n");
				scanf("%s",string);
				principal(string);
			 }
		}
		
		if(B==0){

			printf("Sem sucessooooor\n\n");
			printf("Insira uma sequencia:\n");
			scanf("%s",string);
			principal(string);
		}
	}
	else {
		exit (0);
	}
}


int main(){
	char string[50], name[20];
	int A,B,tam,a;
	//header    
  	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,
    FOREGROUND_GREEN);
    printf(".................          PROJETO        .........................\n" );
    printf("...................................................................\n" );
	printf("| d888888b d888b.        .o88b.  .d88b.  d8888b. d88888b .d8888.  |\n");
	printf("|  `88'   88   `8D      d8P  Y8 .8P  Y8. 88  `8D 88'     88'  YP  |\n");
	printf("|   88    88     88     8P      88    88 88   88 88ooooo `8bo.    |\n");
	printf("|   88    88     88     8b      88    88 88   88 88^^^^^   `Y8b.  |\n");
	printf("|  .88.   88   .8D      Y8b  d8 `8b  d8' 88  .8D 88.     db   8D  |\n");
	printf("| Y888888P Y8888D'       `Y88P'  `Y88P'  Y8888D' Y88888P `8888Y'  |\n");
	printf("|.................................................................|\n" );
	printf("\n");

	srand ( time(NULL) );
    a=rand()%340+1;
    printf("Welcome USER %i!\n\n",a);
	
	SetConsoleTextAttribute(hConsole,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

	SetConsoleTextAttribute(hConsole,
    FOREGROUND_RED);
	printf("Exit Program Press # \n\n");
	SetConsoleTextAttribute(hConsole,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

	printf("Input string:\n");
	
	scanf("%s",string);
	principal(string);

	return 0;
	
}




//Este modulo contera as funções secundarias que serão usadas nos outros modulos (como por exemplo readln, reachar entre outras)

//Header-files 
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

//Defines
#define ERRO -1         //Utilizado para um codigo mais claro na leitura
#define BUFFER_SIZE 1024

/*Função readchar - responsavel por ler um caracter de cada vez de um ficheiro (o descritor de ficheiro do qual sera realizada a leiturasera o 
argumento recebido)*/
int readchar(int fd)
{
    //Declaração e inicialização de variaveis locais 
    char buffer[1]; //Buffer que ira guardar o caracter lido
    int nOcR; //nOcR stands for number of characters read, guardara o valor de retorno da chamada ao sistema read 

    nOcR = read(fd, buffer, 1); //Le um caracter do ficheiro com o descritor fd para o buffer

    if(nOcR > 0)//Algo foi lido (read retorna o numero de caracteres lidos neste caso 1 ou um numero negativo em caso de erro)
        return buffer[0];//Retorna o que gravou, que foi guardado em buffer (1 caracter) - posição 0 do array
    else
        return ERRO;//Para sair do ciclo while na função readln pois não se ira verificar c > 0; (return -1 => c == '-1')
}

/*Função readln - responsavel por ler uma linha de cada vez de um ficheiro (o descritor de ficheiros sera o argumento recebido) - ira retornar o numero
de caracteres lidos na respetiva linha*/
ssize_t readln(int fd, char *line, size_t maxLength)/*fd - descritor do ficheiro a ser lido, line buffer para onde sera lida a linha, maxLength mumero
maximo de caracteres a serem lidos*/
{
    //Declaração e inicialização de variaveis locais 
    int index = 0; //Variavel auxiliar utilizada para iterar pelo array de caracteres (string) linha
    char c = readchar(fd);//Variavel que ira guardar (até verificação) o caracter lido pela função 'readchar'

    while(index < maxLength - 1 && c > 0 && c != '\n'){/*Enquanto não receber o caracter 'newLine' ("\n") ou um valor de erro da função readchar ira 
    continuar a leitura, maxLength - 1 pois o array de caracteres começa na posição 0*/
        line[index] = c; //Adiciona o caractere lido e verificado na primeria posição não preenchida do array de caracteres 
        c = readchar(fd); //Ira ler o proximo caracter do ficheiro 
        index++; //Atualiza a posição de index (aponta sempre para a primeira posição não preenchida do array)
    }
    line[index] = '\0'; //Adição do caracter terminador depois de todos os caracteres validos da linha atual terem sido lidos 
    return index; //Retorno do numero de caracteres lidos + 1 (caracter terminador) - tamanho efetivo da linha
}

/*Função myStrlen - similiar ao que faz a função strlen da biblioteca string.h, mas sem uutilizar funções de alto nivel || Limitações: apenas funciona
se o utilizador for bem comportado (como é o caso) e as suas strings terminarem com o caracter terminador de strings '\0'*/
int myStrlen (char * str)
{
    //Declaração e inicialização de variaveis locais
    int length = 0;//Variavel auxiliar que ira guardar o tamnho da string em questão e ser o retorno da função

    while(*str != '\0'){//Enquanto não encontramos o caracter terminador de string
        ++length;//Iremos incrementar o tamanho da mesma
        ++str;//E verificar a posição seguinte (apenas possivel pois os arrays são guardados em posições contiguas de memoria)
    }
    return length;//Tamanho da string em causa
}

//Função numberOFdigits - ira receber como argumento um inteiro e devolver o numero de digitos do mesmo
int numberOFdigits (int numero)
{
    //Declaração e inicialização de variaveis locais 
    int contador = 0;//Variavel auxiliar que ira guardar o numero de digitos do numero passado como argumento à função

    if(numero == 0)//Caso o numero seja 0 estamos perante um caso limite por isso iremos definir manualmente o numero de digitos do numero passado
        contador = 1;
    else{//Todos os restantes inteiros positivos podem ser tratados genericamente
        while (numero > 0){//Iremos realizar iterações enquanto o numero não for nulo 
            contador++;//Incremento do numero de digitos pois todos os numeros 
            numero /= 10;//Divisão do numero pr 10 (exemplo:. 47 / 10 = 4.7 | visto se tratar de um inteiro apenas ficaremos com 4)
        }
    }

    return contador;//Retorno do numero de digitos do numero em questão
}


//Função storageARRAY - recebe como argumento um inteiro e um array, onde esse inteiro sera separado nos seus diferentes digitos e os mesmos serão ordenados no array do MSB para o LSB
void storageARRAY (int numero, int *array)
{
    //Deckaração e inicialização de variaveis locais 
    int nOd = 0;//Variavel auxiliar que ira iterar sobre o array de inteiros mantendo apontada sempre a primeira posição não preenchida (numero de digitos)
    int index;

    if(numero == 0)//Caso o numero passado à função seja '0' estaremos perante um caso limite que precisa de ser tratado manualmente
        array[nOd] = numero;//Passagem do unico digito para a posição inicial do array
    else{//Todos os outros casos serão tratados genericamente
        while (numero > 0){//Iremos realiar iterações enquanto o numero não for negativo
            array[nOd] = numero % 10;//Exemplo:. 47 % 10 = 7 (resto da divisão inteira)
            numero /= 10;//Explicação igual á fornecida na mesma linha da função numberOFdigits
            nOd++;//Incremento da posição de escrita no array
        }
    }
    
    //Iremos inverter a ordem do array 
    for (index = 0; index < nOd / 2; index++){/*penas iteramos sobre metade do tamanho do array pois iremos "dobrar este sobre si proprio", a ultima posição passa a ser a primera, a penulta a 
    segunda e assim sucessivamente até encontramos o meio do array ou em caso de array par a posição imediatamente antes do exato meio e imediametne após*/
        int temp = array[index];
        array[index] = array[nOd - index - 1];
        array[nOd - index - 1] = temp;
    }

    /*Sendo que iremos escrever no terminal usando a chamada ao sistema write teremos de incrementar em 48 cada elemento do array para que este seja o valor decimal da sua representação na
    table de ascii*/
    for(index = 0; index < nOd; index++)
        array[index] = array[index] + 48;
}

/*Função myATOI - similiar à função atoi da biblioteca mas sem a utilização de funções de alto nivel || esta função receberá como parametro uma string
conteudo um valor numero e fara a conversao da mesma para um inteiro que sera devolvido pela função*/
int myATOI(char *str)
{
    //Declaração e inicialização de variaveis locais 
    int inteiro = 0;//Variavel auxiliar que guardara o valor inteiro convertido a partir da string

    while(*str >= '0' && *str <= '9'){//Verifica se a posição atual da string é um digito entre 0 e 9 (fora disso não é possivel de ser convertido em int)
        inteiro = inteiro * 10 + (*str - '0');/*Adiciona o digito da posição atual string (retirando o caracter terminação de string) ao resultado anterior
        multiplicado por 10 para que se aumente uma unidade (passagem das unidades para as dezenas, dezenas para as centenas e assim sucessivamente)*/
        *str++;//Incremento da posição da string a ser analisada
    }
    return inteiro;//Devolução do valor do inteiro convertido
}

//Função myStrcpy - implementação da função strcpy da biblioteca string.h
void myStrcpy(char *dest, const char *src)
{
    //Declaração e inicialização de variaveis locais
    int index = 0;//Variavel auxiliar que ira iterar pelas duas strings 

    while (src[index] != '\0'){//Verificação que ainda não atingimos o fim da string
        dest[index] = src[index];//Copia do caracter da posição index da string de origem para a string de destino
        index++;//Incremento da posição a ser copiada e de copia em ambas as strings
    }
    dest[index] = '\0';//Colocação do caracter terminação na string copia
}

//Função splitString - ira dividir a string (primeiro argumento) pelo caracter espaço e ira escrever esse valores nos restantes argumentos
void splitString(const char *str, int *latitude, int *longitude, char *caminho)
{
    //Declaração e inicialização de variaveis locais 
    int index = 0;//Variavel axuxiliar que ira iterar nas strings 
    char buffer[BUFFER_SIZE];//Array para onde sera copiada a string recebida para manipulação
    char *latitudeString = malloc(BUFFER_SIZE);//Variavel temporaria que ira guardar a string que contem o inteiro de latitude para conversão
    char *longitudeString = malloc(BUFFER_SIZE);//Variavel temporaria que ira guardar a string que contem o inteiro de longitude para conversão

    // Copia a string para o buffer
    myStrcpy(buffer, str);

    // Divide a string pelo delimitador espaço
    while(buffer[index] != '\0'){//Ira perccorer a string enquanto não encontrar o seu fim
        if(buffer[index] == ' '){//Caso tenha encontrado o delimitador espaço, ira colocar um caracter fim de strinf para proceder à sua posição
            buffer[index] = '\0';
            //Incremento da posição no buffer e salto para a proxima parte da sting a ser delimitadda
            index++;
            break;
        }
        index++;
    }
    myStrcpy(latitudeString, buffer);//Copia do valor do buffer para a respetiva variavel
    *latitude = myATOI(latitudeString);//Conversão para o tipo correto utilizado na struct coordenada

    //Copi a 2 parte da string para a variavel longitude string e procede à conversao para o tipo correto
    myStrcpy(longitudeString, &buffer[index]);
    *longitude = myATOI(longitudeString);

    //Procura o próximo delimitador e realiza o mesmo processo do primerio ciclo while
    while(buffer[index] != '\0'){
        if(buffer[index] == ' '){
            buffer[index] = '\0';
            index++;
            break;
        }
        index++;
    }

    // Copia a terceira parte para a variável caminho 
    myStrcpy(caminho, &buffer[index]);
}
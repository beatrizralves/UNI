#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <mysql.h>
#include <windows.h>

#define HOST "localhost"
#define USER "root"
#define PASS "root"
#define DB "pit_bd"

/*----------menus----------*/
void menu_principal()
{
    printf("\tMENU\n");
    printf("Escolha o que quer gerir:\n");
    printf("1- Sistemas Sensores\n");
    printf("2- Sensores\n");
    printf("3- Leituras\n");
    printf("0- Sair\n");
}
void menu_ss()
{
    printf("--------------------------------------------\n");
    printf("\tSistemaa Sensores\n");
    printf("--------------------------------------------\n");
    printf("1- Listar todos os sistemas sensores\n");
    printf("2- Listar sensores de um sistema sensor\n");
    printf("3- Listar todas as leituras de um sistema sensor\n");
    printf("4- Adicionar sistema sensor\n");
    printf("5- Editar sistema sensor\n");
    printf("0- Voltar ao menu principal\n");
}
void menu_sens()
{
    printf("--------------------------------------------\n");
    printf("\tSensores\n");
    printf("--------------------------------------------\n");
    printf("1- Listar todos os sensores\n");
    printf("2- Listar todos as leituras de um sensor\n");
    printf("3- Listar todos os sensores ativos/desativados\n");
    printf("4- Adicionar sensor\n");
    printf("5- Editar  sensor\n");
    printf("0- Voltar ao menu principal\n");
}
void menu_leitu()
{
    printf("--------------------------------------------\n");
    printf("\tLeituras\n");
    printf("--------------------------------------------\n");
    printf("1- Listar todos as leituras\n");
    printf("2- Listar todos leituras de uma data\n");
    printf("0- Voltar ao menu principal\n");
}
/*-----------------------*/
/*Funçoes queries*/
void lista_ss()
{
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int field_width = 20; // set the width of each field

    char query[]="SELECT * FROM sistema_sensor;";

    if (conn == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
       
    }

    if (mysql_real_connect(conn, HOST, USER,PASS, DB , 0, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    if (mysql_query(conn, query)) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    // Get result set and number of fields
   result = mysql_store_result(conn);

    // Get field information
    field = mysql_fetch_fields(result);

    // print the header row
    while (field = mysql_fetch_field(result)) {
        printf("|%-*.*s", field_width, field_width, field->name);
        
    }
    printf("\n");

    // print the data rows
    while (row = mysql_fetch_row(result)) {
        for (int i = 0; i < mysql_num_fields(result); i++) {
            printf("|%-*.*s", field_width, field_width, row[i]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(conn);
}

void lista_s_ss(){
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int field_width = 20; // set the width of each field

    char query[]="SELECT ss.nome_sistema_sensor, se.nome_sensor FROM sistema_sensor ss, sensor se WHERE ss.id_SS = se.id_SS;";

    if (conn == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
       
    }

    if (mysql_real_connect(conn, HOST, USER,PASS, DB , 0, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    if (mysql_query(conn, query)) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    // Get result set and number of fields
   result = mysql_store_result(conn);

    // Get field information
    field = mysql_fetch_fields(result);

    // print the header row
    while (field = mysql_fetch_field(result)) {
        printf("|%-*.*s", field_width, field_width, field->name);
        
    }
    printf("\n");

    // print the data rows
    while (row = mysql_fetch_row(result)) {
        for (int i = 0; i < mysql_num_fields(result); i++) {
            printf("|%-*.*s", field_width, field_width, row[i]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(conn);
}

void lista_lei_ss(){
     MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int field_width = 20; // set the width of each field
    char nome_sistema_sensor[100];
    printf("");
    printf("Diga o nome do sistema sensor: ");
    scanf("%s", nome_sistema_sensor);

    char query[200];
    sprintf(query, "SELECT l.* FROM leitura l JOIN sensor s ON l.id_sensor = s.id_sensor JOIN sistema_sensor ss ON s.id_SS = ss.id_SS WHERE ss.nome_sistema_sensor = '%s'", nome_sistema_sensor);

    

    if (conn == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
       
    }

    if (mysql_real_connect(conn, HOST, USER,PASS, DB , 0, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    if (mysql_query(conn, query)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
  }

    // Get result set and number of fields
   result = mysql_store_result(conn);

    // Get field information
    field = mysql_fetch_fields(result);

    // print the header row
    while (field = mysql_fetch_field(result)) {
        printf("|%-*.*s", field_width, field_width, field->name);
        
    }
    printf("\n");

    // print the data rows
    while (row = mysql_fetch_row(result)) {
        for (int i = 0; i < mysql_num_fields(result); i++) {
            printf("|%-*.*s", field_width, field_width, row[i]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(conn);

}

/*nao acabadas*/
void inser_ss(){

    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int id_ss; // ID generated by the system_sensor table
    char query[]="INSERT INTO sistema_sensor (nome_sistema_sensor, tipo, estado) VALUES ('Novo System', 'Sensor Tipo', 'Ativo');";
    

     if (mysql_real_connect(conn, HOST, USER,PASS, DB , 0, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    // INSERT statement to create a new system in the system_sensor table
    if (mysql_query(conn, query)) {
        printf("Insert Error: %s\n", mysql_error(conn));
        
    }

    // Get the ID generated by the system_sensor table
    id_ss = mysql_insert_id(conn);
    char query2[]="INSERT INTO sensor (id_SS, nome_sensor, estado) VALUES (id_ss,'Novo Sensor', 'Activo');";
    // INSERT statement to create a new system in the system_sensor table
    if (mysql_query(conn, query2)) {
        printf("Insert Error: %s\n", mysql_error(conn));
        
    }
    mysql_close(conn);

}

void lista_s_estado(char *estado){
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    
    int field_width = 20; // set the width of each field
    char query[200];
    sprintf(query, "SELECT sistema_sensor.nome_sistema_sensor, sensor.nome_sensor, sensor.estado FROM sistema_sensor INNER JOIN sensor ON sistema_sensor.id_SS = sensor.id_SS WHERE sensor.estado = '%s';", estado);

    if (conn == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
       
    }

    if (mysql_real_connect(conn, HOST, USER,PASS, DB , 0, NULL, 0) == NULL) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    if (mysql_query(conn, query)) {
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        
    }

    // Get result set and number of fields
   result = mysql_store_result(conn);

    // Get field information
    field = mysql_fetch_fields(result);

    // print the header row
    while (field = mysql_fetch_field(result)) {
        printf("|%-*.*s", field_width, field_width, field->name);
        
    }
    printf("\n");

    // print the data rows
    while (row = mysql_fetch_row(result)) {
        for (int i = 0; i < mysql_num_fields(result); i++) {
            printf("|%-*.*s", field_width, field_width, row[i]);
        }
        printf("\n");
    }
    mysql_free_result(result);
    mysql_close(conn);
}


int main()
{

    int escolha;
    int escolha_ss;
    int escolha_s;
    int escolha_lei;
    int estado;

    printf("\t|----------------------------------------------|\n");
    printf("\t|---------------BEM-VINDO----------------------|\n");
    printf("\t|----------------------------------------------|\n\n");

    do
    {
        menu_principal();
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            menu_ss();
            printf("Escolha uma opcao: ");
            scanf("%d", &escolha_ss);
            switch (escolha_ss)
            {
            case 1:
                // listar os sistemas sensores
                lista_ss();
                printf("\n");
                printf("Pressione qualquer tecla para voltar ao menu principal\n");
                getch();
                printf("......................\n");
                break;
            case 2:
                // listar sensores de um sistema sensor
                lista_s_ss();
                printf("Pressione qualquer tecla para voltar ao menu principal\n");
                getch();
                printf("......................\n");

                break;
            case 3:
                // listar todas as leituras de um sistema sensor
                lista_lei_ss();
                printf("Pressione qualquer tecla para voltar ao menu principal\n");
                getch();
                printf("......................\n");

                break;
            case 4:
                // adicionar sistema sensor
                inser_ss();
                printf("Pressione qualquer tecla para voltar ao menu principal\n");
                getch();
                printf("......................\n");
                break;
            case 5:
                // editar sistema sensor
                break;
            case 0:
                // volta ao menu_principal
                break;
            default:
                printf("Opcao invalida!\n");
            }
            break;
        case 2:
            menu_sens();
            printf("Escolha uma opcao: ");
            scanf("%d", &escolha_s);
            switch (escolha_s)
            {
            case 1:
                // listar todos os sistemas sensores
                break;
            case 2:
                // listar sensores de um sistema sensor
                break;
            case 3:
                printf("Qual o estado do sensor que deseja consultar\n1-ativo\n0-desativado\n");
                scanf("%d,&estado");
                 char estado_char[20];
                if (estado == 1){
                    sprintf(estado_char, "desativado");
                } else if(estado == 0) {
                    sprintf(estado_char, "ativo");
                }
                lista_s_estado(estado_char);
                printf("Pressione qualquer tecla para voltar ao menu principal\n");
                getch();
                printf("......................\n");
                break;
            case 4:
                // adicionar sistema sensor
                break;
            case 5:
                // editar sistema sensor
                break;
            case 0:
                // volta ao menu_principal
                break;
            default:
                printf("Opcao invalida!\n");
            }
            break;
        case 3:
            menu_leitu();
            printf("Escolha uma opcao: ");
            scanf("%d", &escolha_lei);
            switch (escolha_lei)
            {
            case 1:
                // listar todos os sistemas sensores
                break;
            case 2:
                // listar sensores de um sistema sensor
                break;

            case 0:
                // volta ao menu_principal
                break;
            default:
                printf("Opcao invalida!\n");
            }
            break;
        case 0:
            printf("A sair...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (escolha != 0);
}

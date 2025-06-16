#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <conio.h>
#include "mysql.h"
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define USERS_MAX 10

/*DB_defines*/
#define HOST "localhost"
#define USER "root"
#define PASS "root"
#define DB "pit_bd"
/*-----------*/

int users_online = 0;

// CODIGOS DE MENSAGENS
//  1 -> DATA
//  2 -> STOP
//  3 -> START
//  4 -> END
//  0 -> ERROR

typedef struct
{
    int message_type;
    float temperature;
    float humidity;
    float distance;
    int movement;
    int rain;
    char timestamp[20];
    char error[5];
} trama;

typedef struct
{
    int id; // sistema sensor
    trama tramas[10];
    int num_tramas;
    struct client *next;
} client;

typedef struct node
{
    client data;
    struct node *next;
} node;

node *head = NULL;

void insert_node(client data)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->data = data;
    new_node->next = NULL;

    if (head == NULL)
    {
        head = new_node;
        return;
    }

    node *current_node = head;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }

    current_node->next = new_node;
}

void print_list()
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    node *current_node = head;
    while (current_node != NULL)
    {
        printf("Client %d:\n", current_node->data.id);
        for (int i = 0; i < 10; i++)
        {
            trama t = current_node->data.tramas[i];
            printf("    Trama %d: message_type=%d, temperature=%.2f, humidity=%.2f, distance=%.2f, movement=%d, rain=%d, timestamp=%s, error=%s\n",
                   i, t.message_type, t.temperature, t.humidity, t.distance, t.movement, t.rain, t.timestamp, t.error);
        }
        current_node = current_node->next;
    }
}

client *get_last_client()
{
    if (head == NULL)
    {
        return NULL;
    }

    node *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    return &(current->data);
}

bool interrupted = false;

void signal_handler(int signal)
{
    interrupted = true;
}

void add_trama_to_client(int id, trama new_trama)
{
    client *current = head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            // Found the desired client
            if (sizeof(current->tramas) / sizeof(current->tramas[0]) > current->num_tramas)
            {
                // There is space in the tramas array
                current->tramas[current->num_tramas] = new_trama;
                current->num_tramas++;
                return;
            }
            else
            {
                // Discard the oldest trama and move the existing tramas back one position
                for (int i = 0; i < 9; i++)
                {
                    current->tramas[i] = current->tramas[i + 1];
                }
                current->tramas[9] = new_trama;
                return;
            }
        }
        current = current->next;
    }
}

void insert_ss(char *nome_ss, char *tipo, char *estado)
{
    int res;

    MYSQL *conn = mysql_init(NULL); // initialize MySQL connection object

    char *query = ""; // SQL statement to insert data into the table
    sprintf(query, "INSERT INTO sistema_sensor (nome_sistema_sensor, tipo, estado) \
                 VALUES (%s,%s,%s,%s);",
            nome_ss, tipo, estado);

    if (mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0) == NULL)
    {
        // establish a database connection
        printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
    }

    res = mysql_query(conn, query);
    if (!res)
        printf("Registros inseridos %d\n", mysql_affected_rows(conn));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(conn), mysql_error(conn));

    // mysql_close(conn);
}

void insert_leitura(int *id_s, char *timestamp, char *dados)
{
    // printf("%d, %s,%s", id_s, timestamp, dados);
    int res;

    MYSQL *conn = mysql_init(NULL); // initialize MySQL connection object
    if (conn == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char *query = ""; // SQL statement to insert data into the table
    sprintf(query, "INSERT INTO leitura (id_sensor, timestamp, dados) VALUES (%d,%s,%s);", id_s, timestamp, dados);
    printf(query);
    if (mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    res = mysql_query(conn, query);
    if (!res)
        printf("Registros inseridos %d\n", mysql_affected_rows(conn));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(conn), mysql_error(conn));

    mysql_close(conn);

   // return NULL;
}
DWORD WINAPI HandleClient(LPVOID lpParam)
{
    SOCKET client_socket = (SOCKET)lpParam;
    users_online++;
    client *last_in = get_last_client();
    if (last_in == NULL)
    {
        // First client
        client c = {0, {}};
        insert_node(c);
    }
    else
    {
        client c = {last_in->id + 1, {}};
        insert_node(c);
    }

    printf("Users Online: %d\n", users_online);
    // Receive data from client
    while (!interrupted)
    {
        char recvbuf[1024];
        int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
        trama data;
        memcpy(&data, recvbuf, sizeof(trama));

        char temperature_string[10];
        sprintf(temperature_string, "%f", data.temperature);
        // printf("%s", temperature_string);
        char humidade_string[10];
        sprintf(humidade_string, "%f", data.humidity);
        
        insert_leitura(1, data.timestamp, temperature_string);
        //  printf("%s -> Temperature: %f | Humidity: %f | Distance: %f | Movement: %d | Rain: %d\n", data.timestamp, data.temperature, data.humidity, data.distance, data.movement, data.rain);
        client *user = get_last_client();
        add_trama_to_client(user->id, data);

        
        //  DB - START
        // insert_leitura(2, data.timestamp, humidade_string); 
        //  DB - END

        if (recvlen == SOCKET_ERROR)
        {
            printf("Failed to receive data from client.\n");
            closesocket(client_socket);
            break;
        }

        // Send data back to client
        char sendbuf[] = "Hello, client!";
        int sendlen = send(client_socket, sendbuf, sizeof(sendbuf), 0);
        if (sendlen == SOCKET_ERROR)
        {
            printf("Failed to send data to client.\n");
            closesocket(client_socket);
            return 1;
        }
    }

    // Close the client socket and return
    users_online--;
    printf("Users Online: %d\n", users_online);
    closesocket(client_socket);
    return 0;
}

int main(int argc, char *argv[])
{

    signal(SIGINT, signal_handler);
    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed to initialize.\n");
        return 1;
    }

    // Create Socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Failed to bind socket.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Failed to listen for incoming connections.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Accept incoming connections and spawn threads to handle them
    while (1)
    {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET)
        {
            printf("Failed to accept incoming connection.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        // Spawn a thread to handle this client
        CreateThread(NULL, 0, HandleClient, (LPVOID)client_socket, 0, NULL);
    }
    return 0;
}
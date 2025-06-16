#include <libwebsockets.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
// #include <mysql.h>
#include <curl/curl.h>
#include "cJSON.h"
#include <pthread.h>
#include <netinet/in.h>

#define PORT_API 2222
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

#define TRUE 1
#define FALSE 0
#define PORT 12345

int server_socket;
int api_socket;
int client_sockets[MAX_CLIENTS];
pthread_t client_threads[MAX_CLIENTS];
int client_count = 0;

// CODIGOS DE MENSAGENS
//  1 -> DATA
//  2 -> STOP
//  3 -> START
//  4 -> END
//  0 -> ERROR

// typedef struct{
//     SOCKET client_socket;
//     int simulated;
// }ThreadParams;

typedef struct
{
    int id_ss;
    int systeam_type;
    int message_type;
    float temperature;
    float humidity;
    float distance;
    int movement;
    int rain;
    char timestamp[20];
    char error[];
} trama;

typedef struct
{
    int id_SS;
    int id_user;
    char *nome_sistema_sensor;
    char *tipo;
    char *estado;
} sistema_sensor;

typedef struct
{
    int id;
    trama tramas[10];
    int num_tramas;
    int connected_sensors[5];
    struct client *next;
} client;

typedef struct node
{
    client data;
    struct node *next;
} node;

node *head = NULL;

// char* itoa(int val, int base){

// 	static char buf[32] = {0};

// 	int i = 30;

// 	for(; val && i ; --i, val /= base)

// 		buf[i] = "0123456789abcdef"[val % base];

// 	return &buf[i+1];

// }

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

void cleanup()
{
    // Close client sockets
    for (int i = 0; i < client_count; i++)
    {
        close(client_sockets[i]);
    }

    // Close server socket
    close(server_socket);
}

void sigint_handler(int signum)
{
    interrupted = 1;
    printf("Shutting down server...\n");
    cleanup();
    exit(0);
}

void *client_handler(void *arg)
{
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;
    char *system_endpoint = "localhost:8085/post_ss";
    char *reading_endpoint = "localhost:8085/post_reading";
    client *last_in = get_last_client();
    client c = {0, {}};
    for (int i = 0; i < 5; i++)
    {
        c.connected_sensors[i] = 1;
    }
    if (last_in == NULL)
    {
        // First client
        insert_node(c);
    }
    else
    {
        c.id = last_in->id + 1;
        insert_node(c);
    }
    printf("Cliente id: %d\n", c.id);

    num_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    buffer[num_bytes] = '\0';
    printf("%s\n", buffer);

    // insert SS into db
    char name[15] = "Sistema ";
    char tmp[2];
    sprintf(tmp, "%d", c.id);
    strcat(name, tmp);

    cJSON *sensor_system = cJSON_CreateObject();
    cJSON_AddNumberToObject(sensor_system, "id_SS", c.id);
    cJSON_AddStringToObject(sensor_system, "nome_sistema_sensor", name);
    cJSON_AddStringToObject(sensor_system, "tipo", buffer);
    cJSON_AddStringToObject(sensor_system, "estado", "ON");
    char *sensor_system_string = cJSON_Print(sensor_system);

    // fazer pedido post a API para adicionar este sistema a DB
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, system_endpoint);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sensor_system_string);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    printf("Users Online: %d\n", client_count);

    // Handle client requests
    while ((num_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[num_bytes] = '\0';
        // Process received data from client
        // ...

        trama data;
        memcpy(&data, buffer, sizeof(trama));

        // printf("Received from client: %s\n", buffer);
        printf("TESTE\n");
        printf("Temperatura: %f\n", data.temperature);

        // Send response back to client
        // ...
        send(client_socket, buffer, num_bytes, 0);

        // API - START
        char temperature_string[10];
        char humidity_string[10];
        char distance_string[10];
        char movement_string[10];
        char rain_string[10];
        sprintf(temperature_string, "%f", data.temperature);
        sprintf(humidity_string, "%f", data.humidity);
        sprintf(distance_string, "%f", data.distance);
        cJSON *reading = cJSON_CreateObject();
        printf("ID: %d\n", c.id);
        cJSON_AddNumberToObject(reading, "id_sistema", c.id);
        cJSON_AddStringToObject(reading, "timestamp", data.timestamp);
        cJSON_AddStringToObject(reading, "temperatura", temperature_string);
        cJSON_AddStringToObject(reading, "humidade", humidity_string);
        cJSON_AddStringToObject(reading, "distancia", distance_string);
        cJSON_AddNumberToObject(reading, "movimento", data.movement);
        cJSON_AddNumberToObject(reading, "chuva", data.rain);
        char *reading_string = cJSON_Print(reading);

        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl)
        {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, reading_endpoint);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, reading_string);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
    }

    // Client disconnected or error occurred
    printf("Client disconnected.\n");

    // Remove client socket from the list
    for (int i = 0; i < client_count; i++)
    {
        if (client_sockets[i] == client_socket)
        {
            client_sockets[i] = client_sockets[client_count - 1];
            client_threads[i] = client_threads[client_count - 1];
            client_count--;
            break;
        }
    }

    // Close client socket
    close(client_socket);

    return NULL;
}

void *api_handler(void *arg)
{
    int client_socket = *(int *)arg;

    close(client_socket);
}

void *wait_for_client(void *arg)
{
    int socket = *((int *)arg);
    while (1)
    {
        int client_socket = accept(socket, NULL, NULL);
        if (client_socket == -1)
        {
            perror("Failed to accept client connection");
            continue;
        }
        pthread_t threadIdClient;

        // Create a new thread
        if (pthread_create(&threadIdClient, NULL, client_handler, &client_socket) != 0)
        {
            printf("Failed to create a new thread\n");
        }
    }
    pthread_exit(NULL);
}

void *wait_for_api(void *arg)
{
    int api_socket = *((int *)arg);
    while (1)
    {
        int client_socket = accept(api_socket, NULL, NULL);
        if (client_socket == -1)
        {
            perror("Failed to accept client connection");
            continue;
        }
        char recvbuf[1024];
        int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);

        cJSON *json = cJSON_Parse(recvbuf);
        if (json == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                fprintf(stderr, "Error before: %s\n", error_ptr);
            }
            exit(EXIT_FAILURE);
        }
        cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
        cJSON *id_user = cJSON_GetObjectItemCaseSensitive(json, "id_user");
        printf("Message: %s ", message->valuestring);
        printf("User ID: %d\n", id_user->valueint);
        int returnCode = system("./client");
        cJSON_Delete(json);
        close(client_socket);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Failed to create socket");
        return 1;
    }

    // Set up server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345); // Use your desired port number

    // Bind server socket to the address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Failed to bind socket");
        cleanup();
        return 1;
    }

    // Start listening for client connections
    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Failed to listen");
        cleanup();
        return 1;
    }

    printf("Server started. Listening for connections...\n");

    pthread_t threadIdClient;

    // Create a new thread
    if (pthread_create(&threadIdClient, NULL, wait_for_client, &server_socket) != 0)
    {
        printf("Failed to create a new thread\n");
        return 1;
    }

    // SAME FOR API
    int serverSocket1, newSocket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from server!";

    // Create socket
    if ((serverSocket1 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_API);

    // Bind socket to address
    if (bind(serverSocket1, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket1, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    pthread_t ThreadAPI;

    // Create a new thread
    if (pthread_create(&ThreadAPI, NULL, wait_for_api, &serverSocket1) != 0)
    {
        printf("Failed to create a new thread\n");
        return 1;
    }

    // Wait for the new thread to finish execution
    pthread_join(threadIdClient, NULL);
    pthread_join(ThreadAPI, NULL);

    cleanup();

    return 0;
}
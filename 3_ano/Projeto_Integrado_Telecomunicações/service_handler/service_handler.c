#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
//#include <mysql.h>
#include <curl/curl.h>
#include "cJSON.h"
#pragma comment(lib, "ws2_32.lib")
#define USERS_MAX 10

/*DB_defines*/
// #define HOST "localhost:8085"
// #define USER "root"
// #define PASS "root"
// #define DB "pit_db"
/*-----------*/


int users_online = 0;


//CODIGOS DE MENSAGENS
// 1 -> DATA
// 2 -> STOP
// 3 -> START
// 4 -> END
// 0 -> ERROR


typedef struct{
    SOCKET client_socket;
    int simulated;
}ThreadParams;

typedef struct {
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
}trama;

typedef struct{
    int id_SS;
    int id_user;
    char* nome_sistema_sensor;
    char* tipo;
    char* estado;
}sistema_sensor;

typedef struct{
    int id;
    trama tramas[10];
    int num_tramas;
    int registred_systems[5];
    struct client *next;
}client;

typedef struct node {
    client data;
    struct node* next;
} node;

node* head = NULL;



void insert_node(client data) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->data = data;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        return;
    }

    node* current_node = head;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_node;
}

void print_list() {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    node* current_node = head;
    while (current_node != NULL) {
        printf("Client %d:\n", current_node->data.id);
        for (int i = 0; i < 10; i++) {
            trama t = current_node->data.tramas[i];
            printf("    Trama %d: message_type=%d, temperature=%.2f, humidity=%.2f, distance=%.2f, movement=%d, rain=%d, timestamp=%s, error=%s\n",
                i, t.message_type, t.temperature, t.humidity, t.distance, t.movement, t.rain, t.timestamp, t.error);
        }
        current_node = current_node->next;
    }
}

client* get_last_client() {
    if (head == NULL) {
        return NULL;
    }

    node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    return &(current->data);
}


bool interrupted = false;

void signal_handler(int signal) {
    interrupted = true;
}

void add_trama_to_client(int id, trama new_trama) {
    client* current = head;
    while (current != NULL) {
        if (current->id == id) {
            // Found the desired client
            if (sizeof(current->tramas) / sizeof(current->tramas[0]) > current->num_tramas) {
                // There is space in the tramas array
                current->tramas[current->num_tramas] = new_trama;
                current->num_tramas++;
                return;
            } else {
                // Discard the oldest trama and move the existing tramas back one position
                for (int i = 0; i < 9; i++) {
                    current->tramas[i] = current->tramas[i+1];
                }
                current->tramas[9] = new_trama;
                return;
            }
        }
        current = current->next;
    }
}

DWORD WINAPI HandleClient(LPVOID lpParam) {
    ThreadParams* params = (ThreadParams*)lpParam;
    SOCKET client_socket = params->client_socket;
    int simulated = params->simulated;
    printf("%d\n", simulated);
    char *tipo;
    if(simulated)
        tipo = "real";
    else
        tipo = "simulado";
    char *system_endpoint = "localhost:8086/post_ss";
    char *reading_endpoint = "localhost:8086/post_reading";
    users_online++;
    client *last_in = get_last_client();
    client c = {0, {}};
    for(int i = 0; i < 5; i++){
        c.connected_sensors[i] = 1;
    }
    if(last_in == NULL){
        //First client
        insert_node(c);
    }
    else{
        c.id = last_in->id + 1;
        insert_node(c);
    }
    printf("Cliente id: %d\n", c.id);

    //insert SS into db
    char name[15] = "Sistema ";
    char tmp[2];
    itoa(c.id, tmp, 10);
    strcat(name, tmp);
    
    cJSON *sensor_system = cJSON_CreateObject();
    cJSON_AddNumberToObject(sensor_system, "id_SS", c.id);
    cJSON_AddNumberToObject(sensor_system, "id_user", c.id);
    cJSON_AddStringToObject(sensor_system, "nome_sistema_sensor", name);
    cJSON_AddStringToObject(sensor_system, "tipo", tipo);
    cJSON_AddStringToObject(sensor_system, "estado", "ON");
    char *sensor_system_string = cJSON_Print(sensor_system);


    // fazer pedido post a API para adicionar este sistema a DB
    CURL *curl;
    CURLcode res;
    
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, system_endpoint);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sensor_system_string);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    printf("Users Online: %d\n", users_online);


    // Receive data from client
    while(!interrupted){
        char recvbuf[1024];
        int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
        trama data;
        memcpy(&data, recvbuf, sizeof(trama));
        client *user = get_last_client();
        add_trama_to_client(user->id, data);
        
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
        if(curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, reading_endpoint);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, reading_string);
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        // API - END

        if (recvlen == SOCKET_ERROR) {
            printf("Failed to receive data from client.\n");
            closesocket(client_socket);
            break;
        }

        // Send data back to client
        char sendbuf[] = "Received";
        int sendlen = send(client_socket, sendbuf, sizeof(sendbuf), 0);
        if (sendlen == SOCKET_ERROR) {
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

DWORD WINAPI HandleAPI(LPVOID lpParam){
    SOCKET client_socket = (SOCKET)lpParam;
    //receive data from api
    while(1){
        char recvbuf[1024];
        int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
        if (recvlen == SOCKET_ERROR) {
            printf("Failed to receive data from client. API\n");
            closesocket(client_socket);
            break;
        }
        printf("%s\n", recvbuf);
        // sistema_sensor system;
        // memcpy(&system, recvbuf, sizeof(sensor_system));
        // if(system){
        //     printf("Detetou\n");
        // }
        // else{
        //     printf("Nao detetou\n");
        // }
        // Send data back to client
        char sendbuf[] = "Hello, client!";
        int sendlen = send(client_socket, sendbuf, sizeof(sendbuf), 0);
        if (sendlen == SOCKET_ERROR) {
            printf("Failed to send data to client.\n");
            closesocket(client_socket);
            return 1;
        }
    }
    return 0;
}

DWORD WINAPI WaitForClientsSim(LPVOID lpParam){
    SOCKET server_socket = (SOCKET)lpParam;
    while(1){
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("Failed to accept incoming connection.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        ThreadParams params = { client_socket, 0};
        CreateThread(NULL, 0, HandleClient, &params, 0, NULL);
        //CreateThread(NULL, 0, HandleClient, (LPVOID)client_socket, 0, NULL);
    }
    return 0;
}


DWORD WINAPI WaitForClientsReal(LPVOID lpParam){
    SOCKET server_socket = (SOCKET)lpParam;
    while(1){
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("Failed to accept incoming connection.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        ThreadParams params = { client_socket, 1};
        CreateThread(NULL, 0, HandleClient, &params, 0, NULL);
        //CreateThread(NULL, 0, HandleClient, (LPVOID)client_socket, 0, NULL);
    }
    return 0;
}


DWORD WINAPI WaitForAPI(LPVOID lpParam){
    SOCKET server_socket = (SOCKET)lpParam;
    while(1){
        SOCKET api_socket = accept(server_socket, NULL, NULL);
        if (api_socket == INVALID_SOCKET) {
            printf("Failed to accept incoming connection.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        CreateThread(NULL, 0, HandleAPI, (LPVOID)api_socket, 0, NULL);
    }
    return 0;
}


int main(int argc, char* argv[]){

    signal(SIGINT, signal_handler);
    //Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize.\n");
        return 1;
    }

    //Create Socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET api_socket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET real_system_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }
    if (api_socket == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }
    if (real_system_socket == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }

    //Bind the socket to an address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    //bind different socket for api communication
    struct sockaddr_in api_addr;
    api_addr.sin_family = AF_INET;
    api_addr.sin_addr.s_addr = INADDR_ANY;
    api_addr.sin_port = htons(6666);

    //bind different socket for real system connections
    struct sockaddr_in real_addr;
    real_addr.sin_family = AF_INET;
    real_addr.sin_addr.s_addr = INADDR_ANY;
    real_addr.sin_port = htons(7777);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Failed to bind socket.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (bind(api_socket, (struct sockaddr*)&api_addr, sizeof(api_addr)) == SOCKET_ERROR) {
        printf("Failed to bind socket API.\n");
        closesocket(api_socket);
        WSACleanup();
        return 1;
    }

    if (bind(real_system_socket, (struct sockaddr*)&real_addr, sizeof(real_addr)) == SOCKET_ERROR) {
        printf("Failed to bind socket API.\n");
        closesocket(real_system_socket);
        WSACleanup();
        return 1;
    }

    //listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Failed to listen for incoming connections.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(api_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Failed to listen for incoming connections.\n");
        closesocket(api_socket);
        WSACleanup();
        return 1;
    }

    if (listen(real_system_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Failed to listen for incoming connections.\n");
        closesocket(real_system_socket);
        WSACleanup();
        return 1;
    }
    

    CreateThread(NULL, 0, WaitForClientsSim, (LPVOID)server_socket, 0, NULL);
    CreateThread(NULL, 0, WaitForClientsReal, (LPVOID)real_system_socket, 0, NULL);
    CreateThread(NULL, 0, WaitForAPI, (LPVOID)api_socket, 0, NULL);

    //Accept incoming connections and spawn threads to handle them
    while (1) {
        // SOCKET client_socket = accept(server_socket, NULL, NULL);
        // SOCKET client_api_socket = accept(api_socket, NULL, NULL);
        // if (client_socket == INVALID_SOCKET) {
        //     printf("Failed to accept incoming connection.\n");
        //     closesocket(server_socket);
        //     WSACleanup();
        //     return 1;
        // }
        // if (client_api_socket == INVALID_SOCKET) {
        //     printf("Failed to accept incoming connection API.\n");
        //     closesocket(api_socket);
        //     WSACleanup();
        //     return 1;
        // }

        // // Spawn a thread to handle this client
        // CreateThread(NULL, 0, HandleAPI, (LPVOID)client_api_socket, 0, NULL);
        // CreateThread(NULL, 0, HandleClient, (LPVOID)client_socket, 0, NULL);
    }
    return 0;
}
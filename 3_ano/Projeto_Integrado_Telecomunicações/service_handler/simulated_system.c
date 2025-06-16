#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

//CODIGOS DE MENSAGENS
// 1 -> DATA
// 2 -> STOP
// 3 -> START
// 4 -> END
// 0 -> ERROR

//booleans:
// 0 -> FALSE
// 1 -> TRUE


struct trama{
    int id_ss;
    int system_type;
    int message_type;
    float temperature;
    float humidity;
    float distance;
    int movement;
    int rain;
    char timestamp[20];
    char error[10];
};



unsigned long millis() {
    static clock_t start_time = 0;
    if (start_time == 0) {
        start_time = clock();
    }
    clock_t current_time = clock();
    clock_t elapsed_clock_ticks = current_time - start_time;
    unsigned long elapsed_milliseconds = elapsed_clock_ticks * 1000 / CLOCKS_PER_SEC;
    return elapsed_milliseconds;
}

float random_float(float min, float max) {
    float value = (float)rand() / (float)RAND_MAX * (max - min) + min;
    return value;
}

int random_bool() {
    int value = rand() % 2;
    return value;
}

int main(int argc, char* argv[]){
    //CONNECTION HANDLER CODE - START
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char *server_ip = "127.0.0.1";
    int server_port = 12345;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed");
        return 1;
    }
    //CONNECTION HANDLER CODE - END

    unsigned long start_time, elapsed_time;
    float temperature, humidity, distance;
    bool movement, rain;
    int mov, r;
    srand(time(NULL));

    start_time = millis();

    // Send data to server
    char *message = "I'm Online";
    int message_len = strlen(message);

    if (send(client_socket, my_string, sizeof(my_string), 0) < 0) {
        printf("Send failed");
        return 1;
    }

    // Receive a response from the server
    char recvbuf[1024];
    int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
    if (recvlen == SOCKET_ERROR) {
        int error_code = WSAGetLastError();
        printf("Failed to receive response from server. %d\n", error_code);
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Print the response from the server
    recvbuf[recvlen] = '\0';
    printf("Server response: %s\n", recvbuf);

    while(1){

    }

    while(1){

        elapsed_time = millis() - start_time;
        if(elapsed_time >= 1000){
            temperature = random_float(0.0, 45.0);
            humidity = random_float(0.0, 100.0);
            distance = random_float(0.0, 15.0);
            int mov = random_bool();
            if(mov == 0)
                movement = false;
            else
                movement = true;
            int r = random_bool();
            if(r == 0)
                rain = false;
            else
                rain = true;

            //timestamp
            // Get the current time
            time_t raw_time;
            time(&raw_time);

            // Convert the time to a local time
            struct tm *local_time = localtime(&raw_time);

            // Format the timestamp
            char timestamp[20];
            memset(timestamp, 0, sizeof(timestamp));
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);

            printf("%s -> Temperature: %f | Humidity: %f | Distance: %f | Movement: %s | Rain: %s\n", timestamp, temperature, humidity, distance, movement ? "true" : "false", rain ? "true" : "false");
            
            char error[10] = "erro";
            
            struct trama data;
            data.message_type = 1;
            data.temperature = temperature;
            data.humidity = humidity;
            data.distance = distance;
            data.movement = mov;
            data.rain = r;
            strcpy(data.timestamp, timestamp);
            strcpy(data.error, error);
            char my_string[sizeof(struct trama)];
            memcpy(my_string, &data, sizeof(data));

            // Send data to server
            char *message = "Hello, server!";
            int message_len = strlen(message);

            if (send(client_socket, my_string, sizeof(my_string), 0) < 0) {
                printf("Send failed");
                return 1;
            }

            // Receive a response from the server
            char recvbuf[1024];
            int recvlen = recv(client_socket, recvbuf, sizeof(recvbuf), 0);
            if (recvlen == SOCKET_ERROR) {
                int error_code = WSAGetLastError();
                printf("Failed to receive response from server. %d\n", error_code);
                closesocket(client_socket);
                WSACleanup();
                return 1;
            }

            // Print the response from the server
            recvbuf[recvlen] = '\0';
            printf("Server response: %s\n", recvbuf);

            start_time = millis();
        }
    }

    // Close the socket and cleanup Winsock
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
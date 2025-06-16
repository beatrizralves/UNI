#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#define PORT 12345

int interrupted = 0;
int client_fd;

//CODIGOS DE MENSAGENS
// 1 -> DATA
// 2 -> STOP
// 3 -> START
// 4 -> END
// 0 -> ERROR

//booleans:
// 0 -> FALSE
// 1 -> TRUE

void sigint_handler(int signum) {
    close(client_fd);
    interrupted = 1;
    printf("Program interrupted by Ctrl+C.\n");
    exit(0);
    // Perform additional actions or cleanup here
    // ...
}


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


// unsigned long millis() {
//     static clock_t start_time = 0;
//     if (start_time == 0) {
//         start_time = clock();
//     }
//     clock_t current_time = clock();
//     clock_t elapsed_clock_ticks = current_time - start_time;
//     unsigned long elapsed_milliseconds = elapsed_clock_ticks * 1000 / CLOCKS_PER_SEC;
//     return elapsed_milliseconds;
// }

unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

float random_float(float min, float max) {
    float value = (float)rand() / (float)RAND_MAX * (max - min) + min;
    return value;
}

int random_bool() {
    int value = rand() % 2;
    return value;
}

int main(int argc, char const* argv[]){
    signal(SIGINT, sigint_handler);
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    
    float temperature, humidity, distance;
    bool movement, rain;
    int mov, r;
    srand(time(NULL));

    unsigned long last_execution_time = millis();
    char* simulado = "simulado";
    

    if (send(client_fd, simulado, sizeof(simulado), 0) < 0) {
        printf("Send failed");
        return 1;
    }

    while(!interrupted){
        unsigned long current_time = millis();
        unsigned long elapsed_time = current_time - last_execution_time;
        if(elapsed_time >= 1000){
            last_execution_time = current_time;
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

            if (send(client_fd, my_string, sizeof(my_string), 0) < 0) {
                printf("Send failed");
                return 1;
            }
        }
    }

    // send(client_fd, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    // valread = read(client_fd, buffer, 1024);
    // printf("%s\n", buffer);
  
    // closing the connected socket
    
    return 0;
}
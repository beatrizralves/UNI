#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
    int serverSocket, newSocket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};
    char *message = "Hello from server!";

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept connection from client
    if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Send message to client
    // send(newSocket, message, strlen(message), 0);
    // printf("Hello message sent to client\n");

    // Receive data from client
    valread = read(newSocket, buffer, MAX_BUFFER_SIZE);
    printf("Received message from client: %s\n", buffer);

    // Clean up
    close(newSocket);
    close(serverSocket);
    return 0;
}
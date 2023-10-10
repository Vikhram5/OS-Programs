

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }

    // Prompt the user for a file name
    printf("Enter the file name to request: ");
    char filename[MAX_BUFFER_SIZE];
    fgets(filename, sizeof(filename), stdin);
    strtok(filename, "\n"); // Remove the newline character

    // Send an HTTP GET request for the specified file
    char request[MAX_BUFFER_SIZE + strlen(filename) + 64];
    snprintf(request, sizeof(request), "GET / %s HTTP/1.1\r\nHost: localhost\r\n\r\n", filename);

    send(client_socket, request, strlen(request), 0);

    // Receive and print the response
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received response:\n%s\n", buffer);

    // Close the client socket
    close(client_socket);

    return 0;
}

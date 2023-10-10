#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, sizeof(buffer));
    int n = recv(client_socket, buffer, sizeof(buffer), 0);

    if (n <= 0)
    {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Received date and time from server: %s", buffer);

    close(client_socket);

    return 0;
}

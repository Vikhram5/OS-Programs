#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int sockfd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    printf("Server listening on port 8080...\n");

    addr_size = sizeof(new_addr);
    new_socket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

    // Handle client connection
    strcpy(buffer, "Hello, Client!");
    send(new_socket, buffer, strlen(buffer), 0);

    printf("Message sent to client: %s\n", buffer);

    close(new_socket);
    close(sockfd);

    return 0;
}

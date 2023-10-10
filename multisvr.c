#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    printf("Server listening on port 8080\n");

    while (1)
    {
        addr_size = sizeof(new_addr);
        new_socket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

        if (fork() == 0)
        {                  // Child process
            close(sockfd); // Close the listening socket in the child process
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(new_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
            printf("Connection accepted from %s:%d\n", client_ip, ntohs(new_addr.sin_port));

            strcpy(buffer, "Hello from the server!");
            send(new_socket, buffer, strlen(buffer), 0);

            printf("Message sent to client: %s\n", buffer);
            close(new_socket);
            exit(0); // Terminate the child process
        }

        close(new_socket); // Close the accepted socket in the parent process
    }

    close(sockfd);
    return 0;
}

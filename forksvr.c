#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 12345
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int num_clients = 0;

void handle_client(int client_socket)
{
    char buffer[1024];
    int n;

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        n = recv(client_socket, buffer, sizeof(buffer), 0);

        if (n <= 0)
        {
            for (int i = 0; i < num_clients; i++)
            {
                if (clients[i] == client_socket)
                {
                    num_clients--;
                    clients[i] = clients[num_clients];
                }
            }
            close(client_socket);
            return;
        }

        for (int i = 0; i < num_clients; i++)
        {
            if (clients[i] != client_socket)
            {
                send(clients[i], buffer, n, 0);
            }
        }
    }
}

int main()
{
    int sockfd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        new_socket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

        if (new_socket < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        if (num_clients >= MAX_CLIENTS)
        {
            close(new_socket);
            continue;
        }

        clients[num_clients++] = new_socket;

        pid_t pid = fork();

        if (pid == 0)
        {
            close(sockfd);
            handle_client(new_socket);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);

    return 0;
}

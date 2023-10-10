#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 12345

int main()
{
    int sockfd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    char buffer[1024];
    time_t ticks;

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

    if (listen(sockfd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(new_addr);
    printf("Server listening on port %d\n", PORT);

    new_socket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

    if (new_socket < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Get the current time and format it
    ticks = time(NULL);
    snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));

    send(new_socket, buffer, strlen(buffer), 0);
    printf("Sent date and time to client: %s", buffer);

    close(new_socket);
    close(sockfd);

    return 0;
}

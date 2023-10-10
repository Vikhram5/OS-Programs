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
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    char operator;
    int num1, num2, result;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Menu:\n");
        printf("1. Addition (+)\n");
        printf("2. Subtraction (-)\n");
        printf("3. Multiplication (*)\n");
        printf("4. Division (/)\n");
        printf("q. Quit\n");

        printf("Enter your choice: ");
        scanf(" %c", &operator);

        if (operator== 'q')
        {
            send(client_socket, &operator, sizeof(char), 0);
            break; // Exit the client if the user chooses to quit
        }

        printf("Enter two numbers: ");
        scanf("%d %d", &num1, &num2);

        send(client_socket, &operator, sizeof(char), 0);
        send(client_socket, &num1, sizeof(int), 0);
        send(client_socket, &num2, sizeof(int), 0);

        recv(client_socket, &result, sizeof(int), 0);

        if (result == -1)
        {
            printf("Error: Division by zero\n");
        }
        else if (result == -2)
        {
            printf("Error: Invalid operator\n");
        }
        else
        {
            printf("Result: %d\n", result);
        }
    }

    close(client_socket);
    return 0;
}

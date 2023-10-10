#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int performArithmetic(int num1, int num2, char operator)
{
    switch (operator)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0)
        {
            return -1; // Indicate division by zero
        }
        return num1 / num2;
    default:
        return -2; // Indicate an invalid operator
    }
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 5);
    printf("Server is listening...\n");
    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

    while (1)
    {
        int num1, num2, result;
        char operator;

        // Receive the menu choice from the client
        recv(client_socket, &operator, sizeof(char), 0);

        if (operator== 'q')
        {
            break; // Exit the server if the client chooses to quit
        }

        // Receive the operands from the client
        recv(client_socket, &num1, sizeof(int), 0);
        recv(client_socket, &num2, sizeof(int), 0);

        // Perform the arithmetic operation
        result = performArithmetic(num1, num2, operator);

        // Send the result back to the client
        send(client_socket, &result, sizeof(int), 0);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "xdef.h"

#define PORT 12224
#define HOST "127.0.0.1"
#define BUFFER_SIZE 1024

static xt_bool is_exit = XFALSE;

void handle_sigint(int sig) {
    is_exit = XTRUE;
}

int main() {

    signal(SIGINT, handle_sigint);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(HOST);;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&clnt_addr, (socklen_t*)&clnt_addr_size)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    while (!is_exit) {
        int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
        /*int valread = read(new_socket, buffer, BUFFER_SIZE);*/
        if (valread > 0) {
            printf("Received: %s\n", buffer);
            memset(buffer, 0, BUFFER_SIZE);
        } else if (valread == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            perror("Read failed");
            break;
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}

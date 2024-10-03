#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include "xdef.h"

#define SERVER_PORT 12223
#define BUFFER_SIZE 1024

static xt_bool is_exit = XFALSE;

void handle_sigint(int sig) {
    is_exit = XTRUE;
}

int main() {
    signal(SIGINT, handle_sigint);

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    printf("Server is waiting for messages...\n");

    while (!is_exit) {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
         if (len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
            } else {
                perror("recvfrom failed");
            }
            continue;
         }

        buffer[len] = '\0';
        printf("Received message: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}

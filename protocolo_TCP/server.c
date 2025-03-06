#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Socket failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("Bind failed");
    }

    if (listen(server_fd, 3) < 0) {
        error("Listen failed");
    }

    printf("Servidor esperando conexiones en el puerto %d...\n", PORT);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        error("Accept failed");
    }

    read(new_socket, buffer, BUFFER_SIZE);
    printf("Mensaje recibido: %s\n", buffer);
    send(new_socket, "Mensaje recibido", strlen("Mensaje recibido"), 0);

    close(new_socket);
    close(server_fd);
    return 0;
}


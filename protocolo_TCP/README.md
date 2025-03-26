# Implementación de un Protocolo TCP en C

Este documento explica paso a paso la implementación de un protocolo TCP simple en C utilizando dos programas separados: un servidor y un cliente.

## Descripción General

El protocolo TCP permite la comunicación confiable entre dos aplicaciones a través de una red. En este caso, implementaremos:

- **Servidor:** Escucha conexiones entrantes, recibe un mensaje del cliente y envía una respuesta.
- **Cliente:** Se conecta al servidor, envía un mensaje y espera la respuesta.

## Código del Servidor (`server.c`)

```c
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
```

### Explicación del Código del Servidor
1. Se crea un **socket** con `socket(AF_INET, SOCK_STREAM, 0)`.
2. Se configura la estructura `sockaddr_in` con la dirección `INADDR_ANY` y el puerto `8080`.
3. Se enlaza el socket con `bind()`.
4. Se pone el servidor en espera con `listen()`.
5. Se acepta una conexión entrante con `accept()`.
6. Se recibe un mensaje del cliente con `read()`.
7. Se envía una respuesta con `send()`.
8. Se cierran los sockets.

## Código del Cliente (`client.c`)

```c
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
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        error("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    char *message = "Hola, servidor";
    send(sock, message, strlen(message), 0);
    printf("Mensaje enviado\n");
    read(sock, buffer, BUFFER_SIZE);
    printf("Respuesta del servidor: %s\n", buffer);

    close(sock);
    return 0;
}
```

### Explicación del Código del Cliente
1. Se crea un **socket** con `socket(AF_INET, SOCK_STREAM, 0)`.
2. Se configura la estructura `sockaddr_in` con la IP `127.0.0.1` y el puerto `8080`.
3. Se establece la conexión con `connect()`.
4. Se envía un mensaje con `send()`.
5. Se recibe la respuesta con `read()`.
6. Se cierra el socket.

## Compilación y Ejecución

Para compilar los programas:
```bash
gcc server.c -o server
gcc client.c -o client
```

Para ejecutar el servidor:
```bash
./server
```

Para ejecutar el cliente en otra terminal:
```bash
./client
```

## Funcionamiento del Programa
1. El servidor se ejecuta y queda esperando conexiones en el puerto `8080`.
2. El cliente se conecta al servidor y envía el mensaje "Hola, servidor".
3. El servidor recibe el mensaje y responde "Mensaje recibido".
4. El cliente muestra la respuesta y cierra la conexión.



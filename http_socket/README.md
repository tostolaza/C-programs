
# HTTP Socket
# Servidor HTTP Básico en C

Este código implementa un servidor HTTP en C que escucha en el puerto 8080 y responde con "Hello, World!" a cada solicitud.

## Inclusión de Librerías
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
```
Estas bibliotecas son necesarias para el manejo de entrada/salida, memoria, cadenas, sistema y redes.

## Definición de Constantes
```c
#define PORT 8080
#define BUFFER_SIZE 1024
```
- `PORT`: Puerto donde el servidor escuchará conexiones.
- `BUFFER_SIZE`: Tamaño del buffer para solicitudes HTTP.

## Declaración de Variables
```c
int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};
```

## Definición de la Respuesta HTTP
```c
const char *http_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "Connection: close\r\n\r\n"
    "Hello, World!";
```

## Creación del Socket
```c
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
}
```

## Configuración del Servidor
```c
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
```

## Asociación del Socket al Puerto
```c
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
}
```

## Escuchar Conexiones
```c
if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
}
```

## Aceptar Conexiones
```c
while (1) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
```

## Leer Solicitud HTTP
```c
read(new_socket, buffer, BUFFER_SIZE);
printf("Solicitud recibida:\n%s\n", buffer);
```

## Enviar Respuesta
```c
send(new_socket, http_response, strlen(http_response), 0);
close(new_socket);
```

## Cómo Ejecutarlo
1. **Compilar**:
   ```sh
   gcc -o server server.c
   ```
2. **Ejecutar**:
   ```sh
   ./server
   ```
3. **Abrir en el navegador**:
   - Visita `http://localhost:8080/`
   - Verás `"Hello, World!"` como respuesta.

## Conclusión
Este código implementa un servidor HTTP básico en C que:
- Crea un socket TCP.
- Escucha en el puerto 8080.
- Acepta conexiones de clientes.
- Procesa solicitudes HTTP.
- Responde con `"Hello, World!"`.
- Cierra la conexión.



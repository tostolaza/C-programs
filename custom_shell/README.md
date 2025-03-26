
# Mini Bash en C – Explicación Detallada

Este documento explica paso a paso cómo construir una **Mini Shell (Mini Bash)** en lenguaje C. Aprenderás la lógica de programación detrás de una shell, cómo interactuar con el sistema operativo, y cómo ejecutar comandos como `ls`, `cd` o `exit`.

---

## ¿Qué es una shell?

Una **shell** es un programa que actúa como intermediario entre el usuario y el sistema operativo. Permite ejecutar comandos, navegar por directorios y manipular archivos.

---

## Componentes del proyecto

### Librerías utilizadas

```c
#include <stdio.h>      // Para printf, fgets
#include <stdlib.h>     // Para exit, malloc
#include <string.h>     // Para strtok, strcmp
#include <unistd.h>     // Para fork, execvp, chdir
#include <sys/wait.h>   // Para waitpid
```

Estas librerías proporcionan funciones necesarias para entrada/salida, manipulación de cadenas, creación de procesos y manejo del sistema de archivos.

---

## Flujo general del programa

1. Mostrar un **prompt** (símbolo que indica que puedes escribir un comando).
2. Leer el comando ingresado.
3. Analizarlo y separarlo en argumentos.
4. Verificar si es un **comando interno** (`cd`, `exit`) o externo (`ls`, `pwd`, etc.).
5. Ejecutar el comando.
6. Repetir hasta que el usuario escriba `exit`.

---

## Explicación del código

### `prompt()`

```c
void prompt() {
    printf("mini-bash> ");
}
```

Esta función imprime el prompt cada vez que se espera un nuevo comando del usuario.

---

### `read_input()`

```c
void read_input(char* line) {
    fgets(line, MAX_LINE, stdin);
    line[strcspn(line, "\n")] = '\0'; // Elimina el salto de línea final
}
```

- `fgets()` lee una línea del usuario.
- `strcspn()` se usa para encontrar el salto de línea y reemplazarlo por un carácter nulo (`'\0'`) para evitar errores en el procesamiento del comando.

---

### `parse_input()`

```c
void parse_input(char* line, char** args) {
    int i = 0;
    args[i] = strtok(line, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}
```

- Se utiliza `strtok()` para dividir el input en palabras (tokens).
- Por ejemplo, si el usuario escribe `ls -l`, esta función lo convierte en:  
  `args[0] = "ls", args[1] = "-l", args[2] = NULL`

Esto es necesario para pasarlo luego a `execvp()`.

---

### `execute_command()`

```c
int execute_command(char** args) {
    if (args[0] == NULL) {
        return 1; // Comando vacío
    }

    // Comando interno: exit
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Termina el shell
    }

    // Comando interno: cd
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Uso: cd <directorio>\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    }

    // Comandos externos (ls, pwd, etc.)
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo
        if (execvp(args[0], args) == -1) {
            perror("mini-bash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        // Proceso padre
        int status;
        waitpid(pid, &status, 0);
    }

    return 1;
}
```

#### ¿Qué hace esta función?

1. **Verifica si el comando está vacío** (`args[0] == NULL`).
2. **Detecta comandos internos**:
   - `exit`: termina la shell.
   - `cd`: cambia de directorio con `chdir()`.
3. **Para comandos externos**:
   - Crea un nuevo proceso con `fork()`.
   - El proceso hijo ejecuta el comando con `execvp()`.
   - El proceso padre espera que el hijo termine con `waitpid()`.

---

### `main()`

```c
int main() {
    char line[MAX_LINE];
    char* args[MAX_ARGS];
    int status = 1;

    while (status) {
        prompt();                      // 1. Mostrar prompt
        read_input(line);              // 2. Leer comando
        parse_input(line, args);       // 3. Analizar comando
        status = execute_command(args); // 4. Ejecutar comando
    }

    return 0;
}
```

Este es el **bucle principal**. Repite el ciclo de leer → analizar → ejecutar mientras `status` sea distinto de 0. El valor `0` se devuelve cuando el usuario ejecuta `exit`.

---

## Cómo compilar y ejecutar

1. Guarda el código como `shell.c`.
2. Compílalo:
   ```bash
   gcc shell.c -o shell
   ```
3. Ejecuta tu shell:
   ```bash
   ./shell
   ```

---

## Comandos de prueba

```bash
mini-bash> pwd
/home/usuario

mini-bash> ls -l
(total de archivos y carpetas)

mini-bash> cd ..
mini-bash> pwd

mini-bash> exit
```
---

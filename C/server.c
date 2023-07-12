#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define PORT 8484
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 2048

int serverSocket;

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';

        // Get the current date and time
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        char datetime[128];
        strftime(datetime, sizeof(datetime), "%d %B %Y, %H:%M", tm);

        printf("Received message from client: %s\n", buffer);

        // Combine incoming text with server's current date and time
        char response[RESPONSE_SIZE];
        snprintf(response, sizeof(response), "Text from client received at %s: %s", datetime, buffer);

        // Send the response back to the client
        send(clientSocket, response, strlen(response), 0);

        printf("Sent response to client: %s\n", response);
    }

    close(clientSocket);
}

void *clientThread(void *arg) {
    int clientSocket = *((int *)arg);
    handleClient(clientSocket);
    free(arg);
    return NULL;
}

void sigintHandler(int signum) {
    printf("\nTerminating the server...\n");
    close(serverSocket);
    exit(EXIT_SUCCESS);
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    pthread_t tid;

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Register SIGINT signal handler
    signal(SIGINT, sigintHandler);

    // Accept and handle client connections
    while (1) {
        // Accept a new connection
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

        if (clientSocket == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Allocate memory for the argument to be passed to the thread
        int *clientSocketPtr = (int *)malloc(sizeof(int));
        *clientSocketPtr = clientSocket;

        // Create a new thread to handle the client
        if (pthread_create(&tid, NULL, clientThread, clientSocketPtr) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        // Detach the thread to clean up resources automatically
        if (pthread_detach(tid) != 0) {
            perror("Thread detachment failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

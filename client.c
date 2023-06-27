#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFFER_SIZE 1024


int main() {
    char serverIP[16];
    int serverPort;
    char userInput[BUFFER_SIZE];

    printf("Enter the server IP address: ");
    scanf("%s", serverIP);

    printf("Enter the server port number: ");
    scanf("%d", &serverPort);



    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

 while (1){
    printf("Enter a text to send to the server(or type exit to terminate): ");
    scanf(" %[^\n]", userInput);

    if (strcmp(userInput, "exit") == 0){
	break;
	}

    // Send user input to the server
    send(clientSocket, userInput, strlen(userInput), 0);

    // Receive and print the server's response
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Server response: %s\n", buffer);
    }
}
    // Close the socket
    close(clientSocket);


    return 0;
}


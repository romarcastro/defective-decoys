  																							/*
             _____      ___         _   _           ____                      
            |     \ ___|  _|___ ___| |_|_|_ _ ___  |    \ ___ ___ ___ _ _ ___ 
            |  |  | -_|  _| -_|  _|  _| | | | -_|  |  |  | -_|  _| . | | |_ -|
            |____/|___|_| |___|___|_| |_|\_/|___|  |____/|___|___|___|_  |___|
                                                                    |___|   

    Defective Decoys                                                         
    Filename: main.c
	
    Contributers: 
        [+] John Bicierro 
        [+] Romar Castro
        [+] Ken Lanon
        [+] Eric Tan 
																							*/  

#define VER 0.1
#define MAX_PLAYERS 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Socket
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>

// Callbacks
#include "lib/connection.h"

// Socket
int server_socket,
    client_socket;

struct 
    sockaddr_in 
        server_address, 
        client_address;

    socklen_t 
        client_length;

int main(int argc, char const *argv[])
{
    int port;
    printf("Please enter the port: ");
    scanf("%d", &port);

    OnPlayerConnection("localhost", port);

    OnGameModeExit();
    return 0;
}

// Functions
void OnPlayerConnection(char *ip, int port) 
{
    printf("* Connecting to %s:%d ...\n", ip, port);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        ReturnEx("server_socket failed! (socket)");
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        ReturnEx("binding failed! (socket)");
    }

    listen(server_socket, MAX_PLAYERS);
    printf("* Server listening to port %d ...\n", port);
    
    // Client Socket
    client_length = sizeof(client_length);
    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_length);
    if(client_socket < 0) {
        ReturnEx("accept failed! (socket)");
    }

    int n; char buffer[256];
    while(1) { // Looping the conversation
        bzero(buffer, 256);
        n = recv(client_socket, buffer, 256, 0);
        if(n < 0) {
            ReturnEx("read failed! (socket)");
        }

        printf("Client: %s\n", buffer);
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        n = write(client_socket, buffer, strlen(buffer));
        if(n < 0) {
            ReturnEx("write failed! (socket)");
        }

        int i = strncmp("Bye", buffer, 3);
        if(i == 0) {
            break;
        }
    }

    printf("* Connected. Joining the game ...\n");
}

void OnGameModeExit() 
{
    close(server_socket);
    close(client_socket);
}

void ReturnEx(char *input) 
{
    printf("Error: %s\n", input);
    exit(EXIT_FAILURE);
}
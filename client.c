  																							/*
    Defective Decoys 
    Filename: client.c
    Author: 
																							*/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Socket
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>

#define h_addr h_addr_list[0]

// Debug (note: 1 true else 0 false)
int debug = 1;

// Client Socket
int client_socket;
struct sockaddr_in server_address;
struct hostent *server;

// Callbacks
#include "lib/client.h"

int main(int argc,  char *argv[]) 
{
    if(debug) {
        SetPlayerConnection("localhost", atoi(argv[1]));
    } else {
        char ip[24]; int port;
        printf("Please enter the IP you would like to connect to: ");
        scanf("%s", ip);
        printf("Please enter the port: ");
        scanf("%d", &port);
        SetPlayerConnection(ip, port);
    }

    OnGameModeInit();

    close(client_socket);
    return 0;
}

void OnGameModeInit() 
{
    while(1)
    {
        char str[128];
        bzero(str, sizeof(str));
        recv(client_socket, str, sizeof(str), 0);
        printf("%s", str);
        fgets(str, sizeof(str), stdin);
    }


    /*int n; char buffer[256];
    while(1) { // Looping the conversation
        bzero(buffer, 256);
        fgets(buffer, 256, stdin);

        n = send(client_socket, buffer, strlen(buffer), 0);
        if(n < 0) {
            ReturnEx("send failed! (socket)");
        }

        bzero(buffer, 256);
        n = recv(client_socket, buffer, 256, 0);
        if(n < 0) {
            ReturnEx("read failed! (socket)");
        }

        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if(i == 0) {
            break;
        }
    }*/
}

void SetPlayerConnection(char *ip, int port) 
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0) {
        ReturnEx("client_socket failed! (socket)");
    }

    server = gethostbyname(ip);
    if(server == NULL) {
        ReturnEx("no such host (socket)");
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
    
    if(connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) > 0) {
        ReturnEx("connect failed! (socket)");
    }

    printf("* You are connected to the server using the port %d ...\n", port);
}

void SendClientMessage(const char *str)
{
    int n = send(client_socket, str, strlen(str), 0);
    if(n < 0) {
        ReturnEx("send failed! (socket)");
    }
}

void SendClientMessageToAll(const char *str)
{
    int n = send(client_socket, str, strlen(str), 0);
    if(n < 0) {
        ReturnEx("send failed! (socket)");
    }
    printf("%s\n", str);
}

void ReturnEx(char *input) 
{
    printf("Error: %s\n", input);
    exit(EXIT_FAILURE);
}
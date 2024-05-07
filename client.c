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

// Client Socket
int client_socket;
struct sockaddr_in server_address;
struct hostent *server;

void ReturnEx(char *input) 
{
    printf("Error: %s\n", input);
    exit(EXIT_FAILURE);
}

int main(int argc,  char *argv[]) 
{
    char ip[24]; int port;
    printf("Please enter the IP you would like to connect to: ");
    scanf("%s", ip);
    printf("Please enter the port: ");
    scanf("%d", &port);

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

    int n; char buffer[256];
    while(1) { // Looping the conversation
        bzero(buffer, 256);
        fgets(buffer, 256, stdin);

        n = write(client_socket, buffer, strlen(buffer));
        if(n < 0) {
            ReturnEx("write failed! (socket)");
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
    }

    close(client_socket);

    return 0;
}
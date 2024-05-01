  																							/*
    Defective Decoys 
    Filename: server.c
    Author: 
																							*/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void ReturnEx(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int server_sock, 
        client_sock, 
        port_no, 
        client_size, 
        n;

    char buffer[256];
    
    struct sockaddr_in server_addr, 
        client_addr;

    if(argc < 2) 
    {
        printf("Usage: %s [port number]\n", argv[0]);
        exit(1);
    }

    printf("Server starting ...\n");

    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address 
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port

    if(server_sock < 0) 
    {
        ReturnEx("Error: socket() Failed.");
    }
    else if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        ReturnEx("Error: bind() Failed.");
    }
       
       
    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
    printf("Server listening to port %d ...\n", port_no);
    
    printf("Waiting for connection(s) ...\n");

    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
    if(client_sock < 0)
    {
        ReturnEx("Error: accept() Failed.");
    }
        
    printf("Client succesfully connected ...\n");   

    // Communicate    
    bzero(buffer, 256);
    n = recv(client_sock, buffer, 255, 0);
    if(n < 0) 
    {
        ReturnEx("Error: recv() Failed.");
    }
    printf("Message received from client : %s", buffer);

    printf("Sending reply ...\n");
    n = send(client_sock, "I got your message!", 20, 0);
    if(n < 0) ReturnEx("Error: send() Failed.");

    printf("Closing connection ...\n");
    close(client_sock);
    
    close(server_sock);
    
    return 0; 
}
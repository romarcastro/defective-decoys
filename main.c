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

int main(int argc, char const *argv[])
{
    char ip[24]; int port;
    printf("Please enter the IP you would like to connect to: ");
    scanf("%s", ip);
    printf("Please enter the port: ");
    scanf("%d", &port);

    OnPlayerConnection(ip, port);
    return 0;
}

// Functions
void ReturnEx(char *input) {
    printf("%s", input);
}

void OnPlayerConnection(char *ip, int port) {
    printf("* Connecting to %s:%d ...\n", ip, port);
    printf("* Connected. Joining the game ...");
}


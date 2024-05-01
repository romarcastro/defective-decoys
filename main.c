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
void OnPlayerConnection(char *ip, int port);

int main(int argc, char const *argv[])
{
    // argc amount of argument
    printf("main() debug: argc: %d", argc);


    printf("Defective Decoys\n");

    char ip[24]; int port;
    printf("Please enter the IP you would like to connect to: \n");
    scanf("%s", ip);
    printf("Please enter the port: \n");
    scanf("%d", &port);

    OnPlayerConnection(ip, port);
    // Func connect to the socket (IP, PORT)
    return 0;
}

// Functions
void OnPlayerConnection(char *ip, int port) {
    

}


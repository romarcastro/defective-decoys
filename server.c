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

// Debug (note: 1 true else 0 false)
int debug = 1;

// Socket (Server)
int server_socket,
    client_socket;

struct 
    sockaddr_in 
        server_address, 
        client_address;

    socklen_t 
        client_length;

// Callbacks
#include "lib/game.h" /* -> Game */
#include "lib/main.h" /* -> Main */
#include "lib/socket.h" /* -> Socket */

// Main Function
int main(int argc, char const *argv[])
{
    // Socket
    if(debug) {
        SetPlayerConnection("localhost", atoi(argv[1]));
    } else {
        int port;
        printf("Please enter the port: ");
        scanf("%d", &port);
        SetPlayerConnection("localhost", port);
    }

    // Game
    OnGameModeInit();

    // Delete static objects
    OnGameModeExit();
    return 0;
}

// Game
#define ROW (4)
#define COL (4)
#define SERVER_ID 0
#define CLIENT_ID 1

void OnGameModeInit()
{
    // Variables
    int Board[MAX_PLAYERS][ROW][COL];
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        for(int j = 0; j < ROW; j++)
        {
            for(int k = 0; k < COL; k++)
            {
                Board[i][j][k] = 0;
            }
        }
    }

    /* Setup Phase 1 (Server) */ 
    // Wait phase (Client)
    SendClientMessage("Please wait for your next turn! sabi ni ken");

    // Setup phase (Server)
    for(int i = 0; i < 4; i++) // 4 Decoys sabi ni romar
    {
        char str[128];
        printf("Enter coordinates for decoy (e.g., A0): ");
        if(fgets(str, sizeof(str), stdin))
        {
            str[strcspn(str, "\n")] = 0;

            // check the input
            if(strlen(str) != 2)
            {
                printf("[!] Invalid input. Please enter exactly two characters (e.g., A0). \n");
                continue;
            }

            char row = str[0];
            char col = str[1];

            if(!isValidInput(row, col)) 
            {
                printf("[!] Invalid coordinates. Coordinates must be A-D for rows and 0-3 for columns. Try again.\n");
                continue; 
            }
        }
    }

    // Wait phase (Server)
    printf("Please wait for your opponent to place decoy(s)! sabi ni ken");

    // Setup phase (Client)
    for(int i = 0; i < 4; i++) // 4 Decoys sabi ni romar
    {
        char str[128];
        bzero(str, sizeof(str));
        SendClientMessage("Enter coordinates for decoy (e.g., A0): ");
        recv(client_socket, str, sizeof(str), 0);
        
        // check the input
        if(strlen(str) != 2)
        {
            SendClientMessage("[!] Invalid input. Please enter exactly two characters (e.g., A0)");
            continue;
        }

        char row = str[0];
        char col = str[1];

        if(!isValidInput(row, col)) 
        {
            SendClientMessage("[!] Invalid coordinates. Coordinates must be A-D for rows and 0-3 for columns. Try again.");
            continue; 
        }
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
bool isValidInput(char row, char col) 
{
    return (row >= 'A' && row <= 'D') && (col >= '0' && col <= '3');
}

// Socket
void SetPlayerConnection(char *ip, int port) 
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
    printf("* Client connected using the port %d ...\n", port);
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

// Main
void ReturnEx(char *input) {
    printf("Error: %s\n", input);
    exit(EXIT_FAILURE);
}
void OnGameModeExit() {
    close(server_socket);
    close(client_socket);
}
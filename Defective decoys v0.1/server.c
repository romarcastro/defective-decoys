#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555
#define BUFFER_SIZE 1024
#define ROWS 4
#define COLS 4
#define DECOYS 4

typedef struct {
    int socket;
    char board[ROWS][COLS];
} Player;

Player players[2];

void die_with_error(char *errorMessage) {
    perror(errorMessage);
    exit(1);  // Terminate program with an error status
}

void initializeBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';  // Set all positions to space indicating empty
        }
    }
}

// Function to receive decoy placements from a player
void receiveAndSetDecoys(Player *player) {
    char buffer[4];  // Buffer for "A1\0"
    for (int i = 0; i < DECOYS; i++) {
        if (recv(player->socket, buffer, 3, 0) < 0) {
            die_with_error("Error receiving decoys");
        }
        buffer[3] = '\0'; // Null-terminate the buffer
        int row = buffer[0] - 'A';
        int col = buffer[1] - '0';
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            player->board[row][col] = 'X';  // Mark the decoy
        }
    }
}

void sendWinner(int clientSocket, int server_fd) {
    close(clientSocket);
    close(server_fd);
}

// Check the opponent's board for a hit or miss
char checkGuess(Player *opponent, int row, int col) {
    if (opponent->board[row][col] == 'X') {
        opponent->board[row][col] = 'O';  // Mark as guessed
        return 'H';  // Return 'H' to indicate a hit which also includes re-guessing
    }
    if (opponent->board[row][col] == 'O') {
        opponent->board[row][col] = 'O';  // Mark as guessed
        return 'A'; 
    }
    opponent->board[row][col] = 'O';
    	return 'M';  // Return 'M' for a Miss
}

int main() {
    int server_fd, addrlen;
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        die_with_error("Bind failed");
    }

    listen(server_fd, 2);  // Listen for up to 2 connections
    addrlen = sizeof(address);

    Player players[2];  // Array to hold two players
    // Accept connections from both players
    for (int i = 0; i < 2; i++) {
        players[i].socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (players[i].socket < 0) {
            die_with_error("Accept failed");
        }
        printf("Player %d has connected!\n", i + 1);
        initializeBoard(players[i].board);
    }
    	printf("Welcome to DEFECTIVE DECOYS LETS GO WALING\n");
    // Receive and set decoys for both players
    for (int i = 0; i < 2; i++) {
        receiveAndSetDecoys(&players[i]);
    }
	
    // Game loop: ensure both players submit guesses before processing
    char results[2]; // Store results for both players
    while (1) {
        char guesses[2][4];  // Buffer to receive guesses for both players

        // Receive guesses from both players
        for (int i = 0; i < 2; i++) {
            int bytes_received = recv(players[i].socket, guesses[i], 3, 0);
    
            // Check for errors first
            if (bytes_received < 0) {
                die_with_error("Error receiving guess");
            }
    
    
            // Now check if the specific 'W' was received
            if (bytes_received == 1 && guesses[i][0] == 'W') {
                sendWinner(players[i].socket, server_fd);
                close(players[1 - i].socket);
                close(server_fd);
                return 0;
            }
        }

        // Process both guesses
        for (int i = 0; i < 2; i++) {
            int row = guesses[i][0] - 'A';
            int col = guesses[i][1] - '0';
            results[i] = checkGuess(&players[1 - i], row, col); 
        }

        // Send results back to both players
        for (int i = 0; i < 2; i++) {
            send(players[i].socket, &results[i], 1, 0);
        }
    }

    // Cleanup
    for (int i = 0; i < 2; i++) {
        close(players[i].socket);
    }
    close(server_fd);
    return 0;
}


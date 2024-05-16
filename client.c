#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5555
#define BUFFER_SIZE 1024

#define ROWS 4
#define COLS 4
#define DECOYS 4
#define LIVES 3

char board[ROWS][COLS];
char guessBoard[ROWS][COLS];
int lives = LIVES;
int sock = 0;

void initializeBoards() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
            guessBoard[i][j] = ' ';
        }
    }
}

void printBoard(char board[ROWS][COLS]) {
    printf("    0    1    2    3\n");
    for (int i = 0; i < ROWS; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < COLS; j++) {
            printf("[ %c ]", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void placeAndSendDecoys() {
    printf("Place %d computers as your decoys on your laboratory:\n", DECOYS);
    int decoysPlaced = 0;
    while (decoysPlaced < DECOYS) {
        char input[100];
        printBoard(board);
        printf("Enter position for decoy %d (e.g., A2): ", decoysPlaced + 1);
        fgets(input, sizeof(input), stdin);

        char rowChar;
        int col;
        if (sscanf(input, "%c%d", &rowChar, &col) == 2) {
            int row = rowChar - 'A';
            if (row >= 0 && row < ROWS && col >= 0 && col < COLS && board[row][col] == ' ') {
                board[row][col] = 'X';
                send(sock, input, strlen(input), 0);  // Send each decoy position to the server
                decoysPlaced++;
            } else {
                printf("Invalid position or decoy already placed there.\n");
            }
        } else {
            printf("Invalid input. Please use the format 'A2'.\n");
        }
    }
    printBoard(board);
    printf("\n");
    sleep(3);
    system("clear");
}

void guess() {
    printf("It’s guessing time, Hecker! Four of these computers are planted as decoys by the other hecker. Take your time and strategize before choosing, make sure to not select the decoys!\n\n");
    while (lives > 0) {
        printf("Enemy hecker laboratory:\n");
        printBoard(guessBoard);
        printf("You have %d lives left\n\n", lives);
        printf("Choose a Computer to heck (e.g., A2): ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline character

        if (strlen(input) != 2) {
            printf("Invalid input. Please enter exactly two characters (e.g., A0).\n");
            continue;
        }

        char row = input[0];
        char col = input[1];
        if (row < 'A' || row > 'D' || col < '0' || col > '3') {
            printf("Invalid input! Coordinates must be A-D for rows and 0-3 for columns.\n");
            continue;
        }

        int rowIndex = row - 'A';
        int colIndex = col - '0';
        
        //test if the client is still connected to the server
	char testing[1];
        send(sock, input, strlen(input), 0);
        int test = recv(sock, testing, 1, MSG_PEEK | MSG_DONTWAIT);
        if (test == 0) {
        printf("\nYOU WON THE GAME NICE ONE MAN!!\n");
        printf("Enemy hecker's laboratory that you hecked:\n\n");
        printBoard(board);
        break;
        }
        
        printf("Guess sent: %s\n\nWaiting for the result...\n", input);

        char result[BUFFER_SIZE];
        int bytes_received = recv(sock, result, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            result[bytes_received] = '\0';
            
                
            printf("Result: %s\n\n", result);
		
            // Update guess board based on the result
            if (result[0] == 'H') {
                printf("Hit! Decoy found at %s. You lose a life.\n", input);
                lives--;
                guessBoard[rowIndex][colIndex] = 'X'; // Mark hit decoy
            } else if (result[0] == 'M'){
                printf("Miss! No decoy at %s.\n", input);
                guessBoard[rowIndex][colIndex] = 'O'; // Mark safe guess
            } else if (result[0] == 'A') {
                printf("Already checked that spot, bawas isang buhay HAHA >:).\n");
                lives--;
                guessBoard[rowIndex][colIndex] = 'X'; // Mark hit decoy
            }

            printBoard(guessBoard);
            sleep(3);
            system("clear");
        } 
    }
}

int setupConnection(char **argv) {
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <server IP address>\n", argv[0]);
        return 1;
    }
    if (setupConnection(argv) != 0) {
        return 1;
    }

    initializeBoards();

    printf("______      __          _   _                 _                          \n");
    printf("|  _  \\    / _|        | | (_)               | |                         \n");
    printf("| | | |___| |_ ___  ___| |_ ___   _____    __| | ___  ___ ___  _   _ ___ \n");
    printf("| | | / _ \\  _/ _ \\/ __| __| \\ \\ / / _ \\  / _` |/ _ \\/ __/ _ \\| | | / __|\n");
    printf("| |/ /  __/ ||  __/ (__| |_| |\\ V /  __/ | (_| |  __/ (_| (_) | |_| \\__ \\\n");
    printf("|___/ \\___|_| \\___|\\___|\\__|_| \\_/ \\___|  \\__,_|\\___|\\___\\___/ \\__,_|___/\n");
    printf("                                                                __/ |    \n");
    printf("                                                               |___/     \n");

    placeAndSendDecoys();

    // Main game loop for turn-based play
    while (lives > 0) {
        guess();
        break;
    }
    if (lives > 0) { printf("You won the game! Nice one man!\n"); }
    else {printf("Game over! You have no more lives.\n");}
    send(sock, "W", 1, 0);
    close(sock);
    return 0;
}


#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>
#include "ai.h"
#include "game.h"

int main(void){
    int n;
	char board[MAX_BOARD_LEN][MAX_BOARD_LEN];
    char playerPiece, aiPiece;
    char turn;

    // Enter board dimension
    printf("Enter the board dimension (even number): ");
    while(!scanf("%d", &n) || n % 2 != 0 || n < 4 || n > MAX_BOARD_LEN)
		printf("Invalid, re-enter: ");
    
    // Initialize board
    for(int i=0;i<n;i++)			// Set all locations to U
        for(int j=0; j<n;j++)
            board[i][j] = U;       
    
    board[n/2-1][n/2-1] = W;		// Set center pieces
    board[n/2-1][n/2] = B;
    board[n/2][n/2-1] = B;
    board[n/2][n/2] = W;
    turn = B;						// Set initial turn: black piece goes first
    
    // Input computer colour
    printf("Choose your piece (B/W): ");
    scanf(" %c", &playerPiece);
	
	playerPiece = toupper(playerPiece);
	
	aiPiece = oppositePiece(playerPiece);
	printf("\nInitial Board\n");
    printBoard(board, n);
    
    // Game loop
	char result;
	int aiMoves = 0;
	double intervalSum = 0;
	
    while((result = checkGameStatus(board, n)) == ONGOING){
        // Computer's turn
        if(turn == aiPiece){
            int row, col;

			// Timer start
			LARGE_INTEGER frequency;
			LARGE_INTEGER start;
			LARGE_INTEGER end;
			double interval;
			
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&start);
			
			// Calculate move (timed)
			bool aiCanMove = aiFindMove(board, n, &row, &col, aiPiece);
			
			// Timer end
			QueryPerformanceCounter(&end);
			interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
			
            if(aiCanMove){
                printf("Computer places %c at %c%c.\n", aiPiece, row + 'a', col + 'a');
				printf("*Calculation time (s): %f\n", interval);
                flipEnemyPieces(board, n, row, col, aiPiece);
                printBoard(board, n);
				
				intervalSum += interval;
				aiMoves++;
            }else
                printf("Computer (%c) has no valid move, so turn is skipped.\n\n", aiPiece);
            
			turn = playerPiece;
        }
        
        // Player's turn
        else if(turn == playerPiece){
            bool playerCanMove = false;
            
            for(int i=0; i<n && !playerCanMove;i++){
                for(int j=0; j<n && !playerCanMove;j++){
                    if(validMove(board, n, i, j, playerPiece))
                        playerCanMove = true;
                }
            }
            
            if(playerCanMove){
                char inputRow, inputCol;
                int row, col;
				bool validInput = false;
				
                printf("Enter move for colour %c (RowCol): ", playerPiece);
				
				while(!validInput){
					scanf(" %c%c", &inputRow, &inputCol);
            
					row = tolower(inputRow) - 'a';
					col = tolower(inputCol) - 'a';
            
					if(positionInBounds(n, row, col) && validMove(board, n, row, col, playerPiece))
						validInput = true;
					else
						printf("Invalid move. Re-enter (RowCol): ");
                }
				flipEnemyPieces(board, n, row, col, playerPiece);
				printBoard(board, n);
				
            }else{
                printf("Player (%c) has no valid move, so turn is skipped.\n\n", playerPiece);
            }
            
            turn = aiPiece;
        }
	}
    
    // Output result message
    if(result == TIE)
        printf("Draw!\n\n");
    else if(result == playerPiece)
		printf("Player (%c) wins!\n\n", result);
	else if(result == aiPiece)
		printf("Computer (%c) wins!\n\n", result);

	printf("*Average calculation time: %f\n\n", intervalSum/aiMoves);
	
    return 0;
}
#include "game.h"

// Outputs the gameboard
void printBoard(char board[][MAX_BOARD_LEN], int n) {
	printf("  ");
	for (int i = 0; i < n; i++) {
		printf("%c", 'a' + i);
	}
	printf("\n");

	for (int i = 0; i < n; i++) {
		printf("%c ", 'a' + i);
		for (int j = 0; j < n; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Checks if the position given is within the bounds of the board
bool positionInBounds(int n, int row, int col) {
	return (row >= 0 && col >= 0 && row < n && col < n);
}

// Counts the amount of enemy pieces that are possible to flip at a given location (in a particular direction)
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	int count = 0;

	row += deltaRow;
	col += deltaCol;

	// Checks if first position is within bounds or opposite colour (no flips possible)
	if (!positionInBounds(n, row, col) || board[row][col] == U || board[row][col] == colour)
		return 0;

	// Moves from second position to edge of board
	do {
		row += deltaRow;
		col += deltaCol;
		count++;

		if (board[row][col] == U)
			return 0;
		if (board[row][col] == colour)
			return count;
	} while (positionInBounds(n, row, col));

	return 0;
}

// Counts the amount of enemy pieces that are possible to flip at a given location 
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	int count = 0;

	// Counts in all 8 possible directions
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)  		// Skips stationary case
				continue;

			count += possibleFlipsInDirection(board, n, row, col, colour, i, j);
		}
	}

	return count;
}

// Checks if a piece can be placed at a given position
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour){
	return board[row][col] == U && possibleFlips(board, n, row, col, colour) > 0;
}

// Flips opposite pieces from a given position (in a particular direction)
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
	// Flips opposite markers until current position reaches given colour marker
	do {
		board[row][col] = colour;

		row += deltaRow;
		col += deltaCol;
	} while (board[row][col] != colour && board[row][col] != U);
}

// Flips opposite pieces from a given position
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour) {
	bool flipped = false;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;

			// Flips pieces after checking that at least 1 piece can be flipped in a specific direction
			if (possibleFlipsInDirection(board, n, row, col, colour, i, j) > 0) {
				flipInDirection(board, n, row, col, colour, i, j);
				flipped = true;
			}
		}
	}

	return flipped;
}

// Gives the opposite piece
char oppositePiece(char turn) {
	return turn == B ? W : B;
}

/* B - Black wins
 * W - White wins
 * TIE - Tie
 * ONGOING - Ongoing
 */
// Checks if the game is over
char checkGameStatus(char board[][MAX_BOARD_LEN], int n){
	int blackFlips = 0, whiteFlips = 0;			// Counts total possible flips
	int blackPieces = 0, whitePieces = 0;		// Counts existing pieces placed
	
	for(int i=0; i<n;i++){
		for(int j=0; j<n;j++){
			if(board[i][j] == U){
				blackFlips += possibleFlips(board, n ,i, j, B);
				whiteFlips += possibleFlips(board, n ,i, j, W);
			}
			else if(board[i][j] == B)
				blackPieces ++;
			else if(board[i][j] == W)
				whitePieces ++;
		}
	}
	
	// If both pieces are out of possible flips, game ends
	if(blackFlips == 0 && whiteFlips == 0){
		if(blackPieces > whitePieces)
			return B;
		else if(whitePieces > blackPieces)
			return W;
		else
			return TIE;
	}else
		return ONGOING;
}
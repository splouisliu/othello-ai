#include "ai.h"

// Max of 2 ints
int max(int a, int b){
	return a > b ? a: b;
}

// Min of 2 ints
int min(int a, int b){
	return a < b ? a: b;
}

// Calculates the heuristic value for a given board state in terms of the AI
int heuristic(char board[][MAX_BOARD_LEN], int n, char aiPiece){
	int count = 0;

	for(int i = 0; i< n;i++){
		for(int j=0; j<n; j++){
			if(board[i][j] == aiPiece)
				count++;
			else if(board[i][j] == oppositePiece(aiPiece))
				count--;
		}
	}

	return count;
}

// Runs the minimax algorithm with alpha beta pruning
int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta) {
	// If max depth is reached, search is over. Returns the heuristic value at this game state
	if(depth == 0)
		return heuristic(board, n, aiPiece);
	
	// Checks for base cases. If game is over, the following status values are returned:
	char gameStatus = checkGameStatus(board, n);
	if(gameStatus == aiPiece)			// Win
        return INT_MAX;
	else if(gameStatus == oppositePiece(aiPiece))			// Loss
		return INT_MIN;
	else if(gameStatus == TIE)			// Tie
		return 0;
	
	// Search continues if game is ongoing
	int bestScore = (aiPiece == turn) ? INT_MIN: INT_MAX;
	bool hasValidMove = false;
	bool continueSearch = true;
	
	for (int i = 0; i < n && continueSearch; i++) {
		for (int j = 0; j < n && continueSearch; j++) {
			if (validMove(board, n, i, j, turn)) {
				hasValidMove = true;
				
				// Creates new gameboard to make move and flip enemy pieces
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = turn;
				flipEnemyPieces(tempBoard, n, i, j, turn);
				
				// Calculates the score based on new gameboard
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(turn), depth - 1, alpha, beta);
				
				// Maximize score for AI's turn
				if(turn == aiPiece){
					bestScore = max(bestScore, tempScore);
					alpha = max(alpha, tempScore);
				}
				// Minimize score for Player's turn
				else{
					bestScore = min(bestScore, tempScore);
					beta = min(beta, tempScore);
				}

				// Prunes unnecessary branch
				if(alpha >= beta)
					continueSearch = false;
			}
		}
	}
	
	// Switches to opposite player if current player has no valid moves
	if(!hasValidMove){
		return minimax(board, n, aiPiece, oppositePiece(turn), depth, alpha, beta);
	}
	
	return bestScore;
}

// Runs the first search of the minimax algorithm and takes the location of the optimal move
bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece){
	bool foundMove = false;
	int bestScore = INT_MIN;
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			
			if(validMove(board, n, i, j, aiPiece)){
				// Creates new gameboard to make the initial move and flip enemy pieces
				char tempBoard[MAX_BOARD_LEN][MAX_BOARD_LEN];
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
						tempBoard[k][l] = board[k][l];
					}
				}
				tempBoard[i][j] = aiPiece;
				flipEnemyPieces(tempBoard, n, i, j, aiPiece);
				
				// Calculates the score at each valid position
				int tempScore = minimax(tempBoard, n, aiPiece, oppositePiece(aiPiece), SEARCH_DEPTH - 1, INT_MIN, INT_MAX);
				
				// The highest scoring position is the optimal move (since initial position -> maximize)
				if(tempScore >= bestScore){
					*row = i;
					*col = j;
					bestScore = tempScore;
					foundMove = true;
				}
			}
		}
	}
	
	return foundMove;
}
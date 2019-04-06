#ifndef GAME_H
#define GAME_H

#define B 'B'	// black
#define W 'W'	// white
#define U 'U'	// undecided
#define TIE 'T'	// tie
#define ONGOING 'O'	// ongoing

#define MAX_BOARD_LEN 10

#include <stdbool.h>
#include <stdio.h>

void printBoard(char board[][MAX_BOARD_LEN], int n);
bool positionInBounds(int n, int row, int col);
int possibleFlipsInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
int possibleFlips(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
bool validMove(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
void flipInDirection(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool flipEnemyPieces(char board[][MAX_BOARD_LEN], int n, int row, int col, char colour);
char oppositePiece(char turn);
char checkGameStatus(char board[][MAX_BOARD_LEN], int n);

#endif
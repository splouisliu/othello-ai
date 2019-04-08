#ifndef AI_H
#define AI_H

#define SEARCH_DEPTH 8

#include "game.h"
#include <stdbool.h>
#include <limits.h>

int heuristic(char board[][MAX_BOARD_LEN], int n, char aiPiece);
int minimax(char board[][MAX_BOARD_LEN], int n, char aiPiece, char turn, int depth, int alpha, int beta);
bool aiFindMove(char board[][MAX_BOARD_LEN], int n, int *row, int *col, char aiPiece);

#endif
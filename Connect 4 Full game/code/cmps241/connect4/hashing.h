#ifndef HASHING_H
#define HASHING_H

#include "board.h"

// Extern to allow multiple files to access this global variable
extern unsigned long long zobristTable[rows][cols][3]; // Table, 3 for pieces types (A, B, .)
extern unsigned long long currentHash; // Stores hash of current board state

void initZobrist(); // Initialize
void updateHash(int row, int col, char oldPiece, char newPiece); // Update the hash when a piece changes (called every move/unmove)

#endif
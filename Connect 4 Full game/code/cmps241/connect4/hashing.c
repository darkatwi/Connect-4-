#include <stdlib.h>
#include "hashing.h"

// Global variable definitions
unsigned long long zobristTable[rows][cols][3];
unsigned long long currentHash = 0;

// Initialize Zobrist hash table with random values
void initZobrist()
{
    srand(123456789); // Initialize random number table
    for (int i = 0; i < rows; i++) // Loop through each row, column, piece type
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                zobristTable[i][j][k] = ((unsigned long long)rand() << 32) | rand(); // Generate 64-bit Random Numbers for good hash quality
            }
        }
    }
    currentHash = 0;
}
// Update hash when making/unmaking a move
void updateHash(int row, int col, char oldPiece, char newPiece)
{
    int oldIdx = (oldPiece == '.') ? 0 : (oldPiece == 'A') ? 1 : 2; // Convert old piece to index
    int newIdx = (newPiece == '.') ? 0 : (newPiece == 'A') ? 1 : 2; // Convert new piece to index
    currentHash ^= zobristTable[row][col][oldIdx]; // Remove old piece
    currentHash ^= zobristTable[row][col][newIdx]; // Add new piece
}
// We use XOR since it is the only operation that is its own inverse (X ^ A ^ A = X), 
// meaning we can add and remove pieces using the exact same operation, which is essential 
// for efficiently making and unmaking moves in minimax search.
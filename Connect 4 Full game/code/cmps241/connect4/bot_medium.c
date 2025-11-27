#include <stdio.h>
#include <stdlib.h>
#include "bot_medium.h"
#include "board.h"

// Check if opponent ('A') can win next move by stimulating a move then removing the value 
int checkOpponentWin(char board[rows][cols])
{
    for (int col = 0; col < cols; col++) // Loop through all columns
    {
        int row = addValue(board, col, 'A'); // For each column, temporarily assume the user places the piece there & use the addValue method to get the row if available (-1 otherwise)
        if (row != -1) // Only check columns that are not full
        {
            if (checkWin(board, 'A')) 
            {
                removeValue(row, col, 'A');
                return col; // Column leads to opponent win
            }
            removeValue(row, col, 'A');
        }
    }
    return -1;  // No columns lead to opponent win
}
// Medium bot implementation
void MediumBotMove()
{
    int centerOrder[] = {3, 2, 4, 1, 5, 0, 6};
    // 1. Try to win immediately
    for (int col = 0; col < cols; col++)
    {
        int row = addValue(board, col, 'B'); // Temporarily place piece 
        if (row != -1)
        {
            if (checkWin(board, 'B'))
            {
                printf("Bot placed its checker in column %d (winning move).\n", col + 1); 
                printBoard(board);
                return;
            }
            removeValue(row, col, 'B'); // Undo the temporary move if not winning
        }
    }
    // 2. Block opponent's immediate win
    int blockCol = checkOpponentWin(board); // Defense strategy
    if (blockCol != -1)
    {
        addValue(board, blockCol, 'B');
        printf("Bot placed its checker in column %d (blocking move).\n", blockCol + 1);
        printBoard(board);
        return;
    }
    // 3. Strategic move (center first)
    for (int i = 0; i < cols; i++)
    {
        int col = centerOrder[i]; // Favor center columns 
        int row = addValue(board, col, 'B');
        if (row != -1)
        {
            printf("Bot placed its checker in column %d (strategic move).\n", col + 1);
            printBoard(board);
            return;
        }
    }
    // 4. Fallback random move, like easy bot 
    int success = 0;
    while (!success)
    {
        int col = rand() % cols;
        int row = addValue(board, col, 'B');
        success = (row != -1);
        if (success)
        {
            printf("Bot placed its checker in column %d (random move).\n", col + 1);
            printBoard(board);
        }
    }
}
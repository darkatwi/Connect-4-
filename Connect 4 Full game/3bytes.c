#include <stdio.h>
#include <stdlib.h>

char board[rows][cols];

// Initialize the board with empty values
void initializeBoard(char board[rows][cols])
{
    for (int i = 0; i < rows; i++) 
        for (int j = 0; j < cols; j++)
            board[i][j] = '.';
    // Initialize transposition table
    initTranspositionTable();
    // Initialize Zobrist hashing
    initZobrist();
}
// Print the board
void printBoard(char board[rows][cols])
{
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (board[i][j] == 'A')
                printf("\033[31m%c \033[0m", board[i][j]); // Red
            else if (board[i][j] == 'B')
                printf("\033[34m%c \033[0m", board[i][j]); // Blue
            else
                printf("%c ", board[i][j]); // Empty
        }
        printf("\n");
    }
    for (int j = 1; j <= cols; j++)
        printf("%d ", j);
    printf("\n\n");
}
// Add a value to a column, return row index or -1 if invalid
int addValue(char board[rows][cols], int col, char player)
{
    if (col < 0 || col >= cols)
        return -1; // Invalid column 
    for (int i = rows - 1; i >= 0; i--)
    {
        if (board[i][col] == '.')
        {
            updateHash(i, col, '.', player);
            board[i][col] = player; // PLace player's checker in chosen column
            return i;
        }
    }
    return -1; // Invalid choice
}
// Remove a value (for undo in minimax)
void removeValue(int row, int col, char player)
{
    updateHash(row, col, player, '.');
    board[row][col] = '.'; 
}
// Check 4 in a row in any direction from a cell
int checkDirection(char board[rows][cols], char player, int startRow, int startCol, int dRow, int dCol)
{
    for (int k = 0; k < 4; k++)  // Loop 4 times for 4 in a row
    {
        int r = startRow + k * dRow; // Moves in row direction
        int c = startCol + k * dCol; // Moves in column direction
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != player) // Check if out of bounds or if not the player's symbol
            return 0; // Not found
    }
    return 1; // Found
}
// Check if player has won
int checkWin(char board[rows][cols], char player)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (checkDirection(board, player, i, j, 0, 1)) // Right
                return 1;
            if (checkDirection(board, player, i, j, 1, 0)) // Down
                return 1;
            if (checkDirection(board, player, i, j, 1, 1)) // Diagonal down right
                return 1;
            if (checkDirection(board, player, i, j, 1, -1)) // Diagonal down left
                return 1;
        }
    }
    return 0;
}
// Check if board is full
int checkDraw(char board[rows][cols])
{
    for (int j = 0; j < cols; j++)
        if (board[0][j] == '.')
            return 0;
    return 1; // Draw
}

// Easy bot implementation
void EasyBotMove()
{
    int col, row, success = 0;
    while (!success)
    {
        col = rand() % cols; // Make a completely random move
        row = addValue(board, col, 'B'); // Returns row if valid & -1 if columns are full
        success = (row != -1); // Save in success (true or false / valid or invalid)
    }
    printf("Bot placed its checker in column %d (easy move).\n", col + 1); // Column + 1 since column is 0 based
    printBoard(board);
}

//medium bot

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

//transposition

TTEntry transpositionTable[TABLE_SIZE];

// Initialize transposition table
void initTranspositionTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        transpositionTable[i].hash = 0;
        transpositionTable[i].score = 0;
        transpositionTable[i].depth = -1; // Since 0 is a valid search depth
        transpositionTable[i].flag = 0; //flag is the bound 
        transpositionTable[i].best_move = -1; // Since 0 is a valid move
    }
}
// Lookup transposition table (now returns best move too)
int ttLookup(unsigned long long hash, int depth, int alpha, int beta, int *score, int *best_move)
{
    int index = hash % TABLE_SIZE; // Convert 64-bit hash into a valid array index (0 to 8,388,592)
    TTEntry *entry = &transpositionTable[index]; // Get a pointer to the entry at this index
    if (entry->hash == hash && entry->depth >= depth) // If entry is valid and usable
    {
        // Return the best move from this position if available
        if (best_move && entry->best_move != -1)
        {
            *best_move = entry->best_move;
        }
        if (entry->flag == 0)
        { // Exact score
            *score = entry->score;
            return 1;
        }
        else if (entry->flag == 1)
        { // Lower bound
            if (entry->score >= beta)
            {
                *score = entry->score;
                return 1;
            }
        }
        else if (entry->flag == 2)
        { // Upper bound
            if (entry->score <= alpha)
            {
                *score = entry->score;
                return 1;
            }
        }
    }
    // Even if we do not return early, we might have found a best move
    if (entry->hash == hash && best_move && entry->best_move != -1)
    {
        *best_move = entry->best_move;
    }
    return 0;
}

// Store in transposition table (now stores best move)
void ttStore(unsigned long long hash, int depth, int score, char flag, int best_move)
{
    int index = hash % TABLE_SIZE; // Fit hash into array
    TTEntry *entry = &transpositionTable[index]; // Create a pointer to that array
    // Replace if this is deeper or new
    if (entry->depth <= depth || entry->hash != hash)
    {
        entry->hash = hash;
        entry->score = score;
        entry->depth = depth;
        entry->flag = flag;
        entry->best_move = best_move;
    }
}





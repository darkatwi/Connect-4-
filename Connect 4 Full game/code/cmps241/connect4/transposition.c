#include "transposition.h"

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
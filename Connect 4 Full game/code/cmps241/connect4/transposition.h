#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#define TABLE_SIZE 8388593  // Large prime to reduce hash collisions by distributing entries more evenly

// Transposition table entry
typedef struct {
    unsigned long long hash; // Zobrist hash of the position
    int score; // Evaluated score for this position
    int depth; // How deep the search was
    char flag; // Type of bound (exact/lower/upper)
    int best_move; // Best move found at this position
} TTEntry;
extern TTEntry transpositionTable[TABLE_SIZE];
void initTranspositionTable(); // Initialization
// Lookup and store functions
int ttLookup(unsigned long long hash, int depth, int alpha, int beta, int *score, int *best_move);
void ttStore(unsigned long long hash, int depth, int score, char flag, int best_move);

#endif
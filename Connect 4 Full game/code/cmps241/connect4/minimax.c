#include "minimax.h"
#include "board.h"
#include "evaluation.h"
#include "transposition.h"
#include "hashing.h"

// Minimax with transposition tables, null move pruning, and enhanced features
int minimax(int depth, int alpha, int beta, int maximizingPlayer, int *best_col_ptr){
    //depth: how many moves ahead to search
    //alpha: best score for the maximizing player so far
    //beta: best score for the minimizing player so far
    //maximizingPlayer: 1-->it's the bot's turn & it wants to pick the move with the highest score
    //                  0-->it's the human's turn & the bot wants to simulate what the opponent would do if they play optimally --> the human
    //                      picks the move with the lowest score for the bot.
    //best_col_ptr: a pointer that’s used to store the best col for the bot to play at the top level of the recursion.


    // Check transposition table
    int ttScore; //stores the cached score if we’ve seen this board before.
    int ttMove= -1; //stores the previously best move from the table, we start with -1 in case there isn’t one.
    if (ttLookup(currentHash, depth, alpha, beta, &ttScore, &ttMove)){
        //checks the TT table to see if this board, depth, alpha/beta have been evaluated before 
        return ttScore; //if so then no need to recalc, just return the cached score
    }
    
    int valid_cols[cols], valid_count = getValidLocations(valid_cols); //stores valid cols and number of valid cols
    int terminal = checkWin(board, 'A') || checkWin(board, 'B') || (valid_count == 0); //=1 if game has ended
    if (depth == 0 || terminal){ //base case --> we've searched the max depth or game ended
        int score; //stores how good the position is for the bot 
        if (terminal){  //if game has ended
            if (checkWin(board, 'B')) //if B has won (bot)
                score = 100000 + depth; // Prefer faster wins
            else if (checkWin(board, 'A')) //human wins
                score = -100000 - depth; // Prefer slower losses
            else 
                score = 0; // Draw
        }
        else{ //depth limit reached 
            score = score_position('B');
        }
        ttStore(currentHash, depth, score, 0, -1); //use a heuristic evaluation fn (score_position) & store result in TT
        return score;
    }

    int originalAlpha = alpha; //save the curr alpha val before we start exploring moves
    int best_move = -1; //start at -1 because we dont have anything yet & 0 is a valid value 
    if (maximizingPlayer){ //if its the bot's turn:
        // Null move pruning - skip our turn to see if position is still good
        if (depth >= 3 && !terminal && countThreats('A') == 0){ //enough moves left to make it meaningful & if no threats & game isnt over yet
            int null_score = minimax(depth - 3, alpha, beta, 0, best_col_ptr); //we skip looking at this turn & look 3 moves ahead
            if (null_score >= beta) //compare null score (score if bot skips the turn) & beta (the best score the human (min) can get)
            //if skipping still gives a score = or higher than beta
            {
                return beta; //there's no point of exploring further moves since the oppoent cant do better so return beta 
            }
        }

        orderMoves(valid_cols, valid_count, 'B', ttMove); //reorder moves to check most promising ones first since 
        //alpha-beta pruning works better when good moves are explored first

        int value = -10000000; //start with a very low score so any real move will improve it.
        for (int i = 0; i < valid_count; i++){ //valid count is nb of valid cols
            int col = valid_cols[i];
            int row = addValue(board, col, 'B');
            if (row == -1) //skip this round if invalid 
                continue;
            int new_score = minimax(depth - 1, alpha, beta, 0, best_col_ptr); //recursively simulate the human's response 
            //we basically call minimax on the new board by reducing depth by 1 and setting maximizing player to 0 to indicate humans turn
            removeValue(row, col, 'B'); //undo move to try next

            if (new_score > value){ //If this move gives a better score than current best, update
                value = new_score; //update 
                best_move = col; //save which col produced this best score
                if (depth == AI_DEPTH && best_col_ptr)
                    *best_col_ptr = col;
            }
            if (value > alpha) //if the best score we've seen so far in this loop is higher than the best score the bot can guarantee (alpha)
                alpha = value; //update
            if (alpha >= beta) //The human has a better option elsewhere so we don’t need to check the rest of these moves
            //This branch won’t be chosen by the human anyway.
                break; // Beta cutoff
        }
        // Store in transposition table
        char flag = (value <= originalAlpha) ? 2 : (value >= beta) ? 1 : 0; //tells the TT what type of score it is (exact, lower b, upper b)
        ttStore(currentHash, depth, value, flag, best_move);
        return value;
    }


    //bascially the human turn part
    else
    {
        // Null move pruning for minimizing player
        if (depth >= 3 && !terminal && countThreats('B') == 0) //enough moves, game not over, no threats (like before)
        {
            int null_score = minimax(depth - 3, alpha, beta, 1, best_col_ptr); //simulates the human skipping their turn
            if (null_score <= alpha)
            {
                return alpha; // Alpha cutoff
                //If skipping the human’s move already gives the bot a score better than alpha
                //we don’t need to explore further, human can’t save this branch anyway
            }
        }
        orderMoves(valid_cols, valid_count, 'A', ttMove); //order move
        int value = 10000000; //start with huge value so any move can be meaningful 
        for (int i = 0; i < valid_count; i++){ //loop through valid moves, place human piece in col, call minimax recursively, remove piece 
            int col = valid_cols[i];
            int row = addValue(board, col, 'A');
            if (row == -1)
                continue;
            int new_score = minimax(depth - 1, alpha, beta, 1, best_col_ptr);
            removeValue(row, col, 'A');
            
            //uodate best score 
            if (new_score < value)
            {
                value = new_score;
                best_move = col;
            }
            if (value < beta)
                beta = value;
            if (alpha >= beta)
                break; // Alpha cutoff
        }
        char flag = (value <= originalAlpha) ? 2 : (value >= beta) ? 1 : 0;
        ttStore(currentHash, depth, value, flag, best_move);
        return value;
    }
}
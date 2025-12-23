
# Connect 4 🎮

## Project Overview

This project implements a **console-based Connect 4 game** with **multiple bot difficulty levels**. The development was divided into four sprints:

1. **Two-player game**
2. **Easy-level bot**
3. **Medium-level bot**
4. **Hard-level bot** – near-perfect AI using **Minimax, iterative deepening, transposition tables, and opening tables**.


## Game Description

Connect 4 is a **turn-based strategy game** on a 7×6 board. Players take turns dropping pieces into columns, aiming to **connect four in a row** horizontally, vertically, or diagonally.

Features implemented:

* Full board display with colored pieces (`A` = red, `B` = blue).
* Move validation, win/draw detection.
* Bot difficulty selection: **Easy, Medium, Hard**.
* Console-based interaction.


## Code Structure

| File                                  | Description                               |
| ------------------------------------- | ----------------------------------------- |
| `main.c`                              | Game loop, input handling, mode selection |
| `board.c` / `board.h`                 | Board management and scoring              |
| `utils.c` / `utils.h`                 | Helper functions                          |
| `bot_easy.c` / `bot_easy.h`           | Easy bot logic (random moves)             |
| `bot_medium.c` / `bot_medium.h`       | Medium bot logic (tactical)               |
| `minimax.c` / `minimax.h`             | Minimax algorithm                         |
| `hashing.c` / `hashing.h`             | Zobrist hashing for transposition table   |
| `evaluation.c` / `evaluation.h`       | Alpha-beta evaluation function            |
| `transposition.c` / `transposition.h` | Transposition table management            |
| `Makefile`                            | Build automation                          |

---

## Sprint-wise Implementation

### Sprint 1 – Two-Player Game

* Console-based two-player game.
* Players alternate turns using `alternatePlayers()` and select columns using `getColumnFromUser()`.
* Board printed after each move (`printBoard()`).
* Win/draw detection (`checkWin()`, `checkDraw()`).

### Sprint 2 – Easy Bot

* Bot selects **random valid moves**.
* Ensured bot never chooses invalid columns.
* Console outputs bot moves.

### Sprint 3 – Medium Bot

* Attempts to **win immediately** if possible.
* Blocks opponent's immediate win.
* Prefers central columns for strategic advantage.
* Falls back to random moves if no other option.
* Complexity: O(1) per move due to small board size (7×6).

### Sprint 4 – Hard Bot

**Core Features:**

1. **Minimax Algorithm with Alpha-Beta Pruning** – depth = 11.
2. **Iterative Deepening** – builds transposition table at each depth.
3. **Transposition Table with Zobrist Hashing** – stores previously evaluated positions.
4. **Opening Table** – optimal first 1–3 moves.
5. **Evaluation Function** – scores positions based on patterns, center preference, immediate threats.
6. **Threat-Driven Move Ordering** – prioritizes winning/blocking moves.
7. **Null-Move Pruning** – skips some moves to reduce computation time.

**Performance:**

* Against humans: essentially unbeatable.
* Against other bots: likely to force draw or win.

---

## Testing Strategy

**Unit Testing:**

* Tested board operations (add/remove pieces, win/draw).
* Verified evaluation function and move ordering logic.

**Integration Testing:**

* Human vs Human gameplay.
* Bot vs Bot gameplay (Easy, Medium, Hard).
* Tested edge cases and invalid input.
* Checked console output correctness.

**Debugging Tools:**

* **GDB**: Step through minimax and threat detection.
* **Valgrind**: Ensured no memory leaks.

---

## Improvements Across Sprints

| Sprint | Improvement                                                                                                     |
| ------ | --------------------------------------------------------------------------------------------------------------- |
| 1      | Two-player game with full board and win detection                                                               |
| 2      | Easy bot with random valid moves                                                                                |
| 3      | Medium bot: blocks opponent, prioritizes center, tactical moves                                                 |
| 4      | Hard bot: minimax, alpha-beta, iterative deepening, transposition table, opening table, threat-aware evaluation |

---

## Tournament Readiness

* Hard bot is fast, tactical, and **practically unbeatable**.
* Opening table ensures strong early-game strategy.

---

## Limitations

* Hard bot may slow at depth 11 if transposition table is cold.
* Opening table only covers first 2–3 plies.
* Console interface limits visualization.
* Not multi-threaded.

---

## Alpine Setup

```bash
apk update
apk add build-base
git clone <repo-url>
```

Set `tty1` for auto-update if needed.

---

## Network Programming (Player vs Player)

**Architecture:** Client-server over `127.0.0.1` using TCP.

* **Server:** Waits for connection, maintains board, sends updates.
* **Client:** Connects, sends moves, receives updated board.

Typical functions: `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`.

---

## Conclusion

* Project progressed from **easy → medium → hard bot**.
* Hard bot combines **iterative deepening, transposition tables, opening table, threat-aware evaluation**.
* Depth 11 ensures dominance against human players and other bots.

**Future Enhancements:**

* Dynamic depth adjustment.
* Multithreading.
* Full-width endgame solver.

---

## References

1. Science Buddies (2024) — [Minimax Algorithm with Alpha-Beta Pruning in Connect 4](https://www.youtube.com/watch?v=DV5d31z1xTI)
2. Koushik Chandra Maji (2024) — [AI: Minimax AlphaBeta Connect4 in Java](https://medium.com/@koushiknsec34/ai-minimaxalphabeta-pruning-connect4-game-in)
3. Part 4 – Alpha-beta algorithm (2017) — [Gamesolver.org](http://blog.gamesolver.org/solving-connect-four/04-alphabeta/)
4. Sawwan, A. (2021) — [Artificial Intelligence-Based Connect 4 Player Using Python](https://cis.temple.edu/~pwang/5603-AI/Project/2021S/Sawwan/AI%20Project.pdf)
5. Aberent — [Transposition Table & Zobrist Hashing](https://adamberent.com/transposition-table)
6. GeeksforGeeks (2023) — [Minimax Algorithm in Game Theory — Zobrist Hashing](https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-5-zobrist-hashing/)



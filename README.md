# C++ implementation of the board game 'Impasse' with AI based on alpha-beta search

![Impasse board](https://cloud.bghorvath.dev/wl/?id=1n4TUTzu7TWv04nG9U0Q3ArkB0gBj5rn&fmode=open)
# Information
Game rules: https://www.marksteeregames.com/Impasse_rules.pdf

An Impasse engine with AI utilizing:
- Negamax search with alpha-beta pruning
- Iterative deepening with dynamic time allocation
- Transposition table with Zobrist hashing
- Move ordering based on heuristic evaluation

More: https://bghorvath.dev/projects/impasse

# Usage
Built executables for Windows and Linux are available in the [releases](https://github.com/bghorvath/impasse/releases) section.
Alternatively, build from source:
```
mkdir build
cd build
cmake ..
make
./impasse
```

# Acknowledgments
- [Chess programming wiki](https://www.chessprogramming.org/Main_Page)


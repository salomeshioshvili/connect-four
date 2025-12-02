# Connect Four

A feature-rich Connect Four game implemented in C with both console and graphical interfaces.

![C](https://img.shields.io/badge/language-C-blue.svg)
![CMake](https://img.shields.io/badge/build-CMake-green.svg)
![SDL2](https://img.shields.io/badge/graphics-SDL2-orange.svg)

## Features

- **Multiple Game Modes**
  - Player vs Player (Console)
  - Player vs AI (Console)
  - Player vs Player (Graphics) - requires SDL2
  - Player vs AI (Graphics) - requires SDL2

- **Four AI Difficulty Levels**
  - **Easy**: Random valid moves
  - **Medium**: Blocks opponent wins, takes winning moves
  - **Hard**: Strategic evaluation with scoring heuristics
  - **Expert**: Minimax-based AI with trap detection - nearly unbeatable!

- **Game Features**
  - Undo moves (in Player vs AI mode)
  - Move history tracking
  - Win/Draw detection
  - Clean terminal UI with board visualization
  - Modern graphical interface with SDL2

## Requirements

- **C Compiler**: GCC, Clang
- **CMake**: Version 3.21 or higher
- **SDL2** (optional): For graphical mode

## Installation

### Clone the Repository

```bash
git clone https://github.com/salomeshioshvili/connect-four.git
cd connect-four
```

### Install Dependencies

**macOS:**
```bash
brew install cmake sdl2
```

**Ubuntu/Debian:**
```bash
sudo apt install cmake libsdl2-dev
```

### Build the Project

```bash
# Configure
cmake -B build

# Build
cmake --build build

# Run the game
./build/src/connect4
```

### Build Without Graphics (Console Only)

If you don't have SDL2 installed, the game will automatically build in console-only mode.

## How to Play

### Console Mode

```
 0 1 2 3 4 5 6
 =============
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | |X| | | |
| | |O|X| | | |
```

- **X** = Player 1 (Red in graphics mode)
- **O** = Player 2 (Yellow in graphics mode)
- Enter column number (0-6) to drop a piece
- Press `u` to undo (AI mode only)
- Press `q` to quit

### Graphics Mode

- **Click** on a column to drop a piece
- **Keyboard 1-7** to select columns
- **U or Z** to undo moves
- **ESC or Q** to quit
- **Space/Enter** to play again after game ends

## Project Structure

```
connect-four/
├── CMakeLists.txt          # Root CMake configuration
├── README.md               # This file
├── include/                # Header files
│   ├── ai.h               # AI function declarations
│   ├── board.h            # Board data structures
│   ├── game.h             # Game state management
│   ├── graphics.h         # SDL2 graphics interface
│   ├── history.h          # Move history (undo support)
│   └── io.h               # Input/output utilities
├── src/                    # Source files
│   ├── CMakeLists.txt     # Source build configuration
│   ├── main.c             # Entry point
│   ├── ai.c               # AI implementations
│   ├── board.c            # Board logic
│   ├── game.c             # Game loop
│   ├── graphics.c         # SDL2 rendering
│   ├── history.c          # Move tracking
│   └── io.c               # Console I/O
└── tests/                  # Unit tests
    ├── CMakeLists.txt     # Test configuration
    ├── utest.h            # Testing framework
    ├── test_board.c       # Board tests
    ├── test_ai.c          # AI tests
    └── test_game.c        # Game logic tests
```

## AI Implementation Details

### Scoring Heuristic

The AI evaluates board positions using:
- **+3** points for pieces in the center column
- **+10** points for 2-in-a-row
- **+50** points for 3-in-a-row
- **+1000** points for 4-in-a-row (win)

### Expert AI Strategy

The Expert AI uses a minimax approach:
1. Immediately takes winning moves
2. Blocks opponent's winning moves
3. Creates "traps" (positions with 2+ winning threats)
4. Avoids moves that allow opponent traps
5. Evaluates all possible opponent responses

## Running Tests

```bash
# Build and run tests
cd build
ctest --output-on-failure

# Or run directly
./build/tests/board_tests
```

### Quick Development Commands

```bash
# Clean rebuild
rm -rf build && cmake -B build && cmake --build build

# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

```


## Authors

- SALOME SHIOSHVILI
- JININ AMEED OMAR ALJAYYOUSI
- NIKITA DJANDJGAVA
- DANA AHMAD QASEM DABBAH

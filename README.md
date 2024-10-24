Here’s the README formatted for easy copying:

---

# Minesweeper Project

## Project Overview

This project is a C++ implementation of the classic Minesweeper game. The goal of the game is to clear a grid of tiles without detonating hidden mines. You will utilize the SFML (Simple and Fast Multimedia Library) to handle graphics, input, and display, and apply C++ concepts such as pointers, recursion, and object-oriented programming.

### Features:
- **Board**: A grid with tiles, some of which contain hidden mines.
- **Tile Interaction**: Click to reveal a tile, right-click to place/remove flags. 
- **Game Logic**: If a mine is revealed, the game ends. Reveal all non-mine tiles to win.
- **Leaderboard**: Stores top 5 player times.
- **Multiple Windows**: Welcome window, game window, leaderboard window.
- **SFML Integration**: Handles rendering and mouse interactions.

## Setup

### Prerequisites:
- Install SFML (Simple and Fast Multimedia Library). Follow the instructions provided in the official SFML documentation: [SFML Download](https://www.sfml-dev.org/download.php).

### Compilation:
A `Makefile` is provided for compiling the project. Use the following commands depending on your system:

For Unix-based systems:
```bash
make
```

For Windows systems:
```bash
nmake (for Visual Studio)
mingw32-make (for MinGW)
```

### Running the Game:
After successful compilation, run the executable generated (e.g., `./minesweeper`) to start the game.

## File Structure

- **main.cpp**: The main file where game logic is handled.
- **src/**: Contains all source files related to the game's implementation.
- **images/**: Contains sprites used in the game (mines, tiles, flags, etc.).
- **board_config.cfg**: Configuration file for setting the board dimensions and the number of mines.
- **leaderboard.txt**: File storing the top 5 leaderboard records.

## How to Play

- Enter your name in the welcome window (max 10 characters).
- Reveal tiles by left-clicking. If a mine is revealed, you lose the game.
- Right-click to place a flag where you believe a mine is located.
- Win by revealing all non-mine tiles.
- Access the leaderboard via the button in the game window.



---

You can copy and paste this into your README file.

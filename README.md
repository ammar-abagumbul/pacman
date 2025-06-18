# Pacman Terminal Game

![Pacman](https://github.com/ammar-abagumbul/pacman/blob/765b0144c78f2459d957e10c075cd8ee6fb13b1d/pacman_screenshot.png)

A classic Pacman replica developed in C++ using the ncurses library for terminal-based gameplay.

## Features

This implementation includes most elements of the original Pacman game:
- Classic maze layout with dots and power pellets
- Player-controlled Pacman character
- Intelligent ghost AI using pathfinding algorithms to track Pacman
- Score system and game mechanics faithful to the original

## Prerequisites

### ncurses Library Setup
This game requires the ncurses library version 6.3. You must download and place it in the `include/` directory before compilation.

**Download ncurses 6.3:**
https://invisible-island.net/ncurses/#download_ncurses

After downloading, extract the ncurses library files into the `include/` directory of this project.

## How to Run

1. Ensure ncurses 6.3 is properly installed in the `include/` directory
2. Compile the game:
   ```bash
   make
   ```
3. Run the game:
   ```bash
   ./game
   ```

## Controls

- **W** - Move up
- **A** - Move left  
- **S** - Move down
- **D** - Move right

## Game Mechanics

The ghosts use advanced pathfinding algorithms to intelligently pursue Pacman, making the gameplay challenging and engaging. Navigate through the maze, collect dots, and avoid the ghosts to achieve the highest score possible!

Enjoy the game!

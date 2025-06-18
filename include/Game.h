#include <chrono>
#include <fstream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "Direction.h"
#include "GameObject.h"
#include "Ghost.h"
#include "Map.h"
#include "Player.h"

#ifndef GAME_H
#define Game_H

#define LOAD_NEW 0
#define LOAD_SAVED 1

struct PowerBall {
  int y;
  int x;
  char type;

  bool operator<(const PowerBall &other) const {
    if (y < other.y)
      return true;
    if (x < other.x)
      return true;
    return false;
  }
};

class Game {
  public:
  Game() {}
  Game(std::string mapfile, int loadType);

  // Time variables
  int phaseTracker = 0;
  double currentPhaseTime = 0;

  char **map;

  // Variables for tracking different events
  bool isOver = false;
  bool isPaused = false;
  bool isNewGame = true;
  bool inputEvent = false; // Checks if user has requested to change pac-man's direction

  std::string gameBaseFile; // variable that save's path file for map blueprint

  // Game Objects
  Ghost blueGhost, redGhost, pinkGhost, orangeGhost;
  Ghost *ghosts[4];
  std::set<PowerBall> powerBalls;
  Player player;
  Map mapWindow;

  /**
   * Loads a new game and initializes objects.
   *
   * @param mapFile A string containing the file path to the map file (map.txt)
   */
  void LoadNewGame(std::string mapFile);

  /**
   * Loads a new game and initializes objects.
   *
   * @param savedGameFile A string containing the file path to the file where paused game is saved
   */
  void LoadOldGame(std::string savedGameFile);

  /**
   * Initializes player with the specified Object state (position and direction)
   *
   * @param playerState an ObjectState struct specifiying the positional
   *                    attributes of the player at initialization
   */
  void InitializePlayer(ObjectState playerState);

  /**
   * Initializes ghosts with their respective Object state as specified by parameter
   *
   * @param ghostsStatus A StartingStatusGhosts specifiying the positional attribtes of ghosts
   *                     at initialization
   */
  void InitializeGhosts(StartingStatusGhosts ghostsStatus);

  /**
   * Initializes power Balls (pacman pallets) from the map provided by the gameBaseFile
   * and adds them to the powerBalls set
   */
  void InitializePowerBalls();

  /**
   * Initializes the map object and renderers the first frame (both maze and info window)
   * to the screen. This function also initializes `map` with the correct char in each cell
   *
   * @param mapFile A string containing the file path to the map file (map.txt)
   */
  void InitializeMap(std::string mapFile);

  /**
   * Runs the game loop. This function is responsible for timing and controlling the
   * phases of the ghosts. It orchestrates most of the game logic.
   */
  void Run();

  /**
   * Restarts the game by refreshing the window and resetting the positional attributes
   * of the player and the ghosts to their initial spawning state
   */
  void Restart();

  /**
   * Calls the Map::Refresh method in order for all the updates to be rendered to the screen
   */
  void RenderMap();

  /**
   * Updates both `map` and `map_win` (maize) of `mapWindow`. This function is called always after
   * pacman and the ghosts have moved and move validations have been processed/
   */
  void UpdateMap();

  /**
   * Checks whether the pacman was caught by anyone of the ghosts by checking if
   * their positions overlap.
   *
   * @return True if pacman is caught without having charge. Else, returns false.
   */
  bool PlayerCaught();

  /**
   * Loops through all the ghosts of the game object and sets their movement mode to the
   * specified mode.
   *
   * @param chaseStatus A Ghost::Mode enum to specify the moving behavior of all the ghosts
   */
  void ShiftGhostMode(Ghost::Mode chaseStatus);

  /**
   * Saves the attribues player (pacman), ghosts, timer variables, gameBaseFile and powerballs.
   *
   * @param path A string containing the file path to the location where the variables are saved
   */
  void Save(std::string path);

  /**
   * Checks if the pacman overlapped with any food pallet. It the increases pacman's score and
   * shifts ghosts chase behavior and charges pacman depending on the type of the pallet
   */
  void PalletEaten();

  /**
   * When map is initialized from base file when loading old game, new powerBalls are added.
   * This function removes the pallets that don't exist in the saved file from `map` (2D array).
   */
  void ApplyCorrectionToMap();

  /**
   * This behaves like restart by restoring the positional attributes of the pacman and the ghosts
   * to their initial states. However, it also restores the powerBalls both to the map and to the
   * `powerBalls` set of Game.
   */
  void RestoreObjects();

  /**
   * Clears the contents in `savedGame.txt`. This function is called in the event
   * player lose and new event needs to be called.
   */
  void ClearSavedFile();

  /**
   * Listens to user input and returns the appropriate direction corresponding
   * to the key stroke
   */
  Direction ListenKeyEvent();

  private:
};

#endif
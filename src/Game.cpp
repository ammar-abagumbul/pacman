#include "../include/Game.h"
#include "../include/GameObject.h"
#include <iostream>
#include <string>
#include <thread>

// TODO: have a function that calculates row and col of map
#define ROWS 32
#define COLS 32

#define MAP_HEIGHT ROWS
#define MAP_WIDTH COLS * 2
#define FPS 5
#define SAVE_PATH "./savedGame.txt"
#define PLAYER_SPAWN_STATE \
  ObjectState { 23, 16, Direction::Left }
#define GHOST_SPAWN_STATE \
  ObjectState { 11, 16, Direction::Right }

Game::Game(std::string file, int loadType)
    : inputEvent(false) {
  std::srand(std::time(0));
  if (loadType == LOAD_NEW) {
    LoadNewGame(file);
  } else if (loadType == LOAD_SAVED) {
    LoadOldGame(file);
  }
}

void Game::LoadNewGame(std::string file) {
  gameBaseFile = file;
  InitializeMap(file);
  InitializePlayer(PLAYER_SPAWN_STATE);
  InitializePowerBalls();
  InitializeGhosts(StartingStatusGhosts{
      ObjectState{11, 13, Direction::Left},
      ObjectState{11, 14, Direction::Left},
      ObjectState{11, 15, Direction::Right},
      ObjectState{11, 16, Direction::Left}});
}

void Game::LoadOldGame(std::string file) {
  std::ifstream oldFile(file);
  if (oldFile.fail()) {
    std::cout << "Failed to open file" << std::endl;
    exit(0);
  }
  int status;
  int playerY, playerX, playerScore, playerLife, playerCharge;
  int redY, redX, pinkY, pinkX, blueY, blueX, orangeY, orangeX;
  Direction playerDir, redDir, pinkDir, blueDir, orangeDir;

  oldFile >> status >> playerY >> playerX >> playerDir >> playerScore >> playerLife >> playerCharge;
  oldFile >> redY >> redX >> redDir;
  oldFile >> pinkY >> pinkX >> pinkDir;
  oldFile >> blueY >> blueX >> blueDir;
  oldFile >> orangeY >> orangeX >> orangeDir;
  oldFile >> phaseTracker >> currentPhaseTime;
  oldFile >> gameBaseFile;

  int pBallY, pBallX;
  char pBallT;
  while (oldFile >> pBallY >> pBallX >> pBallT) {
    powerBalls.insert(PowerBall{pBallY, pBallX, pBallT});
  }

  oldFile.close();

  InitializeMap(gameBaseFile);
  InitializePlayer(ObjectState{playerY, playerX, playerDir});
  InitializeGhosts(StartingStatusGhosts{
      ObjectState{redY, redX, redDir},
      ObjectState{pinkY, pinkX, pinkDir},
      ObjectState{blueY, blueX, blueDir},
      ObjectState{orangeY, orangeX, orangeDir}});

  ApplyCorrectionToMap(); // Without correction, pallets previously eaten would reappear
  player.setCharge(playerCharge);
  player.setScore(playerScore);
  player.setLife(playerLife);
  mapWindow.Reset(map);
  mapWindow.DisplayInfo(player.getScore(), player.getLife());
}

void Game::ApplyCorrectionToMap() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      char c = map[i][j];
      if (c == '*' || c == '$') {
        auto it = powerBalls.find(PowerBall{i, j, map[i][j]});
        if (it == powerBalls.end()) {
          map[i][j] = '.';
        }
      }
    }
  }
}

void Game::InitializePlayer(ObjectState playerState) {
  player = Player(playerState.y, playerState.x);
  player.setDirection(playerState.direction);
  player.setSpawnState(PLAYER_SPAWN_STATE);
  map[player.getY()][player.getX()] = 'P';
}

void Game::InitializeGhosts(StartingStatusGhosts ghostsStatus) {
  redGhost = Ghost(ghostsStatus.redState, 'r');
  redGhost.setTargetState(ObjectState{30, 30});
  redGhost.setSpawnState(GHOST_SPAWN_STATE);
  map[redGhost.getY()][redGhost.getX()] = redGhost.getLetter();

  blueGhost = Ghost(ghostsStatus.blueState, 'b');
  blueGhost.setTargetState(ObjectState{1, 1});
  blueGhost.setSpawnState(GHOST_SPAWN_STATE);
  map[blueGhost.getY()][blueGhost.getX()] = blueGhost.getLetter();

  pinkGhost = Ghost(ghostsStatus.pinkState, 'p');
  pinkGhost.setTargetState(ObjectState{1, 30});
  pinkGhost.setSpawnState(GHOST_SPAWN_STATE);
  map[pinkGhost.getY()][pinkGhost.getX()] = pinkGhost.getLetter();

  orangeGhost = Ghost(ghostsStatus.orangeState, 'o');
  orangeGhost.setTargetState(ObjectState{30, 1});
  orangeGhost.setSpawnState(GHOST_SPAWN_STATE);
  map[orangeGhost.getY()][orangeGhost.getX()] = orangeGhost.getLetter();

  ghosts[0] = &redGhost;
  ghosts[1] = &pinkGhost;
  ghosts[2] = &blueGhost;
  ghosts[3] = &orangeGhost;
}

void Game::InitializeMap(std::string filePath) {
  // Create map array for internal purposes
  std::ifstream mapFile(filePath);
  if (mapFile.fail()) {
    std::cout << "Failed to open file base file" << gameBaseFile << std::endl;
    exit(0);
  }
  std::string line;
  int lineCount = 0;
  map = new char *[ROWS];

  while (std::getline(mapFile, line)) {
    map[lineCount] = new char[COLS];
    for (int i = 0; i < COLS; i++) {
      map[lineCount][i] = line.at(i);
    }
    lineCount++;
  }
  // Create Map class for display
  mapWindow = Map(MAP_HEIGHT, MAP_WIDTH);
  mapWindow.initialize();
  mapWindow.AddWalls(this->map);
  mapWindow.AddPowerBalls(this->map);
  mapFile.close();
}

void Game::InitializePowerBalls() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (map[i][j] == '*' || map[i][j] == '$') // '*' ordinary pallets and '$' power pallets
      {
        powerBalls.insert(PowerBall{i, j, map[i][j]});
      }
    }
  }
}

void Game::RenderMap() {
  mapWindow.Refresh();
}

void Game::UpdateMap() {
  // Add player to its new position on map and mapWindow
  map[player.getY()][player.getX()] = player.getLetter();
  mapWindow.AddAt(player.getY(), player.getX(), player.getIcon(), 1);

  // Erase player from its last position
  map[player.getLastY()][player.getLastX()] = '.';
  mapWindow.AddAt(player.getLastY(), player.getLastX(), "  ");

  for (int i = 0; i < 4; i++) {
    Ghost *g = ghosts[i];
    map[g->getY()][g->getX()] = g->getLetter();
    if (!player.getCharge()) // Player doesn't have charge
    {
      mapWindow.AddAt(g->getY(), g->getX(), g->getIcon(), i + 2);
    } else {
      // Flicker color from blue to white as pacman runs out of charge
      if (player.getCharge() <= 2 * FPS && player.getCharge() % 2 == 1) {
        mapWindow.AddAt(g->getY(), g->getX(), g->getIcon(), 8);
      } else {
        mapWindow.AddAt(g->getY(), g->getX(), g->getIcon(), 7);
      }
    }

    map[g->getLastY()][g->getLastX()] = '.';
    mapWindow.AddAt(g->getLastY(), g->getLastX(), "  ");
  }
  for (PowerBall p : powerBalls) {
    char c = map[p.y][p.x];
    if (c == '.') // Draw powerball if powerball's position is not overlapped by pacman or the ghosts
    {
      switch (p.type) {
      case '*':
      case '@':
        map[p.y][p.x] = '*';
        mapWindow.AddAt(p.y, p.x, GRAPE);
        break;
      case '$':
        map[p.y][p.x] = '$';
        mapWindow.AddAt(p.y, p.x, RED_BALL);
        break;
      }
    }
  }
}

Direction Game::ListenKeyEvent() {
  int ch = getch();
  if (ch != ERR) {
    switch (ch) {
    case 'w':
      inputEvent = true;
      return Direction::Up;
    case 'a':
      inputEvent = true;
      return Direction::Left;
    case 's':
      inputEvent = true;
      return Direction::Down;
    case 'd':
      inputEvent = true;
      return Direction::Right;
    case 'q':
      isPaused = true;
      break;
    default:
      break;
    }
  }
  return Direction::Directionless;
}

void Game::Save(std::string filePath) {
  std::ofstream outputFile(filePath);
  outputFile << 1 << std::endl;
  outputFile << player.getY() << " " << player.getX() << " " << player.getDirection() << " ";
  outputFile << player.getScore() << " " << player.getLife() << " " << player.getCharge() << std::endl;
  for (Ghost *g : ghosts) {
    outputFile << g->getY() << " " << g->getX() << " " << g->getDirection() << std::endl;
  }
  outputFile << phaseTracker << " " << currentPhaseTime << std::endl;
  outputFile << gameBaseFile << std::endl;
  for (PowerBall powerBall : powerBalls) {
    outputFile << powerBall.y << " " << powerBall.x << " " << powerBall.type << std::endl;
  }
}

void Game::ShiftGhostMode(Ghost::Mode mode) {
  for (Ghost *ghost : ghosts) {
    ghost->setStatus(mode);
    ghost->ReverseDirection(); // To signify to the player that ghosts are in a different mode
  }
}

bool Game::PlayerCaught() {
  for (Ghost *g : ghosts) {
    // Second and third conditon required in case player and ghost go past through each other
    // without sharing the same position at a given frame
    if ((player.getX() == g->getX() && player.getY() == g->getY()) || (player.getX() == g->getLastX() && player.getY() == g->getLastY()) ||
        (player.getLastX() == g->getX() && player.getLastY() == g->getY())) {
      if (!player.getCharge())
        return true;
      else {
        player.increaseScore(100);
        mapWindow.DisplayInfo(player.getScore(), player.getLife());
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pause screen for effect
        map[g->getY()][g->getX()] = '.';
        mapWindow.AddAt(g->getY(), g->getX(), "  ");
        g->ResetState();
      }
    }
  }
  return false;
}

void Game::PalletEaten() {
  char c = map[player.getY()][player.getX()];
  if (c == '*' || c == '$') // check if pacman overlapped with a pallet
  {
    for (auto it = powerBalls.begin(); it != powerBalls.end();) {
      if (it->y == player.getY() && it->x == player.getX() && it->type == c) {
        if (it->type == '$') {
          player.addCharge(FPS * 6);
          ShiftGhostMode(Ghost::Mode::RANDOM);
        }
        player.increaseScore(it->type);
        it = powerBalls.erase(it);                                  // Delete powerball from the set
        mapWindow.DisplayInfo(player.getScore(), player.getLife()); // Update info on the info display window
      } else {
        ++it;
      }
    }
  }
}

void Game::Restart() {
  // Update objects' position attributes to spawn state and erase them from their current position
  map[player.getY()][player.getX()] = '.';
  player.ResetState();
  map[player.getY()][player.getX()] = player.getLetter();
  for (Ghost *g : ghosts) {
    map[g->getY()][g->getX()] = '.';
    g->ResetState();
    map[g->getY()][g->getX()] = g->getLetter();
  }

  mapWindow.Reset(map);
  phaseTracker = 0;
  currentPhaseTime = 0;
  for (Ghost *g : ghosts)
    g->setStatus(Ghost::Mode::RANDOM);
}

void Game::ClearSavedFile() {
  std::ofstream saveFile(SAVE_PATH);
  if (saveFile.fail()) {
    std::cout << "Failed to Save gaime" << std::endl;
  }
  saveFile << "0";
  saveFile.close();
}

void Game::RestoreObjects() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::ifstream mapFile(gameBaseFile);
  if (mapFile.fail()) {
    std::cout << "Failed to open file base file" << gameBaseFile << std::endl;
    exit(0);
  }
  std::string line;
  int lineCount = 0;
  while (std::getline(mapFile, line)) {
    for (int i = 0; i < COLS; i++) {
      map[lineCount][i] = line.at(i); // Restores the map and all pallets to the map
    }
    lineCount++;
  }
  player.setCharge(0);
  InitializePowerBalls(); // Pallets are added to the powerBalls set
  currentPhaseTime = 0;   // Timer variables of the game are reset
  phaseTracker = 0;
}

void Game::Run() {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::seconds;
  using std::chrono::steady_clock;

  if (isNewGame) {
    isNewGame = false;
  }
  steady_clock::time_point currentTime = steady_clock::now();
  steady_clock::time_point lastTime = steady_clock::now();

  Direction user_input = Direction::Left;

  std::vector<int> phaseDurations = {5, 7, 30};
  Ghost::Mode modes[3] = {Ghost::Mode::RANDOM, Ghost::Mode::SCATTER, Ghost::Mode::CHASE};

  for (Ghost *g : ghosts) {
    g->setStatus(modes[phaseTracker]);
  }

  while (true) {
    currentTime = steady_clock::now();
    auto cycleElapsedTime = duration_cast<milliseconds>(currentTime - lastTime).count(); // time since last frame updates

    if (!inputEvent) {
      user_input = ListenKeyEvent();
      if (isPaused || isOver)
        break;
    }

    if (cycleElapsedTime > 1000 / FPS) {
      if (!player.getCharge()) // Freeze phase timer when pacman has charge. Resume when pacman is out of charges
        currentPhaseTime += (1000 / FPS) / 1000.0;
      if (currentPhaseTime > phaseDurations[phaseTracker]) // time for phase change
      {
        if (phaseTracker + 1 > 2) // going to go out of bounds
        {
          phaseTracker = 0;
        } else {
          phaseTracker++;
        }
        ShiftGhostMode(modes[phaseTracker]);
        currentPhaseTime = 0;
        for (Ghost *g : ghosts) {
          g->Move(map);
        }
      } else {
        for (int i = 0; i < 4; i++) {
          Ghost *g = ghosts[i];
          if (g->IsAtIntersection(map)) {
            g->ChangeDirection(player.getState(), ObjectState{}, map);
          }
          g->Move(map);
        }
      }

      if (inputEvent) {
        player.ChangeDirection(user_input, map);
        inputEvent = false;
      }
      player.Move(map);
      PalletEaten();
      if (player.ConsumeCharge() && !player.getCharge()) // pacman just consumed all of his charge
      {
        for (Ghost *g : ghosts) {
          g->setStatus(modes[phaseTracker]);
        }
      }
      UpdateMap();
      RenderMap();
      if (powerBalls.empty()) {
        RestoreObjects();
        Restart();
      }

      if (PlayerCaught()) {
        player.decreaseLife();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        mapWindow.DisplayInfo(player.getScore(), player.getLife());
        if (player.getLife() <= 0) {
          isOver = true;
        } else {
          Restart();
        }
      }

      lastTime = currentTime;
    }
  }
}

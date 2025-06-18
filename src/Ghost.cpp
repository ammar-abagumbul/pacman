#include "../include/Ghost.h"
#include <cstdlib>
#include <map>
#include <queue>

const int Rows = 32;
const int Cols = 32;

void Ghost::Move(char **map) {

  int tempX = getX();
  int tempY = getY();
  Direction failOver;
  // A failover method where by if ghost is running into wall, their direction is changed until correct one is found
  switch (direction) {
  case Direction::Left:
    failOver = Direction::Up;
    tempX--;
    break;
  case Direction::Right:
    tempX++;
    failOver = Direction::Down;
    break;
  case Direction::Up:
    tempY--;
    failOver = Direction::Right;
    break;
  case Direction::Down:
    tempY++;
    failOver = Direction::Left;
    break;
  case Direction::Directionless:
    direction = Direction::Left;
    return;
  }
  if (map[tempY][tempX] == '#') {
    direction = failOver;
    return;
  }

  saveLastPos();
  setPos(tempY, tempX);
}

void Ghost::ChangeDirection(ObjectState playerState, ObjectState ghostState, char **map) {
  switch (mode) {
  case Mode::CHASE:
    followTarget(playerState, map);
    break;
  case Mode::SCARED:
  case Mode::RANDOM:
    PickRandomDirection(map);
    break;
  case Mode::SCATTER:
    followTarget(targetState, map);
    break;
  }
}

void Ghost::ReverseDirection() {
  switch (direction) {
  case Direction::Up:
    direction = Direction::Down;
    break;
  case Direction::Down:
    direction = Direction::Up;
    break;
  case Direction::Left:
    direction = Direction::Right;
    break;
  case Direction::Right:
    direction = Direction::Left;
    break;
  default:
    break;
  }
}

bool Ghost::IsAtIntersection(char **map) {
  int y = getY(), x = getX();
  if (map[y - 1][x] != '#' && (map[y][x + 1] != '#' || map[y][x - 1] != '#')) {
    return true;
  }
  if (map[y + 1][x] == '.' && (map[y][x + 1] != '#' || map[y][x - 1] != '#')) {
    return true;
  }
  return false;
}

int *Ghost::AvailableTurns(char **map) {
  int y = getY();
  int x = getX();

  int *turns = new int[4]{
      (map[y - 1][x] != '#') ? 1 : 0,
      (map[y][x + 1] != '#') ? 1 : 0,
      (map[y + 1][x] != '#') ? 1 : 0,
      (map[y][x - 1] != '#') ? 1 : 0};

  // Zero's the index corresponding to the opposite direction of current movement
  switch (direction) {
  case Direction::Up:
    turns[2] = 0;
    break;
  case Direction::Right:
    turns[3] = 0;
    break;
  case Direction::Down:
    turns[0] = 0;
    break;
  case Direction::Left:
    turns[1] = 0;
    break;
  default:
    break;
  }

  return turns;
}

void Ghost::PickRandomDirection(char **map) {
  Direction directions[] = {Direction::Up, Direction::Right, Direction::Down, Direction::Left};
  int *turns = AvailableTurns(map);
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (turns[i] == 1) {
      count++;
    }
  }
  int randomIndex = rand() % count;
  int currentIndex = 0;

  for (int i = 0; i < 4; i++) {
    if (turns[i] == 1) {
      if (currentIndex == randomIndex) {
        setDirection(directions[i]);
        break;
      }
      currentIndex++;
    }
  }
  delete[] turns;
}

void Ghost::followTarget(ObjectState state, char **map) {
  direction = FindPath(state.y, state.x, map);
}

bool opposite(Direction dir1, Direction dir2) {
  return (dir1 == Direction::Up && dir2 == Direction::Down) ||
         (dir1 == Direction::Down && dir2 == Direction::Up) ||
         (dir1 == Direction::Left && dir2 == Direction::Right) ||
         (dir1 == Direction::Right && dir2 == Direction::Left);
}

Direction Ghost::FindPath(int targetY, int targetX, char **map) {
  // int targetY targetX map
  // Define the possible movements in each direction
  int y = getY();
  int x = getX();

  std::map<Direction, std::pair<int, int>> movements = {
      {Direction::Up, {-1, 0}},
      {Direction::Down, {1, 0}},
      {Direction::Left, {0, -1}},
      {Direction::Right, {0, 1}}};

  // Create a visited array to keep track of visited positions
  std::vector<std::vector<bool>> visited;
  visited.resize(Rows, std::vector<bool>(Cols, false));

  // Create a queue for BFS and enqueue the starting position
  std::queue<Node> queue;
  Node startNode = {y, x, direction, {}};
  queue.push(startNode);
  bool firstNodeFlag = true;

  // Perform BFS
  while (!queue.empty()) {
    Node currentNode = queue.front();
    queue.pop();
    // Check if the current position is the target
    if (currentNode.y == targetY && currentNode.x == targetX && !firstNodeFlag) {
      // If the target is reached, return the path
      return currentNode.path[0];
    }
    firstNodeFlag = false;
    // Explore the neighboring positions in the direction of movement
    for (const auto &movementPair : movements) {
      const Direction &direction = movementPair.first;
      const std::pair<int, int> &movement = movementPair.second;

      if (opposite(direction, currentNode.direction)) {
        continue; // Don't consider because ghosts are not allowed to reverse unless its a mode shift
      }
      int newY = currentNode.y + movement.first;
      int newX = currentNode.x + movement.second;
      // Check if the neighboring position is valid and not visited
      if (map[newY][newX] != '#' && !visited[newY][newX]) {
        visited[newY][newX] = true;
        // Create a new node for the neighboring position and enqueue it
        Node newNode = {newY, newX, direction, currentNode.path};
        newNode.path.push_back(direction);
        queue.push(newNode);
      }
    }
  }

  // No valid path's found (Doesn't happen in the game)
  return direction;
}
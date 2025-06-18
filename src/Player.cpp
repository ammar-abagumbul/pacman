#include "../include/Player.h"

void Player::Move(char **map) {
  int tempX = getX();
  int tempY = getY();

  switch (getDirection()) {
  case Direction::Left:
    tempX--;
    break;
  case Direction::Right:
    tempX++;
    break;
  case Direction::Up:
    tempY--;
    break;
  case Direction::Down:
    tempY++;
    break;
  case Direction::Directionless:
    break;
  }
  if (map[tempY][tempX] == '#') {
    return;
  }

  saveLastPos();
  setPos(tempY, tempX);
}

void Player::ChangeDirection(Direction dir, char **map) {
  char adjacent = '#';
  switch (dir) {
  case Direction::Up:
    adjacent = map[getY() - 1][getX()];
    break;
  case Direction::Down:
    adjacent = map[getY() + 1][getX()];
    break;
  case Direction::Left:
    adjacent = map[getY()][getX() - 1];
    break;
  case Direction::Right:
    adjacent = map[getY()][getX() + 1];
    break;
  default:
    break;
  }
  if (adjacent == '#') {
    return;
  }
  setDirection(dir);
}

bool Player::ConsumeCharge() {
  if (charge > 0) {
    charge--;
    return true;
  }
  return false;
}

void Player::increaseScore(char pallet) {
  switch (pallet) {
  case '*':
    score += 5;
  case '$':
    score += 10;
  }
}

void Player::increaseScore(int score) {
  this->score += score;
}

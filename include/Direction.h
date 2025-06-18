#include <iostream>

#ifndef DIRECTION_H
#define DIRECTION_H

// Enumeration representing different directions for Pac-man and Ghosts
enum class Direction {
  Directionless,
  Up,
  Down,
  Right,
  Left
};

// Overloaded stream insertion operator for Direction for the convincience of saving
inline std::ostream &operator<<(std::ostream &os, const Direction &direction) {
  switch (direction) {
  case Direction::Directionless:
    os << 0;
    break;
  case Direction::Up:
    os << 1;
    break;
  case Direction::Down:
    os << 2;
    break;
  case Direction::Right:
    os << 3;
    break;
  case Direction::Left:
    os << 4;
    break;
  default:
    os << 0;
  }
  return os;
}

// Overloaded stream extraction operator for Direction for the convinience of extracting from file
inline std::istream &operator>>(std::istream &is, Direction &direction) {
  int value;
  if (is >> value) {
    direction = static_cast<Direction>(value);
  }
  return is;
}
#endif
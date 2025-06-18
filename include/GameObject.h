#include "Direction.h"
#include <cstring>

#ifndef GAME_OBJ
#define GAME_OBJ

/**
 * Represents the state of an object in terms of its position and direction.
 */
struct ObjectState {
  int y;               // The y-coordinate of the object's position.
  int x;               // The x-coordinate of the object's position.
  Direction direction; // The direction the object is facing.
};

class GameObject {
  public:
  /**
   * Default constructor
   */
  GameObject()
      : y(0), x(0), lastY(0), lastX(0), icon("  ") {
  }

  /**
   * Constructor for the Ghost class with initial position parameters.
   *
   * @param y The y-coordinate of the object's initial position.
   * @param x The x-coordinate of the object 's initial position.
   */
  GameObject(int y, int x)
      : y(y), x(x), lastY(y), lastX(x), icon("  ") {
  }

  virtual ~GameObject() {}

  /**
   * Resets the position and direction of the object to its spawning state
   */
  void ResetState();

  /**
   * Changes the position attribute of the game object after checking if path exists.
   * Child classes have their own implementation
   *
   * @param map a 2d map representation that the Game object uses to track movement validity
   */
  virtual void Move(char **map);

  /**
   * Changes the direction attribute of the game object after checking if path exists.
   *
   * @param direction the request to which the object is to change direction
   * @param map a 2d map representation that the Game object uses to track movement validity
   */
  virtual void ChangeDirection(Direction direction, char **map);

  // Mutators and Accessors
  int getX() { return x; }
  int getY() { return y; }
  int getLastX() { return lastX; }
  int getLastY() { return lastY; }
  Direction getDirection() { return direction; }
  ObjectState getState() { return ObjectState{y, x, direction}; }

  void setSpawnState(ObjectState spawnState) { this->spawningState = spawnState; }
  void setX(const int x) { this->x = x; }
  void setY(const int y) { this->y = y; }
  void setLastX(const int x) { this->lastX = x; }
  void setLastY(const int y) { this->lastY = y; }
  void setPos(const int y, const int x) {
    this->x = x;
    this->y = y;
  }

  char *getIcon() { return this->icon; }
  char getLetter() {
    return this->letter;
  }

  void setDirection(const Direction direction) {
    this->direction = direction;
  }

  void saveLastPos() {
    lastY = y;
    lastX = x;
  }

  private:
  int y, x;
  int lastY, lastX;
  ObjectState spawningState;

  protected:
  Direction direction;
  char icon[3];
  char letter;
};

inline void GameObject::Move(char **map) {}
inline void GameObject::ChangeDirection(Direction direction, char **map) {}

inline void GameObject::ResetState() {
  y = spawningState.y;
  x = spawningState.x;
  direction = spawningState.direction;
}

#endif
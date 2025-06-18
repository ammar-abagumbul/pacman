#include "GameObject.h"

#ifndef PLAYER_H
#define PLAYER_H

#define ICON_SIZE 3
#define LIFES 3
#define FREEZE 'f'
#define SLOW 's'
#define SCARE '*'

/**
 * Represents pacman in the game, inheriting from the GameObject class.
 */
class Player : public GameObject {
  public:
  /**
   * Default constructor for the Player class.
   * Initializes the player with default values.
   */
  Player()
      : GameObject(0, 0) {
    strcpy(icon, "Pa");
    direction = Direction::Left;
    letter = 'P';
  }

  /**
   * Constructor for the Player class with specified position.
   *
   * @param y The y-coordinate of the player's position.
   * @param x The x-coordinate of the player's position.
   */
  Player(int y, int x)
      : GameObject(y, x) {
    strcpy(icon, "Pa");
    direction = Direction::Left;
    letter = 'P';
  }

  /**
   * Moves the player on the map based on the current direction.
   *
   * @param map The map array representing the game map.
   */
  void Move(char **map) override;

  /**
   * Changes the direction of the player.
   *
   * @param direction The new direction for the player.
   * @param map The map array representing the game map.
   */
  void ChangeDirection(Direction direction, char **map) override;

  // Acessors and mutators

  void decreaseLife() { lifes--; }
  void increaseScore(char pallet);
  void increaseScore(int score);
  void setLife(int lifes) { this->lifes = lifes; }
  void setScore(int score) { this->score = score; }
  int getScore() { return score; }
  void addCharge(int charge) { this->charge += charge; }
  void setCharge(int charge) { this->charge = charge; }
  int getCharge() { return charge; }
  bool ConsumeCharge();
  int getLife() { return lifes; }

  private:
  int lifes = LIFES;
  int score = 0;
  int charge = 0; // Obtained by eating power pallet in the game
};

#endif

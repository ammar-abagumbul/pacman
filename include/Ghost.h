#include "GameObject.h"
#include <vector>

#ifndef GHOST_H
#define GHOST_H

#define GHOST_RENDER_ICON "  "

/**
 * Represents the starting mode of multiple ghosts in a game.
 */
struct StartingStatusGhosts {
  ObjectState redState;    // The starting state of the red ghost.
  ObjectState pinkState;   // The starting state of the pink ghost.
  ObjectState blueState;   // The starting state of the blue ghost.
  ObjectState orangeState; // The starting state of the orange ghost.
};

/**
 * Represents a node in a graph or a position on a grid.
 */
struct Node {
  int y;                       // The y-coordinate of the node.
  int x;                       // The x-coordinate of the node.
  Direction direction;         // The direction associated with the node.
  std::vector<Direction> path; // The path leading to this node.
};

class Ghost : public GameObject {
public:
  /**
   * Enumerates the different mode modes of the ghost.
   */
  enum class Mode {
    CHASE,   // The ghost is in chase mode.
    SCATTER, // The ghost is in scatter mode.
    SCARED,  // The ghost is scared and vulnerable.
    RANDOM   // The ghost moves randomly.
  };
  /**
   * Default constructor the Ghost class
   */
  Ghost() : GameObject(0, 0) {
    strcpy(icon, GHOST_RENDER_ICON);
    direction = Direction::Directionless;
  }

  /**
   * Constructor for the Ghost class with initial position parameters.
   *
   * @param y The y-coordinate of the ghost's initial position.
   * @param x The x-coordinate of the ghost's initial position.
   */
  Ghost(int y, int x) : GameObject(y, x) {
    strcpy(icon, GHOST_RENDER_ICON);
    direction = Direction::Directionless;
  }

  /**
   * Constructor for the Ghost class with an initial state and letter.
   *
   * @param ghostState The initial state of the ghost.
   * @param letter The letter representing the ghost.
   */
  Ghost(ObjectState ghostState, char letter)
      : GameObject(ghostState.y, ghostState.x) {
    strcpy(icon, GHOST_RENDER_ICON);
    direction = ghostState.direction;
    this->letter = letter;
  }

  /**
   * Follows a target object on the game map by setting the direction of the
   * ghost to the direction return by the `FindPath()` method
   *
   * @param targetState The state of the target object to follow.
   * @param map The 2d char representation of the game map.
   */
  void followTarget(ObjectState target, char **);

  using GameObject::ChangeDirection;

  /**
   * Overrides the base class Move function to move the ghost on the game map.
   *
   * @param map The game map.
   */
  void Move(char **map) override;

  /**
   * Changes the direction of the ghost based on the player's and ghost's
   * states.
   *
   * @param playerState The state of the player.
   * @param ghostState The state of the ghost.
   * @param map The game map.
   */
  void ChangeDirection(ObjectState playerState, ObjectState ghostState,
                       char **map);

  /**
   * Picks a random direction for the ghost to move on the game map at an
   * intersection.
   *
   * @param map The game map.
   */
  void PickRandomDirection(char **map);

  /**
   * Reverses the direction of the ghost.
   */
  void ReverseDirection();

  /**
   * Checks if the ghost is at an intersection on the game map.
   *
   * @param map The game map.
   * @return True if the ghost is at an intersection, false otherwise.
   */
  bool IsAtIntersection(char **map);

  /**
   * Returns the available turns for the ghost on the game map. Reversing
   * direction is often not considered a valid turn unless the game logic calls
   * `ReverseDirection()` explicitly
   *
   * @param map The game map.
   * @return An array of available turns.
   */
  int *AvailableTurns(char **map);

  /**
   * Finds a path from the current ghost position to the specified coordinates
   * on the game map. Uses Breadth-first search algoirthm.
   *
   * @param y The y-coordinate of the target position.
   * @param x The x-coordinate of the target position.
   * @param map The game map.
   * @return The direction of the path to follow.
   */
  Direction FindPath(int y, int x, char **map);

  // Mutators and Accessors
  void setStatus(Mode mode) { this->mode = mode; }
  Mode getStatus() { return mode; }

  void setTargetState(ObjectState state) { targetState = state; }
  ObjectState getTargetState() { return targetState; }

private:
  ObjectState
      targetState; // The point that the ghost targets when in SCATTER mode
  Mode mode;
};

#endif

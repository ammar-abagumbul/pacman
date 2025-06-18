#include "Game.h"

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

/**
 * A class that is reponsible for displaying different windows based on users choice
 */
class Navigator {
  public:
  /**
   * Default constructor of the Navigator class.
   */
  Navigator() {}

  Game currentGame; // The current game that the navigator is overseeing

  /**
   * Starts the navigation system where an internal loop logic navigates all the windows
   * and scenes
   */
  void Start();

  /**
   * Gets all the possible actions the user can take by checking certain conditons
   * regarding the `savedGame.txt`
   *
   * @return options: A pointer to a list of strings corresponding to possible menu options
   */
  std::string *getMenuOptions();

  /**
   * Displays a menu window on the center of the terminal
   *
   * @return highlight: The integer corresponding to the users choice
   */
  int DisplayMenu(std::string *options, std::string msg);

  /**
   * Initializes all the color pairs for the `currentGame` to use afterwards.
   */
  void InitializeColor();
};

#endif

#include "locale.h"
#include "ncurses.h"
#include <string>

#ifndef MAP_H
#define MAP_H

#define BLUE 6
#define GRAPE "🍇"
#define RED_BALL "🔴"

/**
 * Represents a game map with a border and various elements on it.
 */
class Map {
  public:
  /**
   * Default constructor for the Map class.
   */
  Map() {
    construct(0, 0);
  }

  /**
   * Constructor for the Map class with specified height and width.
   *
   * @param height The height of the map.
   * @param width The width of the map.
   */
  Map(int height, int width) {
    construct(height, width);
  }

  /**
   * Initializes the map by clearing it, displaying the information window, and refreshing it
   */
  void initialize() {
    Clear();
    DisplayInfo(0, 0);
    Refresh();
  }

  /**
   * Adds a character at the specified position on the map.
   *
   * @param y The y-coordinate of the position.
   * @param x The x-coordinate of the position.
   * @param ch The character to add.
   */
  void AddAt(int y, int x, chtype ch) {
    mvwaddch(map_win, y, x, ch);
  }

  /**
   * Adds an icon at the specified position on the map.
   *
   * @param y The y-coordinate of the position.
   * @param x The x-coordinate of the position.
   * @param icon The icon to add.
   */
  void AddAt(int y, int x, std::string icon) {
    mvwprintw(map_win, y, 2 * x, icon.c_str());
  }

  /**
   * Adds an icon with a specified color at the specified position on the map.
   *
   * @param y The y-coordinate of the position.
   * @param x The x-coordinate of the position.
   * @param icon The icon to add.
   * @param colorIdx The color index to specify the desired color pair.
   */
  void AddAt(int y, int x, std::string icon, short colorIdx) {
    wattron(map_win, COLOR_PAIR(colorIdx));
    mvwaddstr(map_win, y, 2 * x, icon.c_str());
    wattroff(map_win, COLOR_PAIR(colorIdx));
  }

  chtype getInput() {
    return wgetch(map_win);
  }

  void Clear() {
    wclear(map_win);
  }

  /**
   * Refreshes the map window.
   */
  void Refresh() {
    wrefresh(map_win);
  }

  /**
   * Adds walls to the map based on the provided map array.
   *
   * @param map The map array representing walls.
   */
  void AddWalls(char **map) {
    for (int r = 0; r < 32; r++) {
      for (int c = 0; c < 32; c++) {
        if (map[r][c] == '#') {
          AddAt(r, c, "[]", BLUE);
        }
      }
    }
  }

  /**
   * Adds power balls to the map based on the provided map array.
   *
   * @param map The 2d char array representing the map.
   */
  void AddPowerBalls(char **map) {
    for (int r = 0; r < 32; r++) {
      for (int c = 0; c < 32; c++) {
        if (map[r][c] == '*') {
          AddAt(r, c, GRAPE);
        } else if (map[r][c] == '$') {
          AddAt(r, c, RED_BALL);
        }
      }
    }
  }

  /**
   * Resets the map by clearing it, adding walls and power balls, and refreshing it.
   *
   * @param map The map array representing the entire map.
   */
  void Reset(char **map) {
    Clear();
    AddWalls(map);
    AddPowerBalls(map);
    Refresh();
  }

  /**
   * Displays the game information in the info window.
   *
   * @param score Players current score.
   * @param life The remaining lives of pacman.
   */
  void DisplayInfo(int score, int life);

  // Returns map window
  WINDOW *getWindow() { return map_win; }

  private:
  WINDOW *map_win;
  WINDOW *info_win;

  void construct(int height, int width) {
    // construct map window
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    map_win = newwin(height, width, (yMax / 2) - (height / 2), 10);

    // consstruct info window
    info_win = newwin(height / 2, (xMax / 2) - 10, (yMax / 2) - (height / 4), xMax - width - 10);
  }
};

#endif
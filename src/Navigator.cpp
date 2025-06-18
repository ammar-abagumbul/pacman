#include "../include/Navigator.h"

#define MAX_OPTIONS 3

int countOptions(std::string *options);

void Navigator::Start() {
  std::string *options = getMenuOptions();
  std::string *interimOptions = nullptr;
  int userAction = DisplayMenu(options, "");
  bool programExitRequest = false;
  InitializeColor();
  currentGame = Game();
  while (true) {
    switch (userAction) {
    case 0:                      // Resume from saved game
      if (currentGame.isNewGame) // New game means the game object hasn't been initialized, it's not referring to
                                 // being loaded from a saved file or not
      {
        currentGame = Game("savedGame.txt", LOAD_SAVED);
        currentGame.Run();
      } else {
        currentGame.mapWindow.AddWalls(currentGame.map);
        currentGame.mapWindow.Refresh();
        currentGame.isPaused = false;
        currentGame.Run();
      }
      break;
    case 1: // New Game
      currentGame = Game("map.txt", LOAD_NEW);
      currentGame.Run();
      break;
    case 2: // Exit Game
      if (!currentGame.isNewGame && !currentGame.isOver)
        currentGame.Save("savedGame.txt");
      if (currentGame.isOver) // Can't resume a game that is already lost
        currentGame.ClearSavedFile();
      programExitRequest = true;
      break;
    }
    if (programExitRequest) {
      break;
    } else {
      if (currentGame.isOver) {
        interimOptions = new std::string[4]{"New Game", "Exit", "0"};
        userAction = DisplayMenu(interimOptions, "Game Over");
      } else {
        interimOptions = new std::string[4]{"Resume", "New Game", "Exit", "0"};
        userAction = DisplayMenu(interimOptions, "");
      }
    }
  }
  delete[] options;
  if (interimOptions != nullptr)
    delete[] interimOptions;
}

std::string *Navigator::getMenuOptions() {
  std::ifstream savedGame("savedGame.txt");
  bool resumableGameExists = false;
  if (savedGame.fail()) {
    std::string *options = new std::string[3]{"New Game", "Exit", "0"};
    return options;
  } else {
    savedGame >> resumableGameExists;
    if (savedGame.fail() || !resumableGameExists) {
      std::string *options = new std::string[3]{"New Game", "Exit", "0"};
      savedGame.close();
      return options;
    } else {
      std::string *options = new std::string[4]{"Resume", "New Game", "Exit", "0"};
      savedGame.close();
      return options;
    }
  }
}

int Navigator::DisplayMenu(std::string *options, std::string msg) {
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  int winHeight = 5;
  int winWidth = 30;
  int winY = (screenHeight - winHeight) / 2;
  int winX = (screenWidth - winWidth) / 2;

  WINDOW *menuWin = newwin(winHeight, winWidth, winY, winX);
  box(menuWin, 0, 0);

  short numOptions = countOptions(options);
  int highlight = 0;

  int ch;
  while ((ch = getch()) != '\n') {
    werase(menuWin);
    mvwprintw(menuWin, 0, (winWidth - msg.length()) / 2, msg.c_str());
    for (int i = 0; i < numOptions; i++) {
      if (i == highlight) {
        wattron(menuWin, A_REVERSE);
      }
      mvwprintw(menuWin, i + 1, (winWidth - options[i].length()) / 2, options[i].c_str());
      wattroff(menuWin, A_REVERSE);
    }

    int arrowY = highlight + 1;
    mvwprintw(menuWin, arrowY, 0, "->");
    mvwprintw(menuWin, arrowY, winWidth - 2, "<-");
    wrefresh(menuWin);

    switch (ch) {
    case KEY_UP:
      highlight--;
      if (highlight < 0) {
        highlight = numOptions - 1;
      }
      break;
    case KEY_DOWN:
      highlight++;
      if (highlight >= numOptions) {
        highlight = 0;
      }
      break;
    }
  }

  werase(menuWin);
  wrefresh(menuWin);
  delwin(menuWin);

  if (numOptions < MAX_OPTIONS)
    highlight++;
  return highlight;
}

void Navigator::InitializeColor() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);
  init_pair(2, COLOR_RED, COLOR_RED);
  init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(4, COLOR_CYAN, COLOR_CYAN);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_BLUE, COLOR_BLUE);
  init_pair(8, COLOR_WHITE, COLOR_WHITE);
}

int countOptions(std::string *options) {
  int count = 0;
  while (options[count] != "0") {
    count++;
  }
  return count;
}

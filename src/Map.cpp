#include "../include/Map.h"

void Map::DisplayInfo(int score, int life) {
  const char *pactext = R"(__________    ____   _____ _____    ____   )"
                        "\n"
                        R"(\____ \__  \ _/ ___\ /     \\__  \  /    \ )"
                        "\n"
                        R"(|  |_> > __ \\  \___|  Y Y  \/ __ \|   |  \ )"
                        "\n"
                        R"(|   __(____  /\___  >__|_|  (____  /___|  / )"
                        "\n"
                        R"(|__|       \/     \/      \/     \/     \/")"
                        "\n\n\n\n"
                        "             [w][a][s][d] to move\n"
                        "               [q] to pause/exit";
  wclear(info_win);
  box(info_win, 1, 1);
  wprintw(info_win, pactext);
  wprintw(info_win, "\n\n\n\n\n");
  wprintw(info_win, ("Score: " + std::to_string(score) + "\n").c_str());
  wprintw(info_win, "\n\n");
  wprintw(info_win, ("Life : " + std::to_string(life) + "\n").c_str());
  wrefresh(info_win);
}

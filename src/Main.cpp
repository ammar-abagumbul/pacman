#include "../include/Navigator.h"
#include <iostream>

int main() {
  setlocale(LC_ALL, "");
  initscr();
  refresh();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(0);

  Navigator navigator;
  navigator.Start();

  getch();
  endwin();

  return 0;
}

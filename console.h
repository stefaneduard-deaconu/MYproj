#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>

void gotoxy(short coord_x, short coord_y) {
  COORD p = {coord_y, coord_x};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
void show_console_cursor(bool show_flag) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show_flag;
  SetConsoleCursorInfo(out, &cursorInfo);
}

#endif // HEADER_H_INCLUDED

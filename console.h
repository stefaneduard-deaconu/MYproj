#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>

void gotoxy(short x, short y) {
  COORD p = {y, x};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
void ShowConsoleCursor(bool showFlag) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag;
  SetConsoleCursorInfo(out, &cursorInfo);
}

#endif // HEADER_H_INCLUDED

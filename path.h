#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include <windows.h>
#include "char_star.h"

char* path()
{
  char *p = new char[MAX_PATH];
  GetModuleFileName(NULL, p, MAX_PATH);
  int i = leng(p);
  do
    i--;
  while (p[i] != '\\');
  p[i] = 0;
  return p;
}

#endif // PATH_H_INCLUDED

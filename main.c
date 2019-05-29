#include <iostream>
#include <fstream>

#include <windows.h>
#include <conio.h>

using namespace std;

void ShowConsoleCursor(bool showFlag);
void gotoxy( short x, short y );

char* trim(char *a);
int leng(char *a)
{
    int lg = 0;
    while(a[lg])
        lg++;
    return lg;
}
void cpy(char *a, char *b)
{
    int i = 0;
    do
        a[i] = b[i];
    while(b[i++] != 0);
}

int main()
{
    
    return 0;
}

char* trim(char *a)
{
    char *b = new char[leng(a)];
    cpy(b, a); // b <- a
    return b;
}

void gotoxy( short x, short y )
{
    COORD p = { y, x };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
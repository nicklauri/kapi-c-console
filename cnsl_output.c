/**
 *  File:
 *  Author: Nick Lauri
 *
 *  Copyright (c) 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kapi.h>

#ifdef _LINUX_
    #include <unistd.h>
    #include <sys/ioctl.h>
#elif defined _WINDOWS_
    #include <windows.h>
#endif

bool clrscr() {
    fprintf(stdout, "\033[2J\033[0;0f\n");
    fflush(stdout);
    return true;
}

int getmaxX() {
#ifdef _LINUX_
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#endif
}

int getmaxY() {
#ifdef _LINUX_
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

bool gotoxy(size_t x, size_t y) {

#ifdef _LINUX_
    if(x > getmaxX() || y > getmaxY()) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: %s: invalid X or Y value.\n", __func__);
#endif
        return false;
    }

    printf("%c[%d;%df", 0x1b, y, x);
    return true;

#elif defined(_WINDOWS_)
    COORD coord;
    coord.X = x;
    coord.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord)
#endif
}


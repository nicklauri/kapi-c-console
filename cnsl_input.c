/**
 *  File: cnsl_input.c
 *  Project: KProject KAPI for C.
 *  Author: Nick Lauri
 *
 *  Copyright (c) 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <kapi.h>

#ifdef _LINUX_
    #include <stdio_ext.h>
    #include <termios.h>
    #include <unistd.h>
    #define k_getchar getchar
#elif defined _WINDOWS_
    #include <conio.h>
    #define k_getchar getch
#else
    #error  Unsupported system! Choose Linux or Windows environment.
#endif

#define STRING_BLOCK 100

/**
 *  k_strinput: read a line from console.
 */
char *k_strinput() {
    int i, strblk_counter;
    char c, *output;

    i = 0;
    strblk_counter = 1;
    output = (char *) malloc(strblk_counter * STRING_BLOCK * sizeof(char));
    if(!output) {
#ifdef _DEBUG_
        printf("kapi: k_strinput: unable to allocate memory!\n");
#endif
        return NULL;
    }

#ifdef _LINUX_
    __fpurge(stdin);
#elif defined _WINDOWS_
    fflush(stdin);
#endif

    while(true){
        c = fgetc(stdin);
        if(c == '\n') {
            output[i] = '\0';
            break;
        } else {
            if(i >= strblk_counter*STRING_BLOCK) {
                strblk_counter++;
                output = (char *) realloc(output, strblk_counter*STRING_BLOCK);
            }
            output[i] = c;
            i ++;
        }
    }
    return output;
}

/**
 *  k_strneinput: string input without echo.
 */

char *k_strneinput() {
    int i, strblk_counter;
    char c, *output;

#ifdef _LINUX_
    static struct termios old, new;
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);        // turn off echo flag
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
#endif

    i = 0;
    strblk_counter = 1;
    output = (char *) malloc(strblk_counter * STRING_BLOCK * sizeof(char));
    if(!output) {
#ifdef _DEBUG_
        printf("kapi: k_strinput: unable to allocate memory!\n");
#endif
        return NULL;
    }

    while(true){
        c = k_getchar();
        if(c == '\n') {
            output[i] = '\0';
            putchar(c);
            break;
        } else if(c == '\b') {
            puts("\b \b");
            continue;
        } else {
            if(i >= strblk_counter*STRING_BLOCK) {
                strblk_counter++;
                output = (char *) realloc(output, strblk_counter*STRING_BLOCK);
            }
            output[i] = c;
            i ++;
        }
    }

#ifdef _LINUX_
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
    return output;
}

/**
 *  k_streinput: string input with echoed-char.
 */
char *k_streinput(char ch) {
    int i, strblk_counter;
    char c, *output;

#ifdef _LINUX_
    static struct termios old, new;
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
#endif

    i = 0;
    strblk_counter = 1;
    output = (char *) malloc(strblk_counter * STRING_BLOCK * sizeof(char));
    if(!output) {
#ifdef _DEBUG_
        printf("kapi: k_strinput: unable to allocate memory!\n");
#endif
        return NULL;
    }

    while(true){
        c = k_getchar();
        if(c == '\n') {
            output[i] = '\0';
            putchar(c);
            break;
        } else {
            if(i >= strblk_counter*STRING_BLOCK) {
                strblk_counter++;
                output = (char *) realloc(output, strblk_counter*STRING_BLOCK);
            }

            if(c == 127) {
                if(i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else {
                putchar(ch);
                output[i] = c;
                i ++;
            }
        }
    }

#ifdef _LINUX_
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
    return output;
}


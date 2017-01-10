
#ifndef _KAPI_H
#define _KAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifdef _LINUX_
    #include <unistd.h>
    #include <termios.h>
#elif defined _WINDOWS_
    #include <conio.h>
#else
    #error Choose platform: -D_LINUX: Linux; -D_WINDOWS_: Windows.
#endif

#define _debug printf("%s:%d\n", __func__, __LINE__);

#ifdef __cplusplus
    extern "C" {
#endif

/*************************** KAPI: console input ****************************/

/**
 *  @func: k_strinput
 *  @args: (null)
 *  @ret : (char *) a pointer to string (the same as above)
 *  @desc: return a string without segmentation fault ;)
 *         String is allocated on HEAP
 */
char *k_strinput(void);

/**
 *  @func: k_strneinput
 *  @args: (null)
 *  @ret : (char *) a pointer to string (the same as above)
 *  @desc: input a char without echo. String is allocated on HEAP.
 */
char *k_strneinput(void);

/**
 *  @func: k_streinput
 *  @args: (null)
 *  @ret : (char *) a pointer to string (the same as above)
 *  @desc: input a char and echo the given character.
 *          String is allocated on HEAP
 */
char *k_streinput(char);

/*************************** KAPI: console output ****************************/

int  getmaxX();
int  getmaxY();
bool clrscr();
bool gotoxy(size_t, size_t);

/*************************** KAPI: cstring        ****************************/
/**
 *  @func: k_strcpyft
 *  @args: 
 *      char    *src  : source string.
 *      size_t  from  : position to start copying.
 *      size_t  nbytes: number of bytes to copy.
 *  @ret : - (char *) a pointer to string (the same as above)
 *         - NULL if errors.
 *  @desc: copy nbytes at 'from' position from src to dst.
 *          dst MUST be allocated before passing to this function.
 *          Allocated dst by malloc().
 */
char *k_strcpyft(char *, size_t, size_t);

/**
 *  @func: k_strtoupper
 *  @args: 
 *      char    *src  : source string.
 *  @ret : - (char **) a pointer to array of UPPER chars.
 *         - NULL if errors.
 *  @desc: split char* into char** with delimiter delim.
 *          array and its member are allocated by malloc().
 */
char *k_strtoupper(const char *);

/**
 *  @func: k_strtolower
 *  @args: 
 *      char    *src  : source string.
 *  @ret : - (char **) a pointer to array of LOWER chars.
 *         - NULL if errors.
 *  @desc: split char* into char** with delimiter delim.
 *          array and its member are allocated by malloc().
 */
char *k_strtolower(const char *);

/**
 *  @func: k_strtoarray
 *  @args: 
 *      char    *src  : source string.
 *      char    *delim: delimiter string.
 *  @ret : - (char **) a pointer to array of string.
 *         - NULL if errors.
 *  @desc: split char* into char** with delimiter delim.
 *          array and its member are allocated by malloc().
 */
char **k_strtoarray(char *, char *);

/*************************** KAPI: cobj        ****************************/
typedef struct OBJ_T {
    int  len, *type;
    union {
        bool  b;
        char  c;
        short s;
        int   i;
        uint  u;
        float f;
        long  l;
        void *v;
        void *(*func)(void *, ...);
    } *val;
} obj_t;

static char *stype[] = {"NULL", "BOOL", "BYTE", "SHORT", "INTEGER", 
        "UINTERGER", "FLOAT", "LONG", "STRING", "FUNCTION"};
static size_t stype_len = 10;

#define D_NULL      0
#define D_BOOL      1
#define D_BYTE      2
#define D_CHAR      D_BYTE
#define D_SHORT     3
#define D_INTEGER   4
#define D_UINTEGER  5
#define D_FLOAT     6
#define D_LONG      7
#define D_STRING    8
#define D_FUNCTION  9

obj_t *kobj_init();
int kobj_append(obj_t *, int, void *);
int kobj_insert(obj_t *, size_t, int, void *);
int kobj_delete(obj_t *, size_t);
int kobj_free(obj_t *);
bool kobj_print(obj_t *);

#ifdef __cplusplus
}
#endif
#endif

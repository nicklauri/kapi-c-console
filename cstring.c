/**
 *  File: cstring.c
 *  Project: KProject KAPI for C.
 *  Author: Nick Lauri
 *
 *  Copyright (c) 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <kapi.h>

#define ARRAY_BLOCK  10
#define STRING_BLOCK 100

///
///     k_strcpyft
///
char *k_strcpyft(char *src, size_t nbytes, size_t from) {
    int i, j;
    char *dst;
    dst = (char *) malloc(nbytes*sizeof(char));
    if(!dst) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: k_strcpyft: unable to allocate memory.\n");
#endif
        return NULL;
    }
    for(i = from, j = 0; i < strlen(src) && j < nbytes; i++, j++)
        dst[j] = src[i];
    dst[j] = '\0';
    return dst;
}

///
///     k_strtoupper
///
char *k_strtoupper(const char *src) {
    int i = 0;
    char *dst = (char *) malloc(strlen(src));
    if(!dst) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: k_strtoupper: unable to allocate memory.\n");
#endif
        return NULL;
    }
    while(i < strlen(src)) {
        dst[i] = toupper(src[i]);
        i++;
    }
    return dst;
}

///
///     k_strtolower
///
char *k_strtolower(const char *src) {
    int i = 0;
    char *dst = (char *) malloc(strlen(src));
    if(!dst) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: k_strtolower: unable to allocate memory.\n");
#endif
        return NULL;
    }
    while(i < strlen(src)) {
        dst[i] = tolower(src[i]);
        i++;
    }
    return dst;
}

///
///     k_strtoarray
///
char **k_strtoarray(char *src, char *delim) {
    int len_delim, len_src, i, j, arr_counter, p;
    bool is_notnullafterthelastcopy;
    char *buff, **arr;

    arr  = (char **) malloc(ARRAY_BLOCK * sizeof(char **));
    if(!arr) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: k_strtoarray: unable to allocate memory.\n");
#endif
        return NULL;
    }

    i = 0;
    j = 1;
    p = 0;
    arr_counter = 0;
    len_delim = strlen(delim);
    len_src   = strlen(src);
    is_notnullafterthelastcopy = true;
    while(i < len_src) {
        if(strcmp(k_strcpyft(src, len_delim, i), delim) == 0) {
            if(p == i){
                i++;
                p = i + len_delim;
                continue;
            }
            if(!src[i + len_delim]) {
                is_notnullafterthelastcopy = false;
                arr[arr_counter++] = k_strcpyft(src, i-p, p);
                arr[arr_counter]   = NULL;
                break;
            }
            arr[arr_counter++] = k_strcpyft(src, i-p, p);
            p = i + len_delim;

        }
        i++;
        if(i >= j*ARRAY_BLOCK) {
            j++;
            arr = (char **) realloc(arr, j*ARRAY_BLOCK);
        }
    }
    if(is_notnullafterthelastcopy) {
        arr[arr_counter++] = k_strcpyft(src, len_src-p, p);
        arr[arr_counter++] = NULL;
    }
    return arr;
}

///
///     k_strreverse
///
char *k_strreverse(char *src) {
    int i, j;
    char *dst;
    dst = (char *) malloc(strlen(src));
    for(i = strlen(src)-1, j = 0; i >= 0; i--, j++)
        dst[j] = src[i];
    return dst;
}

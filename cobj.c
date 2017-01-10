/**
 *  File: cdict.c
 *  Project: KProject KAPI for C.
 *  Author: Nick Lauri
 *
 *  Copyright (c) 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <kapi.h>

#define ARRAY_BLOCK   10
#define ELEMENT_BLOCK 10

///
///     kobj_init
///
obj_t *kobj_init() {
    obj_t *o;
    if((o = (obj_t *) malloc(sizeof(o))) == NULL) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: kobj_init: unable to allocate memory to init kobj.\n");
#endif
        return NULL;
    }

    o->len  = 0;
    o->type = (int *) malloc(ARRAY_BLOCK * sizeof(o->type));
    o->val  = malloc(ARRAY_BLOCK * sizeof(o->val));
    if(o->type==NULL || o->val==NULL) {
#ifdef _DEBUG_
        fprintf(stderr, "kapi: kobj_init: unable to allocate memory.\n");
        if(!o->type)
            fprintf(stderr, "kapi: kobj_init: o->type is NULL.\n");
        if(!o->val)
            fprintf(stderr, "kapi: kobj_init: o->val is NULL.\n");
#endif
        o = NULL;
        return NULL;
    }
    o->type[0]  = D_NULL;
    o->val[0].v = NULL;
    return o;
}

///
///     kobj_insert
///
int kobj_insert(obj_t *o, size_t index, int type, void *value) {
    int i;
    if(index > o->len) {
#ifdef _DEBUG_
        fprintf(stderr,
            "kapi: kobj_ins: (index >= o->len) insert into the last of object.\n");
#endif
        index = o->len;
    }

    if(o->len % ARRAY_BLOCK == 0 && o->len > 0) {
        o->type = (int *) realloc(o->type, (o->len + ARRAY_BLOCK)*sizeof(o->type));
        o->val  = realloc(o->val, (o->len + ARRAY_BLOCK)*sizeof(o->val));
        if(o->type==NULL || o->val==NULL) {
#ifdef _DEBUG_
            fprintf(stderr, "kapi: %s: unable to reallocate memory.\n", __func__);
            if(!o->type)
                fprintf(stderr, "kapi: %s: o->type is NULL.\n", __func__);
            if(!o->val)
                fprintf(stderr, "kapi: %s: o->val is NULL.\n", __func__);
#endif
            return 1;
        }
    }

    for(i = o->len; i > index; i--) {
        o->type[i]  = o->type[i-1];
        o->val[i]   = o->val[i-1];
    }

    o->len++;
    o->type[index]  = type;
    o->val[index].v = value;
    return 0;
}

///
///     kobj_append
///
int inline kobj_append(obj_t *o, int t, void *v) {
    return kobj_insert(o, o->len, t, v);
}

///
///     kobj_delete
///
int kobj_delete(obj_t *o, size_t index) {
    int i;
    for(i = index; i < o->len - 1; i++) {
        o->type[i] = o->type[i+1];
        o->val[i]  = o->val[i+1];
    }
    o->type[o->len-1]  = D_NULL;
    o->val[o->len-1].v = NULL;
    o->len--;
    return 0;
}

///
///     kobj_free
///
int kobj_free(obj_t *o) {
    free(o->type);
    free(o->val);
    free(o);
    return 0;
}


///
///     kobj_print
///
bool kobj_print(obj_t *o) {
    char char_mid[] = "├──", char_last[] = "└──", char_line[] = "│  ";
    int i;
    printf("object <0x%08x>\n", o);
    for(i = 0; i < o->len - 1; i++) {
        printf("%s [%d]type: %8s at 0x%08x", char_mid, i, stype[o->type[i]], 
            &(o->val[i]));
        if(o->type[i] == D_NULL)
            continue;
        else if(o->type[i] == D_STRING)
            printf(" ─ len: %d\n%s      value: '%s'\n",
                strlen(o->val[i].v), char_line, o->val[i].v);
        else if(o->type[i] == D_INTEGER || o->type[i] == D_SHORT ||
            o->type[i] == D_BYTE || o->type[i] == D_LONG)
            printf("\n%s   value: %ld\n", char_line, o->val[i].l);
        else if(o->type[i] == D_FLOAT)
            printf("\n%s   value: %f\n", char_line, o->val[i].f);
        else if(o->type[i] == D_FUNCTION)
            printf("\n");
    }

    i = o->len - 1;
    printf("%s [%d]type: %8s at 0x%08x", char_last, i, stype[o->type[i]], 
        &(o->val[i]));
    if(o->type[i] == D_NULL)
        NULL;
    else if(o->type[i] == D_STRING)
        printf(" ─ len: %d\n      value: '%s'\n",
            strlen(o->val[i].v), o->val[i].v);
    else if(o->type[i] == D_INTEGER || o->type[i] == D_SHORT ||
        o->type[i] == D_BYTE || o->type[i] == D_LONG)
        printf("\n   value: %ld\n", o->val[i].l);
    else if(o->type[i] == D_FLOAT)
        printf("\n    value: %f\n", o->val[i].f);
    else if(o->type[i] == D_FUNCTION)
        NULL;
    return true;
}

//
// Created by 李熠 on 2019/12/7.
//

#ifndef DATASTRUCT_LISTGENERICFN_H
#define DATASTRUCT_LISTGENERICFN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum BOOL {
    FALSE, TRUE
} BOOL;

/**
 *  int 当前元素下标
 *  void * 当前数据元素的地址
 *  char* 将输出的字符串写入到该地址
 */
typedef void (*ToStringFn)(int, void *, char *);

typedef void *(*CreateElemFn)(void *);

typedef void (*FreeElemFn)(void *);

typedef BOOL (*ElemEqualsFn)(void *, void *);

typedef struct ListGenericFn {
    ToStringFn toStringFn;

    CreateElemFn createElemFn;

    FreeElemFn freeElemFn;

    ElemEqualsFn equalsFn;

} ListGenericFn;

enum BOOL strEquals(void *, void *);

void printStr(int index, void *string, char *target);

void *createStr(void *);

enum BOOL defaultEquals(void *one, void *two);

#endif //DATASTRUCT_LISTGENERICFN_H

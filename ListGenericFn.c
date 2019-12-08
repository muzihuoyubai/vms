//
// Created by 李熠 on 2019/12/7.
//

#include "ListGenericFn.h"

enum BOOL strEquals(void *one, void *two) {
    if (one != NULL && two != NULL) {
        return strcmp(one, two) == 0;
    } else if (one == NULL && two == NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

enum BOOL defaultEquals(void *one, void *two) {
    return one == two;
}


void printStr(int index, void *string, char *target) {
    sprintf(target, "[%d]=%s", index, string);
}

void *createStr(void *string) {
    void *rlt = malloc(sizeof(char) * (strlen((char *) string) + 1));
    memcpy(rlt, string, (strlen((char *) string) + 1));
    return rlt;
}
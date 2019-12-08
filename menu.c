//
// Created by HYB on 2019/12/8.
//

#include "menu.h"

MenuNode *findMenuByOpt(MenuNode *parentNode, char *opt);

void clearUserInput(MENU *menu) {
    menu->userLastInput[0] = '\0';
}

void printMenuDesc(int index, void *elem, char *target) {
    strcpy(target, ((MenuNode *) elem)->menuDesc);
}

void *createMenuNode(void *targetElem) {
    MenuNode *node = calloc(1, sizeof(MenuNode));
    memcpy(node, targetElem, sizeof(MenuNode));
    return node;
}

void freeMenuNode(void *elem) {
    MenuNode *menuNode = (MenuNode *) elem;
    listDestroy(menuNode->childrenNode);
    free(menuNode->parentNode);
    free(menuNode);
}

BOOL menuNodeEquals(void *one, void *two) {
    return strcmp(((MenuNode *) one)->menuId, ((MenuNode *) two)->menuId) == 0;
}

MENU *createMenu() {
    MENU *rlt = calloc(1, sizeof(MENU));
    rlt->curMenu = calloc(1, sizeof(MenuNode));
    strcpy(rlt->curMenu->menuId, MENU_ROOT_ID);
    rlt->curMenu->menuLevel = 0;
    return rlt;
}

BOOL menuGoBack(MENU *menu) {
    if (strcmp(menu->curMenu->menuId, MENU_ROOT_ID) == 0) {
        return FALSE;
    } else {
        menu->curMenu = menu->curMenu->parentNode;
        return TRUE;
    }
}

BOOL menuGoNext(MENU *menu) {
    MenuNode *node = findMenuByOpt(menu->curMenu, menu->userLastInput);
    if (node != NULL) {
        menu->curMenu = node;
        return TRUE;
    }
    return FALSE;
}

MenuNode *findMenuByOpt(MenuNode *parentNode, char *opt) {
    List *children = parentNode->childrenNode;
    if (children == NULL || opt == NULL) {
        return NULL;
    }
    MenuNode *temp = NULL;
    for (int i = 0; i < listLength(children); ++i) {
        temp = (MenuNode *) listGet(children, i);
        if (strcmp(opt, temp->menuOpt) == 0) {
            return temp;
        }
    }
    return NULL;
}

BOOL menuScanUserInput(MENU *menu) {
    scanf("%s", menu->userLastInput);
    if (findMenuByOpt(menu->curMenu, menu->userLastInput) == NULL) {
        printf("Invalid choice!\n");
        return menuScanUserInput(menu);
    } else {
        return TRUE;
    }
}

void menuDisplay(MENU *menu) {
    listDisplay(menu->curMenu->childrenNode);
    printf("Your choice:\n");
}

MenuNode *findMenuNodeById(MenuNode *node, char *menuId) {
    if (strcmp(node->menuId, menuId) == 0) {
        return node;
    }
    if (node->childrenNode != NULL) {
        for (int i = 0; i < listLength(node->childrenNode); ++i) {
            MenuNode *childNode = findMenuNodeById(listGet(node->childrenNode, i), menuId);
            if (childNode != NULL) {
                return childNode;
            }
        }
    }
    return NULL;
}

MenuNode *addChildMenu(MENU *menu, char *parentMenuId, MenuNode target) {
    MenuNode *parentNode = findMenuNodeById(menu->curMenu, parentMenuId);

    if (parentNode != NULL) {
        MenuNode *childNode = createMenuNode(&target);
        if (parentNode->childrenNode == NULL) {
            ListGenericFn listGenericFn = {printMenuDesc, createMenuNode, freeMenuNode, menuNodeEquals};
            parentNode->childrenNode = listNew(STRUCT, sizeof(MenuNode), &listGenericFn);
        }
        // append会重新分配空间，list加上去的是新的地址，内容是原始内容的复制，因此操作传入的指针修改内容就失效了
        childNode = listAppend(parentNode->childrenNode, childNode);
        childNode->parentNode = parentNode;
        return childNode;
    }

    return NULL;
}


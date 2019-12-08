//
// 菜单的封装
//

#include "List.h"

#ifndef VMS_MENU_H
#define VMS_MENU_H

// 菜单中一项的描述
#define MENU_DESC_LEN 200
// 菜单唯一标识
#define MENU_ID_LEN 32
// 菜单和用户输入匹配的选项
#define MENU_OPT_LEN 200
// 保存用户的屏幕输入
#define USER_LAST_INPUT_LEN 200
// 菜单的根路径
#define MENU_ROOT_ID "_MENU_ROOT_ID"

typedef struct MenuNode {
    char menuDesc[MENU_DESC_LEN];
    char menuId[MENU_ID_LEN];
    char menuOpt[MENU_OPT_LEN];
    int menuLevel;
    struct MenuNode *parentNode;
    List *childrenNode;
} MenuNode;

typedef struct MENU {
    MenuNode *curMenu;
    char userLastInput[USER_LAST_INPUT_LEN];
} MENU;

void clearUserInput(MENU *menu);

MENU *createMenu();

BOOL menuGoBack(MENU *menu);

BOOL menuGoNext(MENU *menu);

BOOL menuScanUserInput(MENU *menu);

void menuDisplay(MENU *menu);

MenuNode *addChildMenu(MENU *menu, char *parentMenuId, MenuNode target);

#endif //VMS_MENU_H

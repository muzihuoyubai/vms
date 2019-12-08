#include <stdio.h>
#include "menu.h"

#define MACHINE_PROD_ENABLE 'O'
#define MACHINE_PROD_OUT 'X'
#define MACHINE_PROD_DEFAULT ' '
#define PROD_LIST_SIZE 5
#define PROD_REFILE_STOCK 10

static char PROD_SELL_ICON = '=';

/**
 * 一个用户投入的硬币总金额
 */
static int INSERT_COIN_TOTAL = 0;

/**
 * 机器里面的总金额
 */
static int REVENUE_TOTAL = 0;

static int ACCESS_CODE = 1110;

static BOOL permitted = FALSE;

enum MENU_LEVEL {
    ROOT, INSERT_ICON, PRESS_PROD_BTN, PRESS_RETURN_BTN, SERVICE_MENU_LEVEL, SERVICE_REFILL_PRODUCT,
    SERVICE_CHANGE_PRODUCT
};

typedef struct Product {
    char name[20];
    int price;
    int stock;
} Product;

struct MenuNode ROOT_MENU[] = {
        {" 1. Read product information\n",          "1_1rdi", "1", ROOT,               NULL, NULL},
        {" 2. Insert coin\n",                       "1_2ic",  "2", INSERT_ICON,        NULL, NULL},
        {" 3. Press product button\n",              "1_3ppb", "3", PRESS_PROD_BTN,     NULL, NULL},
        {" 4. Press return button\n",               "1_4prb", "4", PRESS_RETURN_BTN,   NULL, NULL},
        {" 9. Open service menu (code required)\n", "1_9osm", "9", SERVICE_MENU_LEVEL, NULL, NULL},
        {" 0. Quit\n",                              "1_0q",   "0", ROOT,               NULL, NULL},
};

struct MenuNode INSERT_ICON_MENU[] = {
        {" 1. $1\n",      "2_11",  "1", INSERT_ICON, NULL, NULL},
        {" 2. $2\n",      "2_22",  "2", INSERT_ICON, NULL, NULL},
        {" 3. $5\n",      "2_35",  "3", INSERT_ICON, NULL, NULL},
        {" 4. $10\n",     "2_410", "4", INSERT_ICON, NULL, NULL},
        {" 0. Go back\n", "2_0gb", "0", INSERT_ICON, NULL, NULL},
};

struct MenuNode PRESS_PROD_BTN_MENU[] = {
        {" 1. A\n",       "3_1A",  "1", PRESS_PROD_BTN, NULL, NULL},
        {" 2. B\n",       "3_2B",  "2", PRESS_PROD_BTN, NULL, NULL},
        {" 3. C\n",       "3_3C",  "3", PRESS_PROD_BTN, NULL, NULL},
        {" 4. D\n",       "3_4D",  "4", PRESS_PROD_BTN, NULL, NULL},
        {" 5. E\n",       "3_4E",  "5", PRESS_PROD_BTN, NULL, NULL},
        {" 0. Go back\n", "3_0gb", "0", PRESS_PROD_BTN, NULL, NULL},
};

struct MenuNode SERVICE_MENU[] = {
        {" 1. Inspect machine status\n", "9_1ima", "1", SERVICE_MENU_LEVEL,     NULL, NULL},
        {" 2. Withdraw all money\n",     "9_2wam", "2", SERVICE_MENU_LEVEL,     NULL, NULL},
        {" 3. Refill product\n",         "9_3rp",  "3", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 4. Change product\n",         "9_4cp",  "4", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 0. Go back\n",                "9_0gb",  "0", SERVICE_MENU_LEVEL,     NULL, NULL},
};

struct MenuNode SERVICE_REFILE_MENU[] = {
        {" 1. A\n",       "3_1A",  "1", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 2. B\n",       "3_2B",  "2", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 3. C\n",       "3_3C",  "3", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 4. D\n",       "3_4D",  "4", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 5. E\n",       "3_4E",  "5", SERVICE_REFILL_PRODUCT, NULL, NULL},
        {" 0. Go back\n", "3_0gb", "0", SERVICE_REFILL_PRODUCT, NULL, NULL},
};

struct MenuNode SERVICE_CHANGE_MENU[] = {
        {" 1. A\n",       "3_1A",  "1", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 2. B\n",       "3_2B",  "2", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 3. C\n",       "3_3C",  "3", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 4. D\n",       "3_4D",  "4", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 5. E\n",       "3_4E",  "5", SERVICE_CHANGE_PRODUCT, NULL, NULL},
        {" 0. Go back\n", "3_0gb", "0", SERVICE_CHANGE_PRODUCT, NULL, NULL},
};


struct Product PROD_LIST[] = {
        {"Juice",  10, 5},
        {"Cola",   6,  1},
        {"Tea",    5,  2},
        {"Water",  8,  1},
        {"Coffee", 7,  9}
};

void displayInsertIcon(MENU *menu);


void printMachineStatus();

void withDrawMoney(MENU *menu);

void displayVending() {

    printf(
            "*---------------------------*\n"
            "|     Vending   Machine     |\n"
            "*---------------------------*\n"
    );

    printf("|");
    for (int i = 0; i < PROD_LIST_SIZE; i++) {
        if (i == 0) {
            printf("%4c", 'A' + i);
        } else {
            printf("%5c", 'A' + i);
        }
    }
    printf("   |\n");

    printf("|");
    for (int i = 0; i < PROD_LIST_SIZE; i++) {
        printf("  $%2d", PROD_LIST[i].price);
    }
    printf("  |\n");

    printf("|");
    char prodFlag;
    for (int i = 0; i < PROD_LIST_SIZE; i++) {
        prodFlag = MACHINE_PROD_DEFAULT;
        if (PROD_LIST[i].stock == 0) {
            prodFlag = MACHINE_PROD_OUT;
        } else if (PROD_LIST[i].price <= INSERT_COIN_TOTAL) {
            prodFlag = MACHINE_PROD_ENABLE;
        }
        printf("  [%c]", prodFlag);
    }
    printf("  |\n");
    printf("*---------------------------*\n");
    printf("|                    [$%2d]  |\n", INSERT_COIN_TOTAL);
    printf("|                           |\n");
    printf("|           [=%c=]           |\n", PROD_SELL_ICON);
    printf("*---------------------------*\n");
    printf("\n");
}

void printProductList(char *menuIndex) {
    printf("(%s) The displayed products are:\n", menuIndex);
    for (int i = 0; i < PROD_LIST_SIZE; ++i) {
        printf("%c. %s ($%d)\n", 'A' + i, PROD_LIST[i].name, PROD_LIST[i].price);
    }
}


int main() {

    displayVending();
    MENU *menu = createMenu();
    for (int i = 0; i < sizeof(ROOT_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, MENU_ROOT_ID, ROOT_MENU[i]);
    }

    for (int i = 0; i < sizeof(INSERT_ICON_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, ROOT_MENU[1].menuId, INSERT_ICON_MENU[i]);
    }

    for (int i = 0; i < sizeof(PRESS_PROD_BTN_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, ROOT_MENU[2].menuId, PRESS_PROD_BTN_MENU[i]);
    }

    for (int i = 0; i < sizeof(SERVICE_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, ROOT_MENU[4].menuId, SERVICE_MENU[i]);
    }

    for (int i = 0; i < sizeof(SERVICE_REFILE_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, SERVICE_MENU[2].menuId, SERVICE_REFILE_MENU[i]);
    }

    for (int i = 0; i < sizeof(SERVICE_CHANGE_MENU) / sizeof(struct MenuNode); ++i) {
        addChildMenu(menu, SERVICE_MENU[3].menuId, SERVICE_CHANGE_MENU[i]);
    }

    while (1) {
        switch (menu->curMenu->menuLevel) {
            case ROOT:
                printf("What would you like to do?\n");
                menuDisplay(menu);
                if (!menuScanUserInput(menu)) {
                    break;
                }
                if (strcmp(menu->userLastInput, ROOT_MENU[0].menuOpt) == 0) {
                    printProductList(ROOT_MENU[0].menuOpt);
                    clearUserInput(menu);
                    break;
                }

                if (strcmp(menu->userLastInput, ROOT_MENU[1].menuOpt) == 0 ||
                    strcmp(menu->userLastInput, ROOT_MENU[2].menuOpt) == 0 ||
                    strcmp(menu->userLastInput, ROOT_MENU[3].menuOpt) == 0 ||
                    strcmp(menu->userLastInput, ROOT_MENU[4].menuOpt) == 0) {
                    menuGoNext(menu);
                    break;
                }

                if (strcmp(menu->userLastInput, ROOT_MENU[5].menuOpt) == 0) {
                    return 0;
                }
                break;
            case INSERT_ICON:
                printf("(%s) Which coin would you like to insert?\n", menu->curMenu->menuOpt);
                menuDisplay(menu);
                if (!menuScanUserInput(menu)) {
                    break;
                }
                if (strcmp(menu->userLastInput, INSERT_ICON_MENU[4].menuOpt) == 0) {
                    menuGoBack(menu);
                    clearUserInput(menu);
                    displayVending();
                    break;
                }
                if (strcmp(menu->userLastInput, INSERT_ICON_MENU[0].menuOpt) == 0 ||
                    strcmp(menu->userLastInput, INSERT_ICON_MENU[1].menuOpt) == 0) {
                    INSERT_COIN_TOTAL += strtol(menu->userLastInput, NULL, 10);
                    displayInsertIcon(menu);
                    break;
                }

                if (strcmp(menu->userLastInput, INSERT_ICON_MENU[2].menuOpt) == 0) {
                    INSERT_COIN_TOTAL += 5;
                    displayInsertIcon(menu);
                    break;
                }

                if (strcmp(menu->userLastInput, INSERT_ICON_MENU[3].menuOpt) == 0) {
                    INSERT_COIN_TOTAL += 10;
                    displayInsertIcon(menu);
                    break;
                }

            case PRESS_PROD_BTN:
                displayVending();
                printf("(%s) Which product button would you like to press?\n", menu->curMenu->menuOpt);
                menuDisplay(menu);
                if (!menuScanUserInput(menu)) {
                    break;
                }
                if (strcmp(menu->userLastInput, PRESS_PROD_BTN_MENU[5].menuOpt) == 0) {
                    menuGoBack(menu);
                    PROD_SELL_ICON = '=';
                    clearUserInput(menu);
                    displayVending();
                    break;
                }
                int prodIndex = strtol(menu->userLastInput, NULL, 10) - 1;
                printf("You have pressed button %c.\n", 'A' + prodIndex);

                if (PROD_LIST[prodIndex].price <= INSERT_COIN_TOTAL && PROD_LIST[prodIndex].stock > 0) {
                    PROD_SELL_ICON = 'A' + prodIndex;
                    INSERT_COIN_TOTAL -= PROD_LIST[prodIndex].price;
                    PROD_LIST[prodIndex].stock--;
                    REVENUE_TOTAL += PROD_LIST[prodIndex].price;
                    displayVending();
                    clearUserInput(menu);

                } else {
                    printf("invalid choice\n");
                    clearUserInput(menu);
                }
                break;

            case PRESS_RETURN_BTN:
                printf("(%s) Return button is pressed.\n", menu->curMenu->menuOpt);
                printf("$%d has been returned.\n", INSERT_COIN_TOTAL);
                INSERT_COIN_TOTAL = 0;
                clearUserInput(menu);
                displayVending();
                menuGoBack(menu);
                break;

            case SERVICE_MENU_LEVEL:
                if (!permitted) {
                    printf(" (%s) Opening service menu. Access code is required.\n", menu->curMenu->menuOpt);
                    printf("Enter access code: ");
                    scanf("%s", menu->userLastInput);

                    if (strtol(menu->userLastInput, NULL, 10) == ACCESS_CODE) {
                        permitted = TRUE;
                        printf("Correct code!\n");
                    } else {
                        printf("Incorrect code!\n");
                        menuGoBack(menu);
                        displayVending();
                    }
                } else {
                    printf("(%s) What would you like to do?\n", menu->curMenu->menuOpt);
                    menuDisplay(menu);
                    if (!menuScanUserInput(menu)) {
                        break;
                    }

                    if (strcmp(menu->userLastInput, SERVICE_MENU[0].menuOpt) == 0) {
                        printMachineStatus(menu);
                    } else if (strcmp(menu->userLastInput, SERVICE_MENU[1].menuOpt) == 0) {
                        withDrawMoney(menu);
                    } else if (strcmp(menu->userLastInput, SERVICE_MENU[4].menuOpt) == 0) {
                        menuGoBack(menu);
                        permitted = FALSE;
                    } else if (strcmp(menu->userLastInput, SERVICE_MENU[2].menuOpt) == 0 ||
                               strcmp(menu->userLastInput, SERVICE_MENU[3].menuOpt) == 0) {
                        menuGoNext(menu);
                    }
                }
                clearUserInput(menu);

                break;

            case SERVICE_REFILL_PRODUCT:
                printf("(%s-%s) Which product would you like to refill?\n", menu->curMenu->menuOpt,
                       SERVICE_MENU[2].menuOpt);
                menuDisplay(menu);
                if (!menuScanUserInput(menu)) {
                    break;
                }

                if (strcmp(menu->userLastInput, SERVICE_REFILE_MENU[5].menuOpt) == 0) {
                    menuGoBack(menu);
                } else {
                    int productIndex = (int) strtol(menu->userLastInput, NULL, 10) - 1;
                    printf("You have refilled product %c to full.\n",
                           'A' + productIndex);
                    PROD_LIST[productIndex].stock = PROD_REFILE_STOCK;
                    menuGoBack(menu);
                }
                break;

            case SERVICE_CHANGE_PRODUCT:
                printf("(%s-%s) Which product would you like to change?\n", menu->curMenu->menuOpt,
                       SERVICE_MENU[3].menuOpt);
                menuDisplay(menu);
                if (!menuScanUserInput(menu)) {
                    break;
                }

                if (strcmp(menu->userLastInput, SERVICE_REFILE_MENU[5].menuOpt) == 0) {
                    menuGoBack(menu);
                } else {
                    int productIndex = (int) strtol(menu->userLastInput, NULL, 10) - 1;
                    printf("You are changing product %c.\n",
                           'A' + productIndex);
                    printf("Enter new product name:");
                    scanf("%s", PROD_LIST[productIndex].name);

                    char productPrice[20];
                    printf("Enter new product price:");
                    scanf("%s", productPrice);

                    PROD_LIST[productIndex].price = (int) strtol(productPrice, NULL, 10);
                    PROD_LIST[productIndex].stock = PROD_REFILE_STOCK;
                    printf("The new product %c has been filled to full.", 'A' + productIndex);
                    menuGoBack(menu);
                }
                break;

            default:
                break;
        }

    }
}

void withDrawMoney(MENU *menu) {
    printf("(%s-%s) All money is being withdrawn.\n", menu->curMenu->menuOpt, menu->userLastInput);
    printf("$%d is withdrawn.\n", REVENUE_TOTAL);
    REVENUE_TOTAL = 0;
}

void printMachineStatus(MENU *menu) {
    printf("(%s-%s) Machine status\n", menu->curMenu->menuOpt, menu->userLastInput);
    printf("Amount of revenue: $%d\n", REVENUE_TOTAL);
    printf("Amount of inserted coins: $%d\n", INSERT_COIN_TOTAL);
    for (int i = 0; i < PROD_LIST_SIZE; ++i) {
        if (PROD_LIST[i].stock == 0) {
            printf("%c. %s ($%d) (sold out)\n", 'A' + i, PROD_LIST[i].name, PROD_LIST[i].price);
        } else {
            printf("%c. %s ($%d) (%d left)\n", 'A' + i, PROD_LIST[i].name, PROD_LIST[i].price,
                   PROD_LIST[i].stock);
        }
    }
    printf("\n");
}

void displayInsertIcon(MENU *menu) {
    printf("You have inserted $%d.", INSERT_COIN_TOTAL);
    clearUserInput(menu);
    displayVending();
}

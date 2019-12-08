
#include "List.h"

void freeListNode(const List *list, ListNode *curNode);

void removeNextNode(const List *list, ListNode *node);

void loadFn(enum ListNodeType type, List *list, ListGenericFn *listGenericFn);

ListNode *createListNode() {
    ListNode *rlt = malloc(sizeof(ListNode));
    rlt->next = NULL;
    rlt->value = NULL;
    return rlt;
}

/**
 * list->head 节点用于保存链表有效数据的长度，保存的第一个数据位于head->next
 * @param type
 * @param listGenericFn
 * @return
 */
List *listNew(enum ListNodeType type, int elemSize, ListGenericFn *listGenericFn) {
    List *rlt = (List *) calloc(1, sizeof(List));
    rlt->head = (ListNode *) calloc(1, sizeof(ListNode));
    rlt->head->value = malloc(sizeof(int));
    rlt->tail = rlt->head;
    rlt->elemSize = elemSize;
    *((int *) rlt->head->value) = 0;
    loadFn(type, rlt, listGenericFn);
    return rlt;
}

/**
 * 根据类型提供默认的方法，如果传入的方法集合不是NULL，则使用其中的非NULL方法覆盖默认方法
 * @param type
 * @param list
 * @param listGenericFn
 */
void loadFn(enum ListNodeType type, List *list, ListGenericFn *listGenericFn) {
    if (listGenericFn != NULL) {
        list->listGenericFn = *listGenericFn;
    }

    switch (type) {
        case STRING:
            if (list->listGenericFn.toStringFn == NULL) {
                list->listGenericFn.toStringFn = printStr;
            }
            if (list->listGenericFn.createElemFn == NULL) {
                list->listGenericFn.createElemFn = createStr;
            }
            if (list->listGenericFn.equalsFn == NULL) {
                list->listGenericFn.equalsFn = strEquals;
            }
            break;
        default:
            if (list->listGenericFn.equalsFn == NULL) {
                list->listGenericFn.equalsFn = defaultEquals;
            }
            break;
    }
}

void listDestroy(List *list) {
    ListNode *curNode = list->head->next;
    ListNode *temp = NULL;
    while (curNode != NULL) {
        temp = curNode->next;
        freeListNode(list, curNode);
        curNode = temp;
    }


    free(list->head->value);
    free(list->head);
    free(list);
}

void freeListNode(const List *list, ListNode *curNode) {
    if (list->listGenericFn.freeElemFn != NULL) {
        (*list->listGenericFn.freeElemFn)(curNode->value);
    } else {
        free(curNode->value);
    }
    free(curNode);
}

void *listAppend(List *list, void *elem) {
    if (elem == NULL) {
        printf("elem is NULL");
        return NULL;
    }
    ListNode *newNode = createListNode();
    void *newElem = NULL;
    if (list->listGenericFn.createElemFn != NULL) {
        newElem = (*list->listGenericFn.createElemFn)(elem);
    } else {
        newElem = calloc(1, list->elemSize);
        memcpy(newElem, elem, list->elemSize);
    }
    newNode->value = newElem;
    list->tail->next = newNode;
    list->tail = newNode;
    (*((int *) list->head->value))++;
    return newNode->value;
}

int listLength(List *list) {
    return (*((int *) list->head->value));
}

void listDisplay(const List *list) {
    ListNode *curNode = list->head->next;
    int index = 0;
    char target[LIST_DISPLAY_BUF_SIZE] = {0};
    while (curNode != NULL) {
        if (list->listGenericFn.toStringFn != NULL) {
            (*list->listGenericFn.toStringFn)(index++, curNode->value, target);
            printf("%s", target);
        }
        curNode = curNode->next;
    }
    printf("\n");
}

int listDelete(List *list, int index) {
    if (index > listLength(list) - 1 || index < 0) {
        printf("index 不合法");
        return 0;
    }

    ListNode *node = list->head;
    for (int i = 0; i < index; i++) {
        node = node->next;
    }

    ListNode *temp = node->next->next;
    freeListNode(list, node->next);
    node->next = temp;
    (*((int *) list->head->value))--;
    return 1;
}

/**
 * 删除节点，但是不释放该节点数据，将该节点数据返回
 * @param list
 * @param index
 * @return
 */
void *listDeleteWithNoFree(List *list, int index) {
    if (index > listLength(list) - 1 || index < 0) {
        printf("index 不合法");
        return 0;
    }

    ListNode *node = list->head;
    for (int i = 0; i < index; i++) {
        node = node->next;
    }

    ListNode *temp = node->next->next;
    void *rlt = node->next->value;
    free(node->next);
    node->next = temp;
    (*((int *) list->head->value))--;
    return rlt;
}

int listDeleteElem(List *list, void *elem) {
    ListNode *node = list->head;

    while (node->next != NULL) {
        if (list->listGenericFn.equalsFn != NULL) {
            if ((*list->listGenericFn.equalsFn)(node->next->value, elem)) {
                removeNextNode(list, node);
                return 1;
            }
        } else {
            if (node->next->value == elem) {
                removeNextNode(list, node);
                return 1;
            }
        }
        node = node->next;
    }
    return 0;
}

void removeNextNode(const List *list, ListNode *node) {
    ListNode *temp = node->next->next;
    freeListNode(list, node->next);
    node->next = temp;
    (*((int *) list->head->value))--;
}

void *listGet(List *list, int index) {
    if (index > listLength(list) - 1 || index < 0) {
        printf("index 不合法");
        return 0;
    }
    ListNode *node = list->head->next;
    for (int i = 0; i < index; ++i) {
        node = node->next;
    }

    return node->value;

}

int listGetIndex(List *list, void *elem) {
    ListNode *curNode = list->head->next;
    for (int i = 0; i < listLength(list); ++i) {
        if ((*list->listGenericFn.equalsFn)(curNode->value, elem)) {
            return i;
        }
        curNode = curNode->next;
    }
    return -1;
}

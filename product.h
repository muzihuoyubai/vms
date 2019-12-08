//
// Created by HYB on 2019/12/8.
//

#ifndef VMS_PRODUCT_H
#define VMS_PRODUCT_H

typedef struct Product {
    char name[20];
    int price;
    int stock;
} Product;

void *createProduct(void *elem);

#endif //VMS_PRODUCT_H

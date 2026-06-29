#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;

    Product();
    Product(int id, string name, double price, int quantity);
};

#endif
#include "Product.h"

Product::Product() {
    id = 0;
    name = "";
    price = 0;
    quantity = 0;
}

Product::Product(int id, string name, double price, int quantity) {
    this->id = id;
    this->name = name;
    this->price = price;
    this->quantity = quantity;
}
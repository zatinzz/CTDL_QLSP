#include "Product.h"

//  Product
Product::Product(int id, const std::string& name, double price,
    int qty, const std::string& desc,
    const std::string& catName)
    : id(id), name(name), price(price), quantity(qty),
    description(desc), categoryName(catName), next(nullptr) {
}


//  ProductList
ProductList::ProductList() : head(nullptr), size(0) {}

ProductList::~ProductList() { clear(); }

void ProductList::push(Product* p) {
    p->next = head;
    head = p;
    ++size;
}

bool ProductList::remove(int productId) {
    Product* prev = nullptr;
    Product* cur = head;
    while (cur) {
        if (cur->id == productId) {
            if (prev) prev->next = cur->next;
            else       head = cur->next;
            delete cur;
            --size;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

Product* ProductList::find(int productId) const {
    for (Product* p = head; p; p = p->next)
        if (p->id == productId) return p;
    return nullptr;
}

void ProductList::clear() {
    while (head) {
        Product* tmp = head->next;
        delete head;
        head = tmp;
    }
    size = 0;
}


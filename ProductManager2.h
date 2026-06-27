#pragma once
#include "CategoryNode.h"
#include "Algorithm.h"
#include <string>

class ProductManager {
public:
    explicit ProductManager(CategoryTree& tree);

    void showMenu();

private:
    CategoryTree& tree_;

    void addProduct();
    void editProduct();
    void deleteProduct();
    void listProducts();      // SP trong 1 danh mục
    void sortProducts();
    void searchProduct();
    void searchByPrice();

    // helpers
    void printProductHeader();
    void printProduct(const Product* p, int no = -1);
    int  chooseSortField();
    int  chooseSortOrder();
    int  selectCategory(const std::string& prompt);
};

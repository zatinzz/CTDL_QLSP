#pragma once
#include "CategoryNode.h"
#include <string>

class CategoryManager {
public:
    explicit CategoryManager(CategoryTree& tree);

    void showMenu();

private:
    CategoryTree& tree_;

    void addCategory();
    void viewCategory();
    void editCategory();
    void deleteCategory();
    void showPath();
    void showChildren();
    void showProductsInCategory();
    void showAllProductsDown();
    void printTree(CategoryNode* node, int depth = 0);
};
#ifndef CATEGORY_MANAGER_H
#define CATEGORY_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

// === CẤU TRÚC DỮ LIỆU ===

// Cấu trúc Sản phẩm
struct Product {
    int id;
    string name;
    double price;
    int quantity;
    string description;
    string categoryName;

    Product(int _id = 0, string _name = "", double _price = 0,
        int _qty = 0, string _desc = "", string _cat = "")
        : id(_id), name(_name), price(_price), quantity(_qty),
        description(_desc), categoryName(_cat) {
    }

    void display() const {
        cout << "  ID: " << id << " | Tên: " << name
            << " | Giá: " << fixed << setprecision(0) << price << "đ"
            << " | SL: " << quantity;
        if (!description.empty()) {
            cout << " | Mô tả: " << description;
        }
        cout << "\n";
    }
};

// Cấu trúc Node Danh mục (Tree)
struct CategoryNode {
    int id;
    string name;
    CategoryNode* parent;
    vector<CategoryNode*> children;
    vector<Product> products;

    CategoryNode(int _id = 0, string _name = "")
        : id(_id), name(_name), parent(nullptr) {
    }

    bool isEmpty() const {
        return products.empty() && children.empty();
    }
};

// === LỚP QUẢN LÝ CHÍNH ===

class CategoryManager {
private:
    CategoryNode* root;
    int nextCategoryId;
    int nextProductId;
    vector<CategoryNode*> allNodes; // Dùng để quản lý bộ nhớ

    // Hash tables để tìm kiếm O(1)
    unordered_map<int, CategoryNode*> categoryMap;
    unordered_map<int, Product*> productMap;

    // Helper Functions
    CategoryNode* findCategoryRecursive(CategoryNode* node, int id);
    void displayCategoryTree(CategoryNode* node, int level, bool showProducts);
    void collectAllProducts(CategoryNode* node, vector<Product>& result);
    void clearTree(CategoryNode* node);
    void mergeSortProducts(vector<Product>& arr, int left, int right);
    void mergeProducts(vector<Product>& arr, int left, int mid, int right);

public:
    CategoryManager();
    ~CategoryManager();

    // === QUẢN LÝ DANH MỤC ===
    bool createCategory(int parentId, const string& name);
    void viewAllCategories(bool showProducts = false);
    bool editCategory(int id, const string& newName);
    bool deleteCategory(int id);
    string getPath(int id);
    void viewSubCategories(int id);
    CategoryNode* findCategory(int id); // O(1) với hash table

    // === QUẢN LÝ SẢN PHẨM ===
    bool addProduct(int categoryId, const string& name, double price,
        int quantity, const string& description = "");
    bool editProduct(int productId, const string& name, double price,
        int quantity, const string& description = "");
    bool deleteProduct(int categoryId, int productId);
    void viewProductsInCategory(int categoryId);
    void viewAllProductsFrom(int categoryId);
    Product* findProduct(int productId); // O(1) với hash table
    void searchProduct(const string& keyword);
    void sortProductsInCategory(int categoryId);

    // === LƯU TRỮ DỮ LIỆU ===
    bool loadFromFile(const string& filename);
    bool saveToFile(const string& filename);

    // === THỐNG KÊ ===
    int countTotalProducts(CategoryNode* node);
    int countTotalCategories();
};

#endif
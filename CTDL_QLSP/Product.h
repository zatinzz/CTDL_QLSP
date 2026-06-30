#pragma once
#include <string>

//  Product: node trong linked-list sản phẩm
struct Product {
    int    id;
    std::string name;
    double price;
    int    quantity;
    std::string description;
    std::string categoryName;   // tên danh mục chứa SP này
    Product* next;              // liên kết linked-list

    Product(); 

    Product(int id, const std::string& name, double price,
        int qty, const std::string& desc,
        const std::string& catName);
};

//  ProductList: danh sách liên kết đơn các sản phẩm
struct ProductList {
    Product* head;
    int      size;

    ProductList();
    ~ProductList();

    void    push(Product* p);          // thêm vào đầu
    bool    remove(int productId);     // xóa theo id
    Product* find(int productId) const;
    void    clear();                   // giải phóng toàn bộ
};
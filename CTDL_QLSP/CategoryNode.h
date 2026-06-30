#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Product.h"


//  CategoryNode: node trong cây danh mục
struct CategoryNode {
    int          id;
    std::string  name;
    CategoryNode* parent;              // cha (nullptr nếu là root)

    // danh sách con – dùng vector con trỏ (linked-list logic qua next)
    CategoryNode* firstChild;          // con đầu tiên
    CategoryNode* nextSibling;         // anh/chị/em kế tiếp (linked-list ngang)
    ProductList  products;             // sản phẩm trực tiếp trong DM này
    CategoryNode(int id, const std::string& name, CategoryNode* parent = nullptr);
    ~CategoryNode();

    void addChild(CategoryNode* child);
    bool removeChild(int childId);
    bool isEmpty() const;    // true nếu không có SP và không có con
    bool isLeaf()  const;    // true nếu không có con
};

//  CategoryTree: cây tổng thể + hash table tra cứu nhanh
class CategoryTree {
public:
    CategoryTree();
    ~CategoryTree();

    CategoryNode* addCategory(int id, const std::string& name, int parentId);
    CategoryNode* findCategory(int id) const;
    CategoryNode* findCategoryByName(const std::string& name) const;
    bool          updateCategory(int id, const std::string& newName);
    bool          deleteCategory(int id);       // chỉ xóa khi rỗng & không có con


    bool addProduct(int catId, Product* p);
    bool removeProduct(int catId, int productId);

    std::vector<std::string>   getPath(int id) const;          // đường dẫn từ root
    std::vector<CategoryNode*> getChildren(int id) const;
    std::vector<Product*>      getProducts(int id) const;      // SP trực tiếp
    std::vector<Product*>      getAllProducts(int id) const;   // SP từ cả cây con

    CategoryNode* getRoot() const { return root_; }
    const std::unordered_map<int, CategoryNode*>& getTable() const { return table_; }

    int  nextCategoryId() const;
    int  nextProductId()  const;

private:
    CategoryNode* root_;
    std::unordered_map<int, CategoryNode*> table_;  // hash table: id -> node

    void collectAllProducts(CategoryNode* node, std::vector<Product*>& out) const;
    void destroyTree(CategoryNode* node);
};
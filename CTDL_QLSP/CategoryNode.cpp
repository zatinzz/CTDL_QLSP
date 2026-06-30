#include "CategoryNode.h"
#include <stdexcept>
#include <algorithm>



//  CategoryNode
CategoryNode::CategoryNode(int id, const std::string& name, CategoryNode* parent)
    : id(id), name(name), parent(parent),
    firstChild(nullptr), nextSibling(nullptr) {
}

CategoryNode::~CategoryNode() {
}

void CategoryNode::addChild(CategoryNode* child) {
    child->parent = this;
    child->nextSibling = firstChild;
    firstChild = child;
}

bool CategoryNode::removeChild(int childId) {
    CategoryNode* prev = nullptr;
    CategoryNode* cur = firstChild;
    while (cur) {
        if (cur->id == childId) {
            if (prev) prev->nextSibling = cur->nextSibling;
            else       firstChild = cur->nextSibling;
            cur->nextSibling = nullptr;
            return true;
        }
        prev = cur;
        cur = cur->nextSibling;
    }
    return false;
}

bool CategoryNode::isEmpty() const {
    return products.size == 0 && firstChild == nullptr;
}

bool CategoryNode::isLeaf() const {
    return firstChild == nullptr;
}

//  CategoryTree
CategoryTree::CategoryTree() : root_(nullptr) {}

CategoryTree::~CategoryTree() {
    destroyTree(root_);
    root_ = nullptr;
}

void CategoryTree::destroyTree(CategoryNode* node) {
    if (!node) return;
    CategoryNode* child = node->firstChild;
    while (child) {
        CategoryNode* sibling = child->nextSibling;
        destroyTree(child);
        child = sibling;
    }
    delete node;
}

CategoryNode* CategoryTree::addCategory(int id, const std::string& name, int parentId) {
    if (table_.count(id)) return nullptr;   // id trùng

    auto* node = new CategoryNode(id, name, nullptr);

    if (parentId == 0) {
        // là root
        if (!root_) root_ = node;
        else {
            // gắn như con của root thực
            root_->addChild(node);
        }
    }
    else {
        auto it = table_.find(parentId);
        if (it == table_.end()) { delete node; return nullptr; }
        it->second->addChild(node);
    }

    table_[id] = node;
    return node;
}

CategoryNode* CategoryTree::findCategory(int id) const {
    auto it = table_.find(id);
    return (it != table_.end()) ? it->second : nullptr;
}

CategoryNode* CategoryTree::findCategoryByName(const std::string& name) const {
    for (auto& [id, node] : table_)
        if (node->name == name) return node;
    return nullptr;
}

bool CategoryTree::updateCategory(int id, const std::string& newName) {
    auto* node = findCategory(id);
    if (!node) return false;
    node->name = newName;
    return true;
}

bool CategoryTree::deleteCategory(int id) {
    auto* node = findCategory(id);
    if (!node) return false;
    if (node == root_) return false;           // không xóa root
    if (!node->isEmpty()) return false;        // có SP hoặc có con

    // tách khỏi cha
    if (node->parent) node->parent->removeChild(id);
    table_.erase(id);
    delete node;
    return true;
}

bool CategoryTree::addProduct(int catId, Product* p) {
    auto* node = findCategory(catId);
    if (!node) return false;
    node->products.push(p);
    return true;
}

bool CategoryTree::removeProduct(int catId, int productId) {
    auto* node = findCategory(catId);
    if (!node) return false;
    return node->products.remove(productId);
}

std::vector<std::string> CategoryTree::getPath(int id) const {
    std::vector<std::string> path;
    auto* node = findCategory(id);
    while (node) {
        path.push_back(node->name);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<CategoryNode*> CategoryTree::getChildren(int id) const {
    std::vector<CategoryNode*> result;
    auto* node = findCategory(id);
    if (!node) return result;
    for (CategoryNode* c = node->firstChild; c; c = c->nextSibling)
        result.push_back(c);
    return result;
}

std::vector<Product*> CategoryTree::getProducts(int id) const {
    std::vector<Product*> result;
    auto* node = findCategory(id);
    if (!node) return result;
    for (Product* p = node->products.head; p; p = p->next)
        result.push_back(p);
    return result;
}

std::vector<Product*> CategoryTree::getAllProducts(int id) const {
    std::vector<Product*> result;
    auto* node = findCategory(id);
    if (!node) return result;
    collectAllProducts(node, result);
    return result;
}

void CategoryTree::collectAllProducts(CategoryNode* node, std::vector<Product*>& out) const {
    for (Product* p = node->products.head; p; p = p->next)
        out.push_back(p);
    for (CategoryNode* c = node->firstChild; c; c = c->nextSibling)
        collectAllProducts(c, out);
}

int CategoryTree::nextCategoryId() const {
    int mx = 0;
    for (auto& [id, _] : table_) mx = std::max(mx, id);
    return mx + 1;
}

int CategoryTree::nextProductId() const {
    int mx = 0;
    for (auto& [catId, node] : table_)
        for (Product* p = node->products.head; p; p = p->next)
            mx = std::max(mx, p->id);
    return mx + 1;
}
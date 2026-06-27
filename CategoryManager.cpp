#include "CategoryManager.h"
#include <unordered_map>

// Constructor
CategoryManager::CategoryManager() {
    root = nullptr;
    nextCategoryId = 1;
    nextProductId = 1;
}

// Destructor
CategoryManager::~CategoryManager() {
    clearTree(root);
}

// === HELPER FUNCTIONS ===

CategoryNode* CategoryManager::findCategoryRecursive(CategoryNode* node, int id) {
    if (!node) return nullptr;
    if (node->id == id) return node;

    for (CategoryNode* child : node->children) {
        CategoryNode* result = findCategoryRecursive(child, id);
        if (result) return result;
    }
    return nullptr;
}

void CategoryManager::displayCategoryTree(CategoryNode* node, int level, bool showProducts) {
    if (!node) return;

    // In danh mục hiện tại
    for (int i = 0; i < level; i++) cout << "  ";
    cout << "|- " << node->name << " (ID: " << node->id << ")\n";

    // In sản phẩm nếu yêu cầu
    if (showProducts && !node->products.empty()) {
        for (int i = 0; i < level + 1; i++) cout << "  ";
        cout << "  [San pham: " << node->products.size() << "]\n";
        for (const auto& product : node->products) {
            for (int i = 0; i < level + 2; i++) cout << "  ";
            product.display();
        }
    }

    // Duyệt các con
    for (CategoryNode* child : node->children) {
        displayCategoryTree(child, level + 1, showProducts);
    }
}

void CategoryManager::collectAllProducts(CategoryNode* node, vector<Product>& result) {
    if (!node) return;

    // Thêm sản phẩm của node hiện tại
    result.insert(result.end(), node->products.begin(), node->products.end());

    // Duyệt các con
    for (CategoryNode* child : node->children) {
        collectAllProducts(child, result);
    }
}

void CategoryManager::clearTree(CategoryNode* node) {
    if (!node) return;

    for (CategoryNode* child : node->children) {
        clearTree(child);
    }

    // Xóa khỏi hash maps
    categoryMap.erase(node->id);
    for (auto& product : node->products) {
        productMap.erase(product.id);
    }

    delete node;
}

void CategoryManager::mergeSortProducts(vector<Product>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortProducts(arr, left, mid);
        mergeSortProducts(arr, mid + 1, right);
        mergeProducts(arr, left, mid, right);
    }
}

void CategoryManager::mergeProducts(vector<Product>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Product> leftArr(n1), rightArr(n2);

    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].price <= rightArr[j].price) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// === QUẢN LÝ DANH MỤC ===

bool CategoryManager::createCategory(int parentId, const string& name) {
    CategoryNode* newNode = new CategoryNode(nextCategoryId++, name);
    allNodes.push_back(newNode);

    // Thêm vào hash table
    categoryMap[newNode->id] = newNode;

    if (parentId == 0) {
        // Tạo danh mục gốc
        if (!root) {
            root = newNode;
            return true;
        }
        return false;
    }

    // Tìm category cha
    CategoryNode* parent = findCategory(parentId);
    if (parent) {
        newNode->parent = parent;
        parent->children.push_back(newNode);
        return true;
    }

    return false;
}

void CategoryManager::viewAllCategories(bool showProducts) {
    if (!root) {
        cout << "Chua co danh muc nao!\n";
        return;
    }
    displayCategoryTree(root, 0, showProducts);
}

bool CategoryManager::editCategory(int id, const string& newName) {
    CategoryNode* node = findCategory(id);
    if (node) {
        node->name = newName;
        return true;
    }
    return false;
}

bool CategoryManager::deleteCategory(int id) {
    if (!root || id == root->id) return false;

    CategoryNode* node = findCategory(id);
    if (!node) return false;

    if (node->parent) {
        auto& children = node->parent->children;
        auto it = find(children.begin(), children.end(), node);
        if (it != children.end()) {
            children.erase(it);
        }
    }

    // Xóa khỏi hash map
    categoryMap.erase(id);

    // Giải phóng bộ nhớ cho node và các con
    clearTree(node);
    return true;
}

string CategoryManager::getPath(int id) {
    CategoryNode* node = findCategory(id);
    if (!node) return "";

    string path = node->name;
    while (node->parent) {
        node = node->parent;
        path = node->name + " -> " + path;
    }
    return path;
}

void CategoryManager::viewSubCategories(int id) {
    CategoryNode* node = findCategory(id);
    if (!node) {
        cout << "Khong tim thay danh muc!\n";
        return;
    }

    if (node->children.empty()) {
        cout << "Danh muc nay khong co danh muc con.\n";
        return;
    }

    cout << "Cac danh muc con cua '" << node->name << "':\n";
    for (CategoryNode* child : node->children) {
        cout << "  - " << child->name << " (ID: " << child->id << ")\n";
    }
}

CategoryNode* CategoryManager::findCategory(int id) {
    // Sử dụng hash map để tìm O(1)
    auto it = categoryMap.find(id);
    if (it != categoryMap.end()) {
        return it->second;
    }
    return nullptr;
}

// === QUẢN LÝ SẢN PHẨM ===

bool CategoryManager::addProduct(int categoryId, const string& name, double price,
    int quantity, const string& description) {
    CategoryNode* category = findCategory(categoryId);
    if (!category) return false;

    Product newProduct(nextProductId++, name, price, quantity, description, category->name);

    // Thêm vào vector products của category
    category->products.push_back(newProduct);

    // Thêm vào hash map sản phẩm
    productMap[newProduct.id] = &category->products.back();

    return true;
}

bool CategoryManager::editProduct(int productId, const string& name, double price,
    int quantity, const string& description) {
    Product* product = findProduct(productId);
    if (!product) return false;

    product->name = name;
    product->price = price;
    product->quantity = quantity;
    product->description = description;
    return true;
}

bool CategoryManager::deleteProduct(int categoryId, int productId) {
    CategoryNode* category = findCategory(categoryId);
    if (!category) return false;

    auto& products = category->products;
    auto it = find_if(products.begin(), products.end(),
        [productId](const Product& p) { return p.id == productId; });

    if (it != products.end()) {
        productMap.erase(productId);
        products.erase(it);
        return true;
    }
    return false;
}

void CategoryManager::viewProductsInCategory(int categoryId) {
    CategoryNode* category = findCategory(categoryId);
    if (!category) {
        cout << "Khong tim thay danh muc!\n";
        return;
    }

    if (category->products.empty()) {
        cout << "Danh muc nay chua co san pham.\n";
        return;
    }

    cout << "San pham trong danh muc '" << category->name << "':\n";
    for (const auto& product : category->products) {
        product.display();
    }
}

void CategoryManager::viewAllProductsFrom(int categoryId) {
    CategoryNode* category = findCategory(categoryId);
    if (!category) {
        cout << "Khong tim thay danh muc!\n";
        return;
    }

    vector<Product> allProducts;
    collectAllProducts(category, allProducts);

    if (allProducts.empty()) {
        cout << "Khong co san pham nao trong danh muc nay va cac danh muc con.\n";
        return;
    }

    cout << "Tat ca san pham trong danh muc '" << category->name << "' va cac danh muc con:\n";
    for (const auto& product : allProducts) {
        product.display();
    }
}

Product* CategoryManager::findProduct(int productId) {
    // Sử dụng hash map để tìm O(1)
    auto it = productMap.find(productId);
    if (it != productMap.end()) {
        return it->second;
    }
    return nullptr;
}

void CategoryManager::searchProduct(const string& keyword) {
    bool found = false;
    cout << "Ket qua tim kiem cho '" << keyword << "':\n";

    vector<Product> allProducts;
    collectAllProducts(root, allProducts);

    for (auto& product : allProducts) {
        if (product.name.find(keyword) != string::npos ||
            product.description.find(keyword) != string::npos) {
            product.display();
            found = true;
        }
    }

    if (!found) {
        cout << "Khong tim thay san pham nao.\n";
    }
}

void CategoryManager::sortProductsInCategory(int categoryId) {
    CategoryNode* category = findCategory(categoryId);
    if (!category) {
        cout << "Khong tim thay danh muc!\n";
        return;
    }

    if (category->products.empty()) {
        cout << "Danh muc nay khong co san pham de sap xep.\n";
        return;
    }

    mergeSortProducts(category->products, 0, static_cast<int>(category->products.size()) - 1);
    cout << "Da sap xep san pham theo gia tang dan.\n";
}

// === LƯU TRỮ DỮ LIỆU ===

bool CategoryManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    // Xóa dữ liệu cũ
    clearTree(root);
    root = nullptr;
    nextCategoryId = 1;
    nextProductId = 1;
    categoryMap.clear();
    productMap.clear();
    allNodes.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "CATEGORY") {
            int id, parentId;
            string name;
            ss >> id >> parentId;
            getline(ss, name);
            // Bỏ khoảng trắng đầu
            size_t pos = name.find_first_not_of(" \t");
            if (pos != string::npos) {
                name = name.substr(pos);
            }

            // Tạo category với ID cụ thể
            CategoryNode* newNode = new CategoryNode(id, name);
            allNodes.push_back(newNode);
            categoryMap[id] = newNode;

            if (parentId == 0) {
                root = newNode;
            }
            else {
                CategoryNode* parent = findCategory(parentId);
                if (parent) {
                    newNode->parent = parent;
                    parent->children.push_back(newNode);
                }
            }

            if (id >= nextCategoryId) nextCategoryId = id + 1;
        }
        else if (type == "PRODUCT") {
            int id, categoryId, quantity;
            string name, description;
            double price;
            ss >> id >> categoryId >> name >> price >> quantity;
            getline(ss, description);
            size_t pos = description.find_first_not_of(" \t");
            if (pos != string::npos) {
                description = description.substr(pos);
            }

            CategoryNode* category = findCategory(categoryId);
            if (category) {
                Product newProduct(id, name, price, quantity, description, category->name);
                category->products.push_back(newProduct);
                productMap[id] = &category->products.back();

                if (id >= nextProductId) nextProductId = id + 1;
            }
        }
    }

    file.close();
    return true;
}

bool CategoryManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // Lưu tất cả category (duyệt theo thứ tự)
    vector<CategoryNode*> nodes;
    // Thu thập tất cả node
    if (root) {
        // Duyệt BFS để thu thập
        vector<CategoryNode*> queue;
        queue.push_back(root);
        while (!queue.empty()) {
            CategoryNode* node = queue.front();
            queue.erase(queue.begin());
            nodes.push_back(node);
            for (CategoryNode* child : node->children) {
                queue.push_back(child);
            }
        }
    }

    // Lưu category
    for (CategoryNode* node : nodes) {
        int parentId = node->parent ? node->parent->id : 0;
        file << "CATEGORY " << node->id << " " << parentId << " " << node->name << "\n";
    }

    // Lưu product
    for (CategoryNode* node : nodes) {
        for (const Product& product : node->products) {
            file << "PRODUCT " << product.id << " " << node->id << " "
                << product.name << " " << product.price << " " << product.quantity
                << " " << product.description << "\n";
        }
    }

    file.close();
    return true;
}

// === THỐNG KÊ ===

int CategoryManager::countTotalProducts(CategoryNode* node) {
    if (!node) return 0;

    int total = static_cast<int>(node->products.size());
    for (CategoryNode* child : node->children) {
        total += countTotalProducts(child);
    }
    return total;
}

int CategoryManager::countTotalCategories() {
    return static_cast<int>(categoryMap.size());
}
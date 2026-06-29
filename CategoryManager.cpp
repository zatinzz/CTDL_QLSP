#include "CategoryManager.h"
#include <iostream>
#include <iomanip>
#include <limits>

static void clrIn() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

CategoryManager::CategoryManager(CategoryTree& tree) : tree_(tree) {}

void CategoryManager::printTree(CategoryNode* node, int depth) {
    if (!node) return;
    std::string indent(depth * 4, ' ');
    std::string prefix = (depth == 0) ? "" : "|-- ";
    std::cout << indent << prefix
              << "[" << node->id << "] " << node->name
              << " (" << node->products.size << " SP)\n";
    for (CategoryNode* c = node->firstChild; c; c = c->nextSibling)
        printTree(c, depth + 1);
}


void CategoryManager::showMenu() {
    int choice;
    do {
        std::cout << "\n╔══════════════════════════════════════╗\n";
        std::cout << "║        QUẢN LÝ DANH MỤC             ║\n";
        std::cout << "╠══════════════════════════════════════╣\n";
        std::cout << "║  1. Thêm danh mục                   ║\n";
        std::cout << "║  2. Xem cây danh mục                ║\n";
        std::cout << "║  3. Sửa tên danh mục                ║\n";
        std::cout << "║  4. Xóa danh mục (kiểm tra)         ║\n";
        std::cout << "║  5. Đường dẫn từ Root               ║\n";
        std::cout << "║  6. Xem danh mục con                ║\n";
        std::cout << "║  7. SP trong danh mục               ║\n";
        std::cout << "║  8. Tất cả SP (cây con)             ║\n";
        std::cout << "║  0. Quay lại                        ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
        std::cout << "  Chọn: ";
        std::cin >> choice; clrIn();

        switch (choice) {
            case 1: addCategory();            break;
            case 2: viewCategory();           break;
            case 3: editCategory();           break;
            case 4: deleteCategory();         break;
            case 5: showPath();              break;
            case 6: showChildren();          break;
            case 7: showProductsInCategory();break;
            case 8: showAllProductsDown();   break;
            case 0: break;
            default: std::cout << "  [!] Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);
}


void CategoryManager::addCategory() {
    std::cout << "\n--- THÊM DANH MỤC ---\n";
    std::cout << "  Tên danh mục mới: ";
    std::string name; std::getline(std::cin, name);
    std::cout << "  ID danh mục cha (0=Root): ";
    int parentId; std::cin >> parentId; clrIn();

    int id = tree_.nextCategoryId();
    auto* node = tree_.addCategory(id, name, parentId);
    if (node)
        std::cout << "  [✓] Tạo \"" << name << "\" (ID=" << id << ")\n";
    else
        std::cout << "  [!] Thất bại: ID trùng hoặc cha không tồn tại.\n";
}

void CategoryManager::viewCategory() {
    std::cout << "\n--- CÂY DANH MỤC ---\n";
    printTree(tree_.getRoot());
}

void CategoryManager::editCategory() {
    std::cout << "\n--- SỬA TÊN DANH MỤC ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();
    std::cout << "  Tên mới: ";
    std::string nm; std::getline(std::cin, nm);
    if (tree_.updateCategory(id, nm))
        std::cout << "  [✓] Đã đổi tên.\n";
    else
        std::cout << "  [!] Không tìm thấy danh mục.\n";
}

void CategoryManager::deleteCategory() {
    std::cout << "\n--- XÓA DANH MỤC ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();

    auto* node = tree_.findCategory(id);
    if (!node) { std::cout << "  [!] Không tìm thấy.\n"; return; }

    // Kiểm tra trước khi xóa
    if (!node->isLeaf()) {
        std::cout << "  [✗] Không thể xóa: danh mục còn " ;
        int cnt = 0;
        for (CategoryNode* c = node->firstChild; c; c = c->nextSibling) ++cnt;
        std::cout << cnt << " danh mục con.\n";
        return;
    }
    if (node->products.size > 0) {
        std::cout << "  [✗] Không thể xóa: danh mục còn " << node->products.size << " sản phẩm.\n";
        return;
    }

    std::string nm = node->name;
    if (tree_.deleteCategory(id))
        std::cout << "  [✓] Đã xóa danh mục \"" << nm << "\"\n";
    else
        std::cout << "  [!] Xóa thất bại.\n";
}

void CategoryManager::showPath() {
    std::cout << "\n--- ĐƯỜNG DẪN ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();

    auto path = tree_.getPath(id);
    if (path.empty()) { std::cout << "  [!] Không tìm thấy.\n"; return; }

    std::cout << "  ";
    for (int i = 0; i < (int)path.size(); ++i) {
        if (i) std::cout << " > ";
        std::cout << path[i];
    }
    std::cout << "\n";
}

void CategoryManager::showChildren() {
    std::cout << "\n--- DANH MỤC CON ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();

    auto children = tree_.getChildren(id);
    if (children.empty()) { std::cout << "  (Không có danh mục con)\n"; return; }

    std::cout << "  " << std::string(40, '-') << "\n";
    for (auto* c : children)
        std::cout << "  [" << c->id << "] " << c->name
                  << "  (" << c->products.size << " SP trực tiếp)\n";
}

void CategoryManager::showProductsInCategory() {
    std::cout << "\n--- SP TRONG DANH MỤC ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();

    auto products = tree_.getProducts(id);
    if (products.empty()) { std::cout << "  (Không có sản phẩm trực tiếp)\n"; return; }

    std::cout << "  " << std::string(60, '-') << "\n";
    std::cout << "  " << std::left
              << std::setw(5)  << "ID"
              << std::setw(30) << "Tên"
              << std::setw(16) << "Giá" << "\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    for (auto* p : products)
        std::cout << "  " << std::setw(5) << p->id
                  << std::setw(30) << p->name
                  << std::setw(16) << (long long)p->price << "\n";
}

void CategoryManager::showAllProductsDown() {
    std::cout << "\n--- TẤT CẢ SP (TOÀN CÂY CON) ---\n";
    std::cout << "  ID danh mục: ";
    int id; std::cin >> id; clrIn();

    auto products = tree_.getAllProducts(id);
    if (products.empty()) { std::cout << "  (Không có sản phẩm)\n"; return; }

    std::cout << "  " << std::string(70, '-') << "\n";
    std::cout << "  " << std::left
              << std::setw(4)  << "No"
              << std::setw(5)  << "ID"
              << std::setw(28) << "Tên"
              << std::setw(16) << "Giá"
              << std::setw(16) << "Danh mục" << "\n";
    std::cout << "  " << std::string(70, '-') << "\n";
    int no = 1;
    for (auto* p : products)
        std::cout << "  " << std::setw(4) << no++
                  << std::setw(5) << p->id
                  << std::setw(28) << p->name.substr(0, 26)
                  << std::setw(16) << (long long)p->price
                  << std::setw(16) << p->categoryName << "\n";
    std::cout << "  Tổng: " << products.size() << " sản phẩm\n";
}

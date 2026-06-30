#include "ProductManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>

static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string formatPrice(double p) {
    // Format: 29,990,000
    long long n = (long long)p;
    std::string s = std::to_string(n);
    int pos = (int)s.size() - 3;
    while (pos > 0) { s.insert(pos, "."); pos -= 3; }
    return s + " VNĐ";
}

ProductManager::ProductManager(CategoryTree& tree) : tree_(tree) {}

void ProductManager::printProductHeader() {
    std::cout << "\n  " << std::string(80, '-') << "\n";
    std::cout << "  " << std::left
        << std::setw(4) << "No"
        << std::setw(5) << "ID"
        << std::setw(30) << "Tên sản phẩm"
        << std::setw(18) << "Giá (VNĐ)"
        << std::setw(6) << "SL"
        << std::setw(20) << "Danh mục"
        << "\n";
    std::cout << "  " << std::string(80, '-') << "\n";
}

void ProductManager::printProduct(const Product* p, int no) {
    std::cout << "  " << std::left
        << std::setw(4) << (no >= 0 ? std::to_string(no) : "")
        << std::setw(5) << p->id
        << std::setw(30) << p->name.substr(0, 28)
        << std::setw(18) << formatPrice(p->price)
        << std::setw(6) << p->quantity
        << std::setw(20) << p->categoryName
        << "\n";
}


int ProductManager::selectCategory(const std::string& prompt) {
    std::cout << prompt;
    int id; std::cin >> id; clearInput();
    return id;
}

int ProductManager::chooseSortField() {
    std::cout << "\n  Sắp xếp theo:\n"
        << "  1. Tên\n  2. Giá\n  3. Số lượng\n  > ";
    int c; std::cin >> c; clearInput();
    return c;
}

int ProductManager::chooseSortOrder() {
    std::cout << "  Thứ tự: 1. Tăng dần  2. Giảm dần\n  > ";
    int c; std::cin >> c; clearInput();
    return c;
}

void ProductManager::showMenu() {
    int choice;
    do {
        std::cout << "\n╔══════════════════════════════════╗\n";
        std::cout << "║      QUẢN LÝ SẢN PHẨM           ║\n";
        std::cout << "╠══════════════════════════════════╣\n";
        std::cout << "║  1. Thêm sản phẩm                ║\n";
        std::cout << "║  2. Sửa sản phẩm                 ║\n";
        std::cout << "║  3. Xóa sản phẩm                 ║\n";
        std::cout << "║  4. DS sản phẩm trong danh mục   ║\n";
        std::cout << "║  5. Sắp xếp sản phẩm             ║\n";
        std::cout << "║  6. Tìm kiếm theo tên            ║\n";
        std::cout << "║  7. Tìm kiếm theo khoảng giá     ║\n";
        std::cout << "║  8. Báo cáo hiệu năng            ║\n";
        std::cout << "║  0. Quay lại                     ║\n";
        std::cout << "╚══════════════════════════════════╝\n";
        std::cout << "  Chọn: ";
        std::cin >> choice; clearInput();

        switch (choice) {
        case 1: addProduct();    break;
        case 2: editProduct();   break;
        case 3: deleteProduct(); break;
        case 4: listProducts();  break;
        case 5: sortProducts();  break;
        case 6: searchProduct(); break;
        case 7: searchByPrice(); break;
        case 8: {
            auto all = tree_.getAllProducts(tree_.getRoot() ? tree_.getRoot()->id : 1);
            Algorithm::performanceReport(all);
            break;
        }
        case 0: break;
        default: std::cout << "  [!] Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);
}

void ProductManager::addProduct() {
    std::cout << "\n--- THÊM SẢN PHẨM ---\n";
    int catId = selectCategory("  ID danh mục chứa SP: ");
    CategoryNode* cat = tree_.findCategory(catId);
    if (!cat) { std::cout << "  [!] Danh mục không tồn tại.\n"; return; }
    if (cat == tree_.getRoot()) { std::cout << "  [!] Không thêm SP vào Root.\n"; return; }

    int id = tree_.nextProductId();
    std::cout << "  Tên SP: ";
    std::string name; std::getline(std::cin, name);
    std::cout << "  Giá (VNĐ): ";
    double price; std::cin >> price; clearInput();
    std::cout << "  Số lượng: ";
    int qty; std::cin >> qty; clearInput();
    std::cout << "  Mô tả: ";
    std::string desc; std::getline(std::cin, desc);

    auto* p = new Product(id, name, price, qty, desc, cat->name);
    cat->products.push(p);
    std::cout << "  [✓] Đã thêm SP \"" << name << "\" (ID=" << id << ") vào \"" << cat->name << "\"\n";
}

void ProductManager::editProduct() {
    std::cout << "\n--- SỬA SẢN PHẨM ---\n";
    int catId = selectCategory("  ID danh mục chứa SP: ");
    CategoryNode* cat = tree_.findCategory(catId);
    if (!cat) { std::cout << "  [!] Danh mục không tồn tại.\n"; return; }

    std::cout << "  ID sản phẩm cần sửa: ";
    int pid; std::cin >> pid; clearInput();
    Product* p = cat->products.find(pid);
    if (!p) { std::cout << "  [!] Sản phẩm không tìm thấy.\n"; return; }

    std::cout << "  Tên mới (Enter=giữ nguyên): ";
    std::string nm; std::getline(std::cin, nm);
    if (!nm.empty()) p->name = nm;

    std::cout << "  Giá mới (0=giữ nguyên): ";
    double pr; std::cin >> pr; clearInput();
    if (pr > 0) p->price = pr;

    std::cout << "  Số lượng mới (0=giữ nguyên): ";
    int qty; std::cin >> qty; clearInput();
    if (qty > 0) p->quantity = qty;

    std::cout << "  Mô tả mới (Enter=giữ nguyên): ";
    std::string desc; std::getline(std::cin, desc);
    if (!desc.empty()) p->description = desc;

    std::cout << "  [✓] Đã cập nhật sản phẩm.\n";
}

void ProductManager::deleteProduct() {
    std::cout << "\n--- XÓA SẢN PHẨM ---\n";
    int catId = selectCategory("  ID danh mục: ");
    std::cout << "  ID sản phẩm: ";
    int pid; std::cin >> pid; clearInput();
    if (tree_.removeProduct(catId, pid))
        std::cout << "  [✓] Đã xóa sản phẩm.\n";
    else
        std::cout << "  [!] Không tìm thấy sản phẩm.\n";
}

void ProductManager::listProducts() {
    std::cout << "\n--- DS SẢN PHẨM ---\n";
    std::cout << "  1. Trong danh mục (trực tiếp)\n";
    std::cout << "  2. Tất cả SP (kể cả danh mục con)\n";
    std::cout << "  > "; int mode; std::cin >> mode; clearInput();

    int catId = selectCategory("  ID danh mục: ");
    std::vector<Product*> list;
    if (mode == 2) list = tree_.getAllProducts(catId);
    else            list = tree_.getProducts(catId);

    if (list.empty()) { std::cout << "  (Không có sản phẩm)\n"; return; }
    printProductHeader();
    int no = 1;
    for (auto* p : list) printProduct(p, no++);
    std::cout << "  Tổng: " << list.size() << " sản phẩm\n";
}

void ProductManager::sortProducts() {
    std::cout << "\n--- SẮP XẾP SẢN PHẨM ---\n";
    int catId = selectCategory("  ID danh mục (0=tất cả): ");
    std::vector<Product*> list;
    if (catId == 0 && tree_.getRoot())
        list = tree_.getAllProducts(tree_.getRoot()->id);
    else
        list = tree_.getAllProducts(catId);

    if (list.empty()) { std::cout << "  (Không có SP)\n"; return; }

    int fc = chooseSortField();
    int oc = chooseSortOrder();
    SortField sf = (fc == 2) ? SortField::PRICE : (fc == 3) ? SortField::QUANTITY : SortField::NAME;
    SortOrder so = (oc == 2) ? SortOrder::DESC : SortOrder::ASC;

    auto res = Algorithm::sortProducts(list, sf, so);
    printProductHeader();
    int no = 1;
    for (auto* p : res.items) printProduct(p, no++);
    std::cout << "  Thời gian: " << std::fixed << std::setprecision(4)
        << res.elapsedMs << " ms  |  So sánh: " << res.comparisons << "\n";
}

void ProductManager::searchProduct() {
    std::cout << "\n--- TÌM KIẾM THEO TÊN ---\n";
    std::cout << "  Từ khóa: ";
    std::string kw; std::getline(std::cin, kw);

    int catId = selectCategory("  ID danh mục (0=tất cả): ");
    std::vector<Product*> list;
    if (catId == 0 && tree_.getRoot())
        list = tree_.getAllProducts(tree_.getRoot()->id);
    else
        list = tree_.getAllProducts(catId);

    std::cout << "  1. Linear Search  2. Binary Search\n  > ";
    int m; std::cin >> m; clearInput();

    SearchResult res;
    if (m == 2) res = Algorithm::binarySearch(list, kw);
    else        res = Algorithm::linearSearch(list, kw);

    if (res.items.empty()) { std::cout << "  (Không tìm thấy)\n"; return; }
    printProductHeader();
    int no = 1;
    for (auto* p : res.items) printProduct(p, no++);
    std::cout << "  Tìm thấy: " << res.items.size()
        << "  |  Thời gian: " << std::fixed << std::setprecision(4)
        << res.elapsedMs << " ms  |  So sánh: " << res.comparisons << "\n";
}

void ProductManager::searchByPrice() {
    std::cout << "\n--- TÌM KIẾM THEO GIÁ ---\n";
    double mn, mx;
    std::cout << "  Giá tối thiểu: "; std::cin >> mn; clearInput();
    std::cout << "  Giá tối đa:    "; std::cin >> mx; clearInput();

    auto list = tree_.getRoot() ? tree_.getAllProducts(tree_.getRoot()->id)
        : std::vector<Product*>{};
    auto res = Algorithm::searchByPriceRange(list, mn, mx);

    if (res.items.empty()) { std::cout << "  (Không tìm thấy)\n"; return; }
    printProductHeader();
    int no = 1;
    for (auto* p : res.items) printProduct(p, no++);
    std::cout << "  Tìm thấy: " << res.items.size() << "\n";
}
#include <iostream>
#include <limits>
#include "CategoryNode.h"
#include "CategoryManager.h"
#include "ProductManager.h"
#include "FileManager.h"
#include "Algorithm.h"

static const std::string DATA_FILE = "DemoData.txt";

static void clrIn() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    // Thiết lập UTF-8 console (Windows)
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║   HỆ THỐNG QUẢN LÝ SẢN PHẨM THEO DANH MỤC         ║\n";
    std::cout << "║   Tree + Linked List + Hash Table                   ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";

    CategoryTree tree;

    // Nạp dữ liệu
    std::cout << "\n[*] Đang tải dữ liệu từ " << DATA_FILE << " ...\n";
    if (FileManager::loadFromFile(DATA_FILE, tree))
        std::cout << "[✓] Tải dữ liệu thành công!\n";
    else
        std::cout << "[!] Không tải được file, bắt đầu với dữ liệu trống.\n";

    CategoryManager catMgr(tree);
    ProductManager  prodMgr(tree);

    int choice;
    do {
        std::cout << "\n╔══════════════════════════════════╗\n";
        std::cout << "║         MENU CHÍNH               ║\n";
        std::cout << "╠══════════════════════════════════╣\n";
        std::cout << "║  1. Quản lý Danh mục             ║\n";
        std::cout << "║  2. Quản lý Sản phẩm             ║\n";
        std::cout << "║  3. Lưu dữ liệu                  ║\n";
        std::cout << "║  0. Thoát                        ║\n";
        std::cout << "╚══════════════════════════════════╝\n";
        std::cout << "  Chọn: ";
        std::cin >> choice; clrIn();

        switch (choice) {
            case 1: catMgr.showMenu();  break;
            case 2: prodMgr.showMenu(); break;
            case 3:
                if (FileManager::saveToFile(DATA_FILE, tree))
                    std::cout << "[✓] Lưu thành công!\n";
                else
                    std::cout << "[!] Lưu thất bại.\n";
                break;
            case 0:
                std::cout << "  Lưu dữ liệu trước khi thoát? (y/n): ";
                char c; std::cin >> c; clrIn();
                if (c == 'y' || c == 'Y') FileManager::saveToFile(DATA_FILE, tree);
                std::cout << "[*] Tạm biệt!\n";
                break;
            default:
                std::cout << "  [!] Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);

    return 0;
}

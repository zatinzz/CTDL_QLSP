#include "CategoryManager.h"
#include <limits>
#include <fstream>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <locale>
#include <clocale>

using namespace std;

void displayMainMenu() {
    cout << "\n";
    cout << "========================================================\n";
    cout << "        HE THONG QUAN LY SAN PHAM THEO DANH MUC\n";
    cout << "========================================================\n";
    cout << " 1. Quan ly Danh muc\n";
    cout << " 2. Quan ly San pham\n";
    cout << " 3. Xem toan bo he thong\n";
    cout << " 4. Tim kiem san pham\n";
    cout << " 5. Luu / Tai du lieu\n";
    cout << " 6. Thong ke\n";
    cout << " 0. Thoat\n";
    cout << "--------------------------------------------------------\n";
    cout << "Chon chuc nang: ";
}

void displayCategoryMenu() {
    cout << "\n";
    cout << "================ QUAN LY DANH MUC ================\n";
    cout << "1. Tao danh muc moi\n";
    cout << "2. Xem danh sach danh muc\n";
    cout << "3. Sua ten danh muc\n";
    cout << "4. Xoa danh muc\n";
    cout << "5. Xem duong dan danh muc\n";
    cout << "6. Xem danh muc con\n";
    cout << "0. Quay lai\n";
    cout << "--------------------------------------------------\n";
    cout << "Chon chuc nang: ";
}

void displayProductMenu() {
    cout << "\n";
    cout << "================ QUAN LY SAN PHAM ================\n";
    cout << "1. Them san pham moi\n";
    cout << "2. Xem san pham trong danh muc\n";
    cout << "3. Sua thong tin san pham\n";
    cout << "4. Xoa san pham\n";
    cout << "5. Xem tat ca san pham\n";
    cout << "6. Sap xep san pham theo ten\n";
    cout << "0. Quay lai\n";
    cout << "--------------------------------------------------\n";
    cout << "Chon chuc nang: ";
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Hàm tạo dữ liệu mẫu
void createSampleData(CategoryManager& manager) {
    // ===== Danh mục gốc =====
    manager.createCategory(0, "Thiet bi dien tu");

    // ===== Danh mục con =====
    manager.createCategory(1, "Dien thoai");
    manager.createCategory(1, "Laptop");
    manager.createCategory(1, "May tinh bang");
    manager.createCategory(1, "Dong ho thong minh");
    manager.createCategory(1, "Tai nghe");
    manager.createCategory(1, "Phu kien");

    // ===== Điện thoại =====
    manager.addProduct(2, "iPhone 15 Pro Max", 29990000, 15,
        "Apple A17 Pro, 256GB");

    manager.addProduct(2, "Samsung Galaxy S24 Ultra", 25990000, 20,
        "Snapdragon 8 Gen 3");

    manager.addProduct(2, "Xiaomi 14 Ultra", 21990000, 18,
        "Camera Leica");

    // ===== Laptop =====
    manager.addProduct(3, "MacBook Pro M3", 45990000, 8,
        "Laptop Apple M3");

    manager.addProduct(3, "Dell XPS 16", 38990000, 10,
        "Intel Core Ultra");

    manager.addProduct(3, "ASUS ROG Zephyrus G16", 42990000, 6,
        "Laptop Gaming RTX 4070");

    // ===== Máy tính bảng =====
    manager.addProduct(4, "iPad Pro M4", 28990000, 12,
        "OLED 11 inch");

    manager.addProduct(4, "Samsung Galaxy Tab S9 Ultra", 25990000, 10,
        "AMOLED 14.6 inch");

    // ===== Đồng hồ thông minh =====
    manager.addProduct(5, "Apple Watch Series 10", 11990000, 25,
        "GPS 45mm");

    manager.addProduct(5, "Samsung Galaxy Watch Ultra", 12990000, 18,
        "Theo doi suc khoe");

    // ===== Tai nghe =====
    manager.addProduct(6, "AirPods Pro 2", 5990000, 30,
        "Chong on chu dong");

    manager.addProduct(6, "Sony WH-1000XM5", 8990000, 15,
        "Tai nghe chong on");

    // ===== Phụ kiện =====
    manager.addProduct(7, "Sac du phong 20000mAh", 990000, 40,
        "PD 65W");

    manager.addProduct(7, "Ban phim Logitech MX Keys", 2690000, 20,
        "Bluetooth");

    manager.addProduct(7, "Chuot Logitech MX Master 3S", 2490000, 20,
        "Wireless");
}

int main() {
    // Cấu hình UTF-8 cho Windows
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");

    CategoryManager manager;
    int choice = 0;
    string filename = "data.txt";

    // Kiểm tra và tải dữ liệu từ file
    ifstream testFile(filename);
    if (testFile.good()) {
        testFile.close();
        cout << "Phat hien file du lieu. Dang tai...\n";
        if (manager.loadFromFile(filename)) {
            cout << "Tai du lieu thanh cong!\n";
        }
        else {
            cout << "Tai du lieu that bai. Tao du lieu mau...\n";
            createSampleData(manager);
        }
    }
    else {
        cout << "Khong tim thay file du lieu. Tao du lieu mau...\n";
        createSampleData(manager);
        // Lưu dữ liệu mẫu ra file
        manager.saveToFile(filename);
    }

    waitForEnter();

    do {
        clearScreen();
        displayMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: { // Quản lý Danh mục
            int catChoice;
            do {
                clearScreen();
                displayCategoryMenu();
                cin >> catChoice;
                cin.ignore();

                switch (catChoice) {
                case 1: { // Tạo danh mục
                    int parentId;
                    string name;
                    cout << "\nNhap ID danh muc cha: ";
                    cin >> parentId;
                    cin.ignore();
                    cout << "Nhap ten danh muc moi: ";
                    getline(cin, name);
                    if (manager.createCategory(parentId, name)) {
                        cout << "Tao danh muc thanh cong!\n";
                    }
                    else {
                        cout << "Tao danh muc that bai!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 2: { // Xem danh mục
                    int showProducts;
                    cout << "\nHien thi san pham? (1-Co / 0-Khong): ";
                    cin >> showProducts;
                    manager.viewAllCategories(showProducts == 1);
                    waitForEnter();
                    break;
                }
                case 3: { // Sửa danh mục
                    int id;
                    string newName;
                    cout << "\nNhap ID danh muc can sua: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Nhap ten moi: ";
                    getline(cin, newName);
                    if (manager.editCategory(id, newName)) {
                        cout << "Sua danh muc thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay danh muc!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 4: { // Xóa danh mục
                    int id;
                    cout << "\nNhap ID danh muc can xoa: ";
                    cin >> id;
                    if (manager.deleteCategory(id)) {
                        cout << "Xoa danh muc thanh cong!\n";
                    }
                    else {
                        cout << "Khong the xoa danh muc!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 5: { // Xem đường dẫn
                    int id;
                    cout << "\nNhap ID danh muc: ";
                    cin >> id;
                    string path = manager.getPath(id);
                    if (!path.empty()) {
                        cout << "\nDuong dan: " << path << "\n";
                    }
                    else {
                        cout << "Khong tim thay danh muc!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 6: { // Xem danh mục con
                    int id;
                    cout << "\nNhap ID danh muc: ";
                    cin >> id;
                    manager.viewSubCategories(id);
                    waitForEnter();
                    break;
                }
                case 0:
                    cout << "Quay lai menu chinh...\n";
                    break;
                default:
                    cout << "Chuc nang khong hop le!\n";
                    waitForEnter();
                }
            } while (catChoice != 0);
            break;
        }

        case 2: { // Quản lý Sản phẩm
            int prodChoice;
            do {
                clearScreen();
                displayProductMenu();
                cin >> prodChoice;
                cin.ignore();

                switch (prodChoice) {
                case 1: { // Thêm sản phẩm
                    int catId, qty;
                    string name, desc;
                    double price;
                    cout << "\nNhap ID danh muc: ";
                    cin >> catId;
                    cin.ignore();
                    cout << "Ten san pham: ";
                    getline(cin, name);
                    cout << "Gia (VND): ";
                    cin >> price;
                    cout << "So luong: ";
                    cin >> qty;
                    cin.ignore();
                    cout << "Mo ta: ";
                    getline(cin, desc);
                    if (manager.addProduct(catId, name, price, qty, desc)) {
                        cout << "Them san pham thanh cong!\n";
                    }
                    else {
                        cout << "Them san pham that bai!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 2: { // Xem sản phẩm trong danh mục
                    int catId;
                    cout << "\nNhap ID danh muc: ";
                    cin >> catId;
                    manager.viewProductsInCategory(catId);
                    waitForEnter();
                    break;
                }
                case 3: { // Sửa sản phẩm
                    int prodId, qty;
                    string name, desc;
                    double price;
                    cout << "\nNhap ID san pham can sua: ";
                    cin >> prodId;
                    cin.ignore();
                    cout << "Ten moi: ";
                    getline(cin, name);
                    cout << "Gia moi: ";
                    cin >> price;
                    cout << "So luong moi: ";
                    cin >> qty;
                    cin.ignore();
                    cout << "Mo ta moi: ";
                    getline(cin, desc);
                    if (manager.editProduct(prodId, name, price, qty, desc)) {
                        cout << "Sua san pham thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay san pham!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 4: { // Xóa sản phẩm
                    int catId, prodId;
                    cout << "\nNhap ID danh muc: ";
                    cin >> catId;
                    cout << "Nhap ID san pham can xoa: ";
                    cin >> prodId;
                    if (manager.deleteProduct(catId, prodId)) {
                        cout << "Xoa san pham thanh cong!\n";
                    }
                    else {
                        cout << "Xoa san pham that bai!\n";
                    }
                    waitForEnter();
                    break;
                }
                case 5: { // Xem tất cả sản phẩm
                    int catId;
                    cout << "\nNhap ID danh muc: ";
                    cin >> catId;
                    manager.viewAllProductsFrom(catId);
                    waitForEnter();
                    break;
                }
                case 6: { // Sắp xếp sản phẩm
                    int catId;
                    cout << "\nNhap ID danh muc: ";
                    cin >> catId;
                    manager.sortProductsInCategory(catId);
                    waitForEnter();
                    break;
                }
                case 0:
                    cout << "Quay lai menu chinh...\n";
                    break;
                default:
                    cout << "Chuc nang khong hop le!\n";
                    waitForEnter();
                }
            } while (prodChoice != 0);
            break;
        }

        case 3: { // Xem toàn bộ hệ thống
            clearScreen();
            cout << "\n========================================================\n";
            cout << "         TOAN BO HE THONG DANH MUC & SAN PHAM\n";
            cout << "========================================================\n";
            manager.viewAllCategories(true);
            waitForEnter();
            break;
        }

        case 4: { // Tìm kiếm sản phẩm
            string keyword;
            cout << "\nNhap tu khoa tim kiem: ";
            getline(cin, keyword);
            manager.searchProduct(keyword);
            waitForEnter();
            break;
        }

        case 5: { // Lưu / Tải dữ liệu
            clearScreen();
            cout << "\n========================================================\n";
            cout << "         LUU / TAI DU LIEU\n";
            cout << "========================================================\n";
            cout << "1. Luu du lieu\n";
            cout << "2. Tai du lieu tu file\n";
            cout << "0. Quay lai\n";
            cout << "Chon: ";
            int saveChoice;
            cin >> saveChoice;
            cin.ignore();

            if (saveChoice == 1) {
                if (manager.saveToFile(filename)) {
                    cout << "Luu du lieu thanh cong!\n";
                }
                else {
                    cout << "Luu du lieu that bai!\n";
                }
            }
            else if (saveChoice == 2) {
                if (manager.loadFromFile(filename)) {
                    cout << "Tai du lieu thanh cong!\n";
                }
                else {
                    cout << "Tai du lieu that bai!\n";
                }
            }
            waitForEnter();
            break;
        }

        case 6: { // Thống kê
            clearScreen();
            cout << "\n========================================================\n";
            cout << "         THONG KE HE THONG\n";
            cout << "========================================================\n";
            cout << "Tong so danh muc: " << manager.countTotalCategories() << "\n";
            CategoryNode* root = manager.findCategory(1);
            if (root) {
                cout << "Tong so san pham: " << manager.countTotalProducts(root) << "\n";
            }
            else {
                cout << "Tong so san pham: 0\n";
            }
            waitForEnter();
            break;
        }

        case 0: // Thoát
            cout << "\nDang luu du lieu...\n";
            manager.saveToFile(filename);
            cout << "Cam on ban da su dung chuong trinh!\n";
            break;

        default:
            cout << "Chuc nang khong hop le!\n";
            waitForEnter();
        }

    } while (choice != 0);

    return 0;
}
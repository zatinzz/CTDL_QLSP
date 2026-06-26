#include <unordered_map>
#include <iostream>
#include <string>
using namespace std;
// --- THIẾT KẾ LINKED LIST CHO SẢN PHẨM ---
struct ProductNode {
    string id;          // ID sản phẩm (Khóa để băm)
    string name;        // Tên sản phẩm
    double price;       // Giá sản phẩm
    
    ProductNode* next;  // Con trỏ tới sản phẩm tiếp theo trong cùng danh mục
};

// --- THIẾT KẾ TREE NODE CHO DANH MỤC ---
struct CategoryNode {
    string id;           // ID danh mục (Khóa để băm)
    string name;         // Tên danh mục
    
    CategoryNode* parent; // Trỏ ngược lên cha (để phục vụ UC4: Tìm đường dẫn)

    // --- Cấu trúc Linked List cho Danh mục con (Children) ---
    CategoryNode* firstChild;  // Con trỏ tới danh mục con đầu tiên
    CategoryNode* nextSibling; // Con trỏ tới danh mục anh em cùng cấp tiếp theo

    // --- Cấu trúc Linked List cho Sản phẩm (Products) ---
    ProductNode* productHead;  // Con trỏ quản lý DSLK sản phẩm thuộc danh mục này
};



// ==========================================
// LỚP QUẢN LÝ DANH MỤC VÀ SẢN PHẨM TRONG HỆ THỐNG THƯƠNG MẠI ĐIỆN TỬ
// ==========================================   
class ECommerceCategoryManager {
private:
    CategoryNode* root; // Gốc của cây danh mục

    // HASH TABLE: Ánh xạ từ ID (string) sang địa chỉ con trỏ vùng nhớ
    unordered_map<string, CategoryNode*> categoryMap;
    unordered_map<string, ProductNode*> productMap;

public:
    ECommerceCategoryManager() {
        root = nullptr;
    }

    // ==========================================
    // 1. TẠO DANH MỤC MỚI (Thêm vào Cây + Cập nhật Hash Table)
    // ==========================================
    void addCategory(string id, string name, string parentId = "") {
        // Tạo node mới
        CategoryNode* newCat = new CategoryNode{id, name, nullptr, nullptr, nullptr, nullptr};

        // ĐĂNG KÝ VÀO HASH TABLE NGAY LẬP TỨC
        categoryMap[id] = newCat;

        // Nếu là nút gốc (không có cha)
        if (parentId == "") {
            root = newCat;
            return;
        }

        // TÌM KIẾM CHA VỚI ĐỘ PHỨC TẠP O(1) NHỜ HASH TABLE
        if (categoryMap.find(parentId) != categoryMap.end()) {
            CategoryNode* parentNode = categoryMap[parentId];
            newCat->parent = parentNode;

            // Thêm vào đầu danh sách liên kết con của nút cha (Độ phức tạp O(1))
            newCat->nextSibling = parentNode->firstChild;
            parentNode->firstChild = newCat;
        } else {
            cout << "Loi: Khong tim thay danh muc cha co ID: " << parentId << endl;
        }
    }

    // ==========================================
    // 2. TÌM KIẾM DANH MỤC THEO ID - ĐẠT ĐỘ PHỨC TẠP O(1)
    // ==========================================
    CategoryNode* getCategoryById(string id) {
        if (categoryMap.find(id) != categoryMap.end()) {
            return categoryMap[id]; // Trả về con trỏ vùng nhớ trực tiếp
        }
        return nullptr; // Không tồn tại
    }

    // ==========================================
    // 3. THÊM SẢN PHẨM VÀO DANH MỤC
    // ==========================================
    void addProductToCategory(string catId, string prodId, string prodName, double price) {
        // Tìm danh mục chứa sản phẩm bằng Hash Table mất O(1)
        CategoryNode* catNode = getCategoryById(catId);
        if (!catNode) {
            cout << "Loi: Danh muc khong ton tai!\n";
            return;
        }

        // Tạo node sản phẩm mới
        ProductNode* newProd = new ProductNode{prodId, prodName, price, nullptr};

        // ĐĂNG KÝ VÀO HASH TABLE SẢN PHẨM
        productMap[prodId] = newProd;

        // Thêm vào đầu DSLK sản phẩm của danh mục đó (Mất O(1))
        newProd->next = catNode->productHead;
        catNode->productHead = newProd;
    }

    // ==========================================
    // 4. TÌM KIẾM SẢN PHẨM THEO ID - ĐẠT ĐỘ PHỨC TẠP O(1)
    // ==========================================
    ProductNode* getProductById(string id) {
        if (productMap.find(id) != productMap.end()) {
            return productMap[id];
        }
        return nullptr;
    }
};
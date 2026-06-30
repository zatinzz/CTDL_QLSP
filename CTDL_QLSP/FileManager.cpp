#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Tách chuỗi theo delimiter
static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, delim)) tokens.push_back(tok);
    return tokens;
}

bool FileManager::loadFromFile(const std::string& filename, CategoryTree& tree) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "[FileManager] Không mở được file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto tokens = split(line, ':');
        if (tokens.empty()) continue;

        if (tokens[0] == "CATEGORIES" || tokens[0] == "PRODUCTS") continue;

        if (tokens[0] == "CATEGORY" && tokens.size() >= 4) {
            int id = std::stoi(tokens[1]);
            std::string name = tokens[2];
            int parentId = std::stoi(tokens[3]);
            tree.addCategory(id, name, parentId);
        }
        else if (tokens[0] == "PRODUCT" && tokens.size() >= 7) {
            int id = std::stoi(tokens[1]);
            std::string name = tokens[2];
            double price = std::stod(tokens[3]);
            int qty = std::stoi(tokens[4]);
            std::string desc = tokens[5];
            std::string cat = tokens[6];

            CategoryNode* node = tree.findCategoryByName(cat);
            if (node) {
                auto* p = new Product(id, name, price, qty, desc, cat);
                node->products.push(p);
            }
            else {
                std::cerr << "[FileManager] Không tìm thấy danh mục: " << cat << "\n";
            }
        }
    }
    fin.close();
    return true;
}

// DFS lưu categories theo thứ tự
static void writeCategory(std::ofstream& out, CategoryNode* node) {
    if (!node) return;
    int parentId = node->parent ? node->parent->id : 0;
    out << "CATEGORY:" << node->id << ":" << node->name << ":" << parentId << "\n";
    for (CategoryNode* c = node->firstChild; c; c = c->nextSibling)
        writeCategory(out, c);
}

bool FileManager::saveToFile(const std::string& filename, const CategoryTree& tree) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "[FileManager] Không ghi được file: " << filename << "\n";
        return false;
    }

    // Đếm category & product
    auto& tbl = tree.getTable();
    int catCount = (int)tbl.size();
    int prodCount = 0;
    for (auto& [id, node] : tbl) prodCount += node->products.size;

    fout << "CATEGORIES:" << catCount << "\n";
    writeCategory(fout, tree.getRoot());

    // Products
    fout << "PRODUCTS:" << prodCount << "\n";
    for (auto& [id, node] : tbl) {
        for (Product* p = node->products.head; p; p = p->next) {
            fout << "PRODUCT:" << p->id << ":" << p->name << ":"
                << (long long)p->price << ":" << p->quantity << ":"
                << p->description << ":" << p->categoryName << "\n";
        }
    }
    fout.close();
    return true;
}
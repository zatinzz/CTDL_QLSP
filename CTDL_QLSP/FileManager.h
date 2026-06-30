#pragma once
#include "CategoryNode.h"
#include "Product.h" 
#include <string>

class FileManager {
public:
    static bool loadFromFile(const std::string& filename, CategoryTree& tree);
    static bool saveToFile(const std::string& filename, const CategoryTree& tree);
};
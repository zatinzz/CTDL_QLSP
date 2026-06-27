#pragma once
#include "CategoryNode.h"
#include <vector>
#include <string>
#include <chrono>

//  Algorithm: sắp xếp & tìm kiếm sản phẩm + đo hiệu năng

enum class SortField  { NAME, PRICE, QUANTITY };
enum class SortOrder  { ASC, DESC };
enum class SearchField{ NAME, PRICE_RANGE };

struct SearchResult {
    std::vector<Product*> items;
    double  elapsedMs   = 0.0;
    int     comparisons = 0;
};

struct SortResult {
    std::vector<Product*> items;
    double  elapsedMs   = 0.0;
    int     comparisons = 0;
};

class Algorithm {
public:
    // Sort
    static SortResult sortProducts(std::vector<Product*> products,
                                   SortField field = SortField::NAME,
                                   SortOrder order = SortOrder::ASC);

    // Search
    static SearchResult linearSearch(const std::vector<Product*>& products,
                                     const std::string& keyword);

    static SearchResult binarySearch(std::vector<Product*> products,
                                     const std::string& keyword);

    static SearchResult searchByPriceRange(const std::vector<Product*>& products,
                                           double minPrice, double maxPrice);

    //Báo cáo hiệu năng
    static void performanceReport(const std::vector<Product*>& products);

private:
    // Quick sort
    static void quickSort(std::vector<Product*>& v, int lo, int hi,
                          SortField field, SortOrder order, int& cmp);
    static int  partition(std::vector<Product*>& v, int lo, int hi,
                          SortField field, SortOrder order, int& cmp);
    static bool compare(const Product* a, const Product* b,
                        SortField field, SortOrder order);

    static double now();   // millis
};

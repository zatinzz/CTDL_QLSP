#include "Algorithm.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

double Algorithm::now() {
    using namespace std::chrono;
    return duration<double, std::milli>(
        steady_clock::now().time_since_epoch()).count();
}

bool Algorithm::compare(const Product* a, const Product* b,
    SortField field, SortOrder order) {
    bool less_than;
    switch (field) {
    case SortField::PRICE:    less_than = a->price < b->price;    break;
    case SortField::QUANTITY: less_than = a->quantity < b->quantity; break;
    default:                  less_than = a->name < b->name;     break;
    }
    return (order == SortOrder::ASC) ? less_than : !less_than;
}

int Algorithm::partition(std::vector<Product*>& v, int lo, int hi,
    SortField field, SortOrder order, int& cmp) {
    Product* pivot = v[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        ++cmp;
        if (compare(v[j], pivot, field, order)) {
            ++i;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[hi]);
    return i + 1;
}

void Algorithm::quickSort(std::vector<Product*>& v, int lo, int hi,
    SortField field, SortOrder order, int& cmp) {
    if (lo < hi) {
        int p = partition(v, lo, hi, field, order, cmp);
        quickSort(v, lo, p - 1, field, order, cmp);
        quickSort(v, p + 1, hi, field, order, cmp);
    }
}

SortResult Algorithm::sortProducts(std::vector<Product*> products,
    SortField field, SortOrder order) {
    SortResult res;
    res.items = products;
    int cmp = 0;
    double t0 = now();
    if (!res.items.empty())
        quickSort(res.items, 0, (int)res.items.size() - 1, field, order, cmp);
    res.elapsedMs = now() - t0;
    res.comparisons = cmp;
    return res;
}

SearchResult Algorithm::linearSearch(const std::vector<Product*>& products,
    const std::string& keyword) {
    SearchResult res;
    std::string kw = keyword;
    // lowercase
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
    double t0 = now();
    for (Product* p : products) {
        ++res.comparisons;
        std::string nm = p->name;
        std::transform(nm.begin(), nm.end(), nm.begin(), ::tolower);
        if (nm.find(kw) != std::string::npos)
            res.items.push_back(p);
    }
    res.elapsedMs = now() - t0;
    return res;
}

SearchResult Algorithm::binarySearch(std::vector<Product*> products,
    const std::string& keyword) {
    SearchResult res;
    std::string kw = keyword;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

    std::sort(products.begin(), products.end(), [](Product* a, Product* b) {
        return a->name < b->name;
        });

    double t0 = now();
    int lo = 0, hi = (int)products.size() - 1;
    int found = -1;
    while (lo <= hi) {
        ++res.comparisons;
        int mid = lo + (hi - lo) / 2;
        std::string nm = products[mid]->name;
        std::transform(nm.begin(), nm.end(), nm.begin(), ::tolower);
        if (nm.find(kw) != std::string::npos) { found = mid; break; }
        else if (nm < kw) lo = mid + 1;
        else              hi = mid - 1;
    }
    if (found != -1) {
        for (int i = found; i >= 0; --i) {
            std::string nm = products[i]->name;
            std::transform(nm.begin(), nm.end(), nm.begin(), ::tolower);
            if (nm.find(kw) == std::string::npos) break;
            res.items.push_back(products[i]);
        }
        for (int i = found + 1; i < (int)products.size(); ++i) {
            std::string nm = products[i]->name;
            std::transform(nm.begin(), nm.end(), nm.begin(), ::tolower);
            if (nm.find(kw) == std::string::npos) break;
            res.items.push_back(products[i]);
        }
    }
    res.elapsedMs = now() - t0;
    return res;
}

SearchResult Algorithm::searchByPriceRange(const std::vector<Product*>& products,
    double minPrice, double maxPrice) {
    SearchResult res;
    double t0 = now();
    for (Product* p : products) {
        ++res.comparisons;
        if (p->price >= minPrice && p->price <= maxPrice)
            res.items.push_back(p);
    }
    res.elapsedMs = now() - t0;
    return res;
}

void Algorithm::performanceReport(const std::vector<Product*>& products) {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║          BÁO CÁO HIỆU NĂNG THUẬT TOÁN              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "  Số sản phẩm mẫu: " << products.size() << "\n\n";

    struct Row { std::string algo; double ms; int cmp; };
    std::vector<Row> rows;

    {
        auto r = sortProducts(products, SortField::NAME, SortOrder::ASC);
        rows.push_back({ "Quick Sort (tên  ASC)", r.elapsedMs, r.comparisons });
    }
    {
        auto r = sortProducts(products, SortField::PRICE, SortOrder::ASC);
        rows.push_back({ "Quick Sort (giá  ASC)", r.elapsedMs, r.comparisons });
    }
    {
        auto r = sortProducts(products, SortField::PRICE, SortOrder::DESC);
        rows.push_back({ "Quick Sort (giá DESC)", r.elapsedMs, r.comparisons });
    }
    {
        auto r = sortProducts(products, SortField::QUANTITY, SortOrder::ASC);
        rows.push_back({ "Quick Sort (SL   ASC)", r.elapsedMs, r.comparisons });
    }

    std::string kw = products.empty() ? "a" : products[0]->name.substr(0, 3);
    {
        auto r = linearSearch(products, kw);
        rows.push_back({ "Linear Search (\"" + kw + "\")", r.elapsedMs, r.comparisons });
    }
    {
        auto r = binarySearch(products, kw);
        rows.push_back({ "Binary Search (\"" + kw + "\")", r.elapsedMs, r.comparisons });
    }

    std::cout << std::left
        << std::setw(30) << "Thuật toán"
        << std::setw(14) << "Thời gian(ms)"
        << std::setw(12) << "So sánh" << "\n";
    std::cout << std::string(56, '-') << "\n";
    for (auto& row : rows) {
        std::cout << std::setw(30) << row.algo
            << std::setw(14) << std::fixed << std::setprecision(4) << row.ms
            << std::setw(12) << row.cmp << "\n";
    }

    std::cout << "\n  Đánh giá độ phức tạp:\n";
    std::cout << "  - Quick Sort   : O(n log n) trung bình, O(n²) xấu nhất\n";
    std::cout << "  - Linear Search: O(n)\n";
    std::cout << "  - Binary Search: O(log n) — chỉ hiệu quả với dữ liệu đã sắp xếp\n";
    std::cout << "  - Hash Table (tra cứu DM): O(1) trung bình\n\n";
}
#pragma once

#include <deque>
#include <utility>
#include <memory>
#include <unordered_map>
#include <map>

namespace graph {

enum class Color {
    White,
    Gray,
    Black
};

enum class SearchType {
    BFS,
    DFS
};

enum class MSTAlgorithm {
    Prim,
    Kruskal
};

template <typename T>
struct Graph {
    std::deque<T> vertices = {};
    std::deque<std::pair<T, T>> edges = {};
    std::map<std::pair<T, T>, double> edge_to_weight;
    std::function<double(std::pair<T, T>)> weight = [this] (const std::pair<T, T>& edge) {
        return this->edge_to_weight[edge];
    } ;
};

template <typename T>
struct VertixWrapper {
    T vertix;
    int distance;
    Color color = Color::White;
    std::shared_ptr<VertixWrapper<T>> parent = nullptr;
};

}

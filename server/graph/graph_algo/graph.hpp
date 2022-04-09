#pragma once


#include "domain.hpp"
#include "connected_components.hpp"

#include <vector>
#include <deque>
#include <utility>
#include <map>
#include <unordered_map>
#include <list>
#include <initializer_list>
#include <functional>
#include <memory>
#include <set>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <algorithm>

namespace graph {

template <typename T>
std::ostream& operator<<(std::ostream& os, std::pair<T, T> edge) {
    os << "{" << edge.first << "-" << edge.second << "}" << std::endl;
    return os;
}

template <typename T>
void ShowVector(const std::vector<T>& v) {
    for (const auto& elem : v) {
        std::cout << elem << std::endl;
    }
}

template <typename T>
void ShowDeque(const std::deque<T>& v) {
    for (const auto& elem : v) {
        std::cout << elem << std::endl;
    }
}

template <typename T>
class GraphProcess {
public:

    GraphProcess() {}
    GraphProcess(const Graph<T>& graph) {
        graph_representation_ = graph;
    } 

    GraphProcess(Graph<T>&& graph) {
        graph_representation_ = graph;
    } 

    void AddWeightFunction(std::function<double(std::pair<T, T>)> get_weight) {
        graph_representation_.weight = get_weight;
    }

    void AddWeightFunction(const std::vector<std::pair<T, T>>& edges, std::vector<double>&& weights) {
        for (int i = 0; i < edges.size(); ++i) {
            graph_representation_.edge_to_weight[{edges[i].first, edges[i].second}] = weights[i];
            graph_representation_.edge_to_weight[{edges[i].second, edges[i].first}] = weights[i];
        }

        graph_representation_.weight = [this] (const std::pair<T, T>& edge) {
            return this->graph_representation_.edge_to_weight[edge];
        };
    }

    void AddWeightFunction(const std::vector<std::pair<T, T>>& edges, const std::vector<double>& weights) {
        for (int i = 0; i < edges.size(); ++i) {
            graph_representation_.edge_to_weight[{edges[i].first, edges[i].second}] = weights[i];
            graph_representation_.edge_to_weight[{edges[i].second, edges[i].first}] = weights[i];
        }

        graph_representation_.weight = [this] (const std::pair<T, T>& edge) {
            return this->edge_to_weight[edge];
        };
    }

    void InitializeGraph(std::initializer_list<T> vertices, 
            std::initializer_list<std::pair<T, T>> edges) {
        for (const auto& v : vertices) {
            graph_representation_.vertices.push_back(v);
        }

        for (const auto& e : edges) {
            graph_representation_.edges.push_back(e);
        }
    }

    void InitializeGraph(const std::vector<T>& vertices, 
            const std::vector<std::pair<T, T>>& edges) {
        for (const auto& v : vertices) {
            graph_representation_.vertices.push_back(v);
        }

        for (const auto& e : edges) {
            graph_representation_.edges.push_back(e);
        }
    }

    void InitializeGraph(std::initializer_list<T> vertices, 
            std::initializer_list<std::pair<T, T>> edges, std::function<double(std::pair<T, T>)> get_weight)  {
        InitializeGraph(vertices, edges);
        AddWeight(get_weight);
    }

    std::vector<T> GetPath(T source, T target, SearchType type = SearchType::BFS) {
        if (type == SearchType::BFS) {
            return GetPathBFS(source, target);
        } else {
            return GetPathDFS(source, target);
        }
    }

    std::ostream& ShowAdjacencyList(std::ostream& os) {
        for (const auto& [v, connections] : adjacency_list_) {
            os << v << " -> ";
            for (auto elem : connections) {
                os << elem->vertix << " -> ";
            }
            os << "#" << std::endl;
        }

        return os;
    }

    void FormAdjacencyList() {

        for (const auto& v : graph_representation_.vertices) {
            adjacency_list_.insert({v, {}});
        }

        for (const auto& [left, right] : graph_representation_.edges) {
            if (vertices_to_wrappers.count(left) < 1) {
                vertices_to_wrappers.insert({left, std::make_shared<VertixWrapper<T>>(VertixWrapper<T>{left, INT_MAX, Color::White, nullptr})});
            }

            if (vertices_to_wrappers.count(right) < 1) {
                vertices_to_wrappers.insert({right, std::make_shared<VertixWrapper<T>>(VertixWrapper<T>{right, INT_MAX, Color::White, nullptr})});
            }

            adjacency_list_[left].push_back(vertices_to_wrappers.at(right));
            adjacency_list_[right].push_back(vertices_to_wrappers.at(left));
        }

        adjacency_list_formed = true;
    }


    Graph<T> GenerateMST() {
        return GenerateMSTKruskal();
    }
private:

    Graph<T> graph_representation_;

    std::unordered_map<T, std::shared_ptr<VertixWrapper<T>>> vertices_to_wrappers;
    std::unordered_map<T, std::list<std::shared_ptr<VertixWrapper<T>>>> adjacency_list_ = {};
    bool adjacency_list_formed = false;

    std::vector<int> FormPath(T target) {
        std::vector<T> path = {};

        auto target_ptr = vertices_to_wrappers.at(target);
        while (target_ptr->parent != nullptr) {
            path.push_back(target_ptr->vertix);
            target_ptr = target_ptr->parent;
        }
        path.push_back(target_ptr->vertix);
        std::reverse(path.begin(), path.end());

        return path;
    }

    void ClearMarks() {
        for (const auto& [v, wrapper] : vertices_to_wrappers) {
            wrapper->color = Color::White;
            wrapper->distance = INT_MAX;
            wrapper->parent = nullptr;
        }
    }

    std::vector<T> GetPathBFS(T source, T target) {
        if (vertices_to_wrappers.count(source) == 0 || vertices_to_wrappers.count(target) == 0) {
            return {};
        }

        if (!adjacency_list_formed) {
            FormAdjacencyList();
        }
        //Clearing graph from previous usages
        ClearMarks();

        std::shared_ptr<VertixWrapper<T>> source_ptr = vertices_to_wrappers.at(source);
        source_ptr->color = Color::Gray;
        source_ptr->distance = 0;

        std::queue<std::shared_ptr<VertixWrapper<T>>> q;
        q.push(source_ptr);

        while(!q.empty()) {
            auto u = q.front();
            q.pop();
            for (const auto& vptr : adjacency_list_[u->vertix]) {
                if (vptr->color == Color::White) {
                    vptr->color = Color::Gray;
                    vptr->distance = u->distance + 1;
                    vptr->parent = u;
                    q.push(vptr);
                }
            }
            u->color = Color::Black;
        }

        return FormPath(target);
    }

    void DFSVisit(std::shared_ptr<VertixWrapper<T>> uptr) {
        uptr->color = Color::Gray;
        for (const auto& vptr : adjacency_list_.at(uptr->vertix)) {
            if (vptr->color == Color::White) {
                vptr->parent = uptr;
                DFSVisit(vptr);
            }
        }
        uptr->color = Color::Black;
    }

    std::vector<T> GetPathDFS(T source, T target) {
        if (vertices_to_wrappers.count(source) == 0 || vertices_to_wrappers.count(target) == 0) {
            return {};
        }

        if (!adjacency_list_formed) {
            FormAdjacencyList();
        }

        //Clearing Graph from previous usages
        ClearMarks();

        auto source_ptr = vertices_to_wrappers.at(source);
        DFSVisit(source_ptr);

        return FormPath(target);
    }

    Graph<T> GenerateMSTKruskal() {
        Graph<T> mst;
        mst.vertices = {graph_representation_.vertices};
        mst.edges = {};

        std::vector<std::pair<T, T>> sorted_edges{graph_representation_.edges.begin(), 
                                                    graph_representation_.edges.end()};
        
        std::sort(sorted_edges.begin(), sorted_edges.end(), [&] (const auto& lhs, const auto& rhs) {
            return graph_representation_.weight(lhs) < graph_representation_.weight(rhs);
        });

        ConnectedComponents<T> components({mst.vertices.begin(), mst.vertices.end()});

        for (const auto& edge : sorted_edges) {
            if (!components.InSameComponent(edge.first, edge.second)) {
                mst.edges.push_back(edge);
                components.LinkVertices(edge.first, edge.second);
            }
        }

        return mst;
    }

    //Graph<T> GenerateMSTPrim();
};

}
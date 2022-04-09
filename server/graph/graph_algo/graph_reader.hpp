#include "json/json.hpp"
#include "domain.hpp"

#include <string>
#include <iostream>
#include <unordered_map>

namespace graph {


class GraphReader {
public:
    GraphReader() {
        graph.vertices = {};
        graph.edges = {};
        graph.edge_to_weight = {};
    }

    Graph<std::string> GetGraph() {
        return graph;
    }

    void ParseGraphJson(std::istream& input) {
        json::Document document = json::Load(input);
        json::Node root = document.GetRoot();

        //Adding vertices
        for (auto& element : root.AsMap().at("vertices").AsArray()) {
            graph.vertices.push_back(element.AsString());
        }

        std::unordered_map<int, std::pair<std::string, std::string>> ids_to_edges;

        //Adding edges
        for (auto& [string_id, edge_node] : root.AsMap().at("edges").AsMap()) {
            int id = std::stoi(string_id);
            std::pair<std::string, std::string> edge{edge_node.AsArray()[0].AsString(), edge_node.AsArray()[1].AsString()};
            ids_to_edges[id] = edge;
            graph.edges.push_back(edge);
        }

        //adding weights and connecting them with edges
        for (auto& [string_id, weight] : root.AsMap().at("weights").AsMap()) {
            int id = std::stoi(string_id);
            auto edge = ids_to_edges.at(id);
            graph.edge_to_weight[edge] = weight.AsDouble();
            graph.edge_to_weight[{edge.second, edge.first}] = weight.AsDouble();
        }
    }

private:
    Graph<std::string> graph;
};

}
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>

#include "graph.hpp"
#include "graph_reader.hpp"
#include "graph_painter.hpp"

using namespace graph;

/*template <typename T>
using GraphSetUp = std::pair<std::vector<T>, std::vector<std::pair<T, T>>>;

template <typename T>
void ShowGraphRepresentation(std::ostream& os, const Graph<T>& g) {
    std::cout << "{ ";
    for (const auto& v : g.vertices) {
        std::cout << v << ", ";
    }
    std::cout << "}" << std::endl;

    for (const auto& edge : g.edges) {
        std::cout << "{ " << edge.first << " - " << edge.second << " }" << std::endl;
    }
}*/



int main() {

    GraphReader reader;
    reader.ParseGraphJson(std::cin);

    auto g = reader.GetGraph();

    GraphProcess<std::string> graph(g);

    auto mst = graph.GenerateMST();

    GraphPainter painter;
    painter.SetGraph(g);
    painter.SetMST(mst);
    painter.SetMSTHighlight(true);
    painter.RenderGraph(std::cout);  

    
    //std::string say_hello;
    //std::cin >> say_hello;
    //std::cout << say_hello << " from cpp program" << std::endl;

      

    return 0;
}
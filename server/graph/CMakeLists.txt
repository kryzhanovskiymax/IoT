
   
cmake_minimum_required (VERSION 3.5)
project (GraphAPI)

set(SOURCES 
        ./graph_algo/main.cpp
        ./graph_algo/svg/svg.cpp
        ./graph_algo/json/json.cpp
)

set(HEADERS 
        ./graph_algo/domain.hpp
        ./graph_algo/graph.hpp
        ./graph_algo/connected_components.hpp
        ./graph_algo/svg/svg.hpp
        ./graph_algo/json/json.hpp
)

add_executable(graph ${SOURCES} ${HEADERS})
set_target_properties(
    graph PROPERTIES
    CXX_STANDARD 17 
    CXX_STANDARD_REQUIRED ON
)
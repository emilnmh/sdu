#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/topological_sort.hpp"

using namespace graph;



void topoSort() {
    std::cout << "--- Topological Sort ---" << std::endl;

    AdjacencyList<tags::Directed, std::string> g;

    std::vector<std::string> clothes{"shirt", "tie", "jacket", "belt", "watch", "undershorts", "pants", "shoes", "socks"};
    std::vector<std::pair<int, int>> edges{{0, 1}, {0, 3}, {1, 2}, {3, 2}, {5, 6}, {5, 7}, {6, 3}, {6, 7}, {8, 7}};
    std::vector<AdjacencyList<tags::Directed, std::string>::VertexDescriptor> vertices;

    for (std::string c : clothes) {
        vertices.push_back(addVertex(c, g));
    }

    for (std::pair<int, int> e : edges) {
        addEdge(vertices.at(e.first), vertices.at(e.second), g);
    }
    

    std::vector<typename graph::Traits<AdjacencyList<tags::Directed, std::string>>::VertexDescriptor> vs;
    vs.reserve(numVertices(g));
    graph::topoSort(g, std::back_inserter(vs));
    std::reverse(vs.begin(), vs.end());


    std::vector<std::string> expected{"socks", "undershorts", "pants", "shoes", "watch", "shirt", "belt", "tie", "jacket"};
    size_t i = 0;
    for (typename graph::Traits<AdjacencyList<tags::Directed, std::string>>::VertexDescriptor v : vs) {
        assert(g[v] == expected.at(i));
        i++;
    }

    std::cout << "Topological Sort: SUCCESS" << std::endl;
}
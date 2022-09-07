#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/topological_sort.hpp"
#include <vector>

using namespace graph;


/**
 * Tests inDegree for bidirectional adjacency list.
 */
void testInDegree() {
    AdjacencyList<tags::Bidirectional> g(5);

    for (size_t i = 0; i < numVertices(g); i++) {
        assert(inDegree(i, g) == 0);
    }
    
    addEdge(0, 1, g);
    addEdge(0, 4, g);
    addEdge(2, 1, g);
    addEdge(3, 4, g);
    addEdge(1, 4, g);

    std::vector<size_t> inEdgeCounts{0, 2, 0, 0, 3};
    for (size_t i = 0; i < numVertices(g); i++) {
        assert(inDegree(i, g) == inEdgeCounts.at(i));
    }
    
    std::cout << "InDegree (Bidirectional): SUCCESS" << std::endl;
}

void testInEdges() noexcept {
    AdjacencyList<tags::Bidirectional> g(5);
    addEdge(0, 1, g);
    addEdge(0, 4, g);
    addEdge(2, 1, g);
    addEdge(3, 4, g);
    addEdge(1, 4, g);

    std::vector<std::vector<std::pair<std::size_t, std::size_t>>> iEdges{{}, {{0, 1}, {2, 1}}, {}, {}, {{0, 4}, {3, 4}, {1, 4}}};
    for (size_t i = 0; i < numVertices(g); i++) {
        AdjacencyList<tags::Bidirectional>::InEdgeRange iEdgeRange = inEdges(i, g);
        size_t j = 0;
        for (AdjacencyList<tags::Bidirectional>::EdgeDescriptor e : iEdgeRange) {
            assert(iEdges.size() > i);
            assert(iEdges.at(i).size() > j);
            std::pair<std::size_t, std::size_t> expectedEdge = iEdges.at(i).at(j);
            assert(expectedEdge.first == e.src);
            assert(expectedEdge.second == e.tar);
            j++;
        }
    }
    
    std::cout << "InEdges (Bidirectional): SUCCESS" << std::endl;
}

void bidirectionalAdjacencyList() {
    std::cout << "--- Bidirectional ---" << std::endl;
    testInDegree();
    testInEdges();
}
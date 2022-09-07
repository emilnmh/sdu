#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/topological_sort.hpp"
#include <vector>

using namespace graph;

/**
 * Tests outDegree for directed adjacency list.
 */
void testOutDegree() {
    AdjacencyList<tags::Directed> g(5);

    for (size_t i = 0; i < numVertices(g); i++) {
        assert(outDegree(i, g) == 0);
    }
    
    addEdge(0, 1, g);
    addEdge(0, 4, g);
    addEdge(2, 1, g);
    addEdge(3, 4, g);
    addEdge(1, 4, g);

    std::vector<size_t> outEdgeCounts{2, 1, 1, 1, 0};
    for (size_t i = 0; i < numVertices(g); i++) {
        assert(outDegree(i, g) == outEdgeCounts.at(i));
    }
    
    std::cout << "OutDegree (Directed): SUCCESS" << std::endl;
}

/**
 * Tests outEdges for directed adjacency list.
 */
void testOutEdges() {
    AdjacencyList<tags::Directed> g(5);
    addEdge(0, 1, g);
    addEdge(0, 4, g);
    addEdge(2, 1, g);
    addEdge(3, 4, g);
    addEdge(1, 4, g);

    std::vector<std::vector<std::pair<std::size_t, std::size_t>>> oEdges{{{0, 1}, {0, 4}}, {{1, 4}}, {{2, 1}}, {{3,4}}, {}};
    for (size_t i = 0; i < numVertices(g); i++) {
        AdjacencyList<tags::Directed>::OutEdgeRange oEdgeRange = outEdges(i, g);
        size_t j = 0;
        for (AdjacencyList<tags::Directed>::EdgeDescriptor e : oEdgeRange) {
            assert(oEdges.size() > i);
            assert(oEdges.at(i).size() > j);

            std::pair<std::size_t, std::size_t> expectedEdge = oEdges.at(i).at(j);

            assert(expectedEdge.first == e.src);
            assert(expectedEdge.second == e.tar);
            j++;
        }
    }
    
    std::cout << "OutEdge (Directed): SUCCESS" << std::endl;
}

void incidenceAdjacencyList() {
    std::cout << "--- Incidence ---" << std::endl;
    testOutDegree();
    testOutEdges();
}
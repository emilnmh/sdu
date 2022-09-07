#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/topological_sort.hpp"

using namespace graph;

/**
 * Tests addVertex() for directed adjacency list with NoProp as vertex property.
 */
void testAddVertex() {
    AdjacencyList<tags::Directed> g(10);

    assert(numVertices(g) == 10);

    AdjacencyList<tags::Directed>::VertexDescriptor v = addVertex(g);

    assert(numVertices(g) == 11);
    assert(v == 10);

    std::cout << "addVertex (Directed, NoProp): SUCCESS" << std::endl;
}


/**
 * Tests addEdge() for directed adjacency list with NoProp as edge property.
 */
void testAddEdge() {
    AdjacencyList<tags::Directed> g(10);

    assert(numEdges(g) == 0);

    AdjacencyList<tags::Directed>::EdgeDescriptor e = addEdge(0, 1, g);

    assert(numEdges(g) == 1);
    assert(e.src == 0);
    assert(e.tar == 1);
    assert(e.storedEdgeIdx == 0);

    e = addEdge(5, 9, g);

    assert(numEdges(g) == 2);
    assert(e.src == 5);
    assert(e.tar == 9);
    assert(e.storedEdgeIdx == 1);

    std::cout << "addEdge (Directed, NoProp): SUCCESS" << std::endl;
}

void mutableAdjacencyList() {
    std::cout << "--- Mutable ---" << std::endl;
    testAddVertex();
    testAddEdge();
}
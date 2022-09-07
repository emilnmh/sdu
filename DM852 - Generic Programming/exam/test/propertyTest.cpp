#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/topological_sort.hpp"
#include <vector>

using namespace graph;


/**
 * Tests inDegree for bidirectional adjacency list.
 */
void testVertexProperty() {
    AdjacencyList<tags::Directed, int> g;
    
    AdjacencyList<tags::Directed, int>::VertexDescriptor v1 = addVertex(1, g);
    AdjacencyList<tags::Directed, int>::VertexDescriptor v2 = addVertex(2, g);
    AdjacencyList<tags::Directed, int>::VertexDescriptor v3 = addVertex(3, g);

    assert(1 == g[v1]);
    assert(2 == g[v2]);
    assert(3 == g[v3]);

    g[v1] = 100;

    assert(g[v1] == 100);


    // Is and should be invalid, as the const AdjacencyList shouldn't be able to change the properties.
    // const AdjacencyList<tags::Directed, int> g1(g);
    // AdjacencyList<tags::Directed, int>::VertexRange vRange = vertices(g1);
    // for (AdjacencyList<tags::Directed, int, int>::VertexDescriptor v : vRange) {
    //     g1[v] = 1337;
    // }

    std::cout << "VertexProperty: SUCCESS" << std::endl;
}

void testEdgeProperty() noexcept {
    AdjacencyList<tags::Directed, NoProp, int> g(5);

    addEdge(0, 1, 100, g);
    addEdge(1, 2, 69, g);
    addEdge(2, 0, 420, g);
    addEdge(3, 4, 42, g);
    addEdge(4, 0, 200, g);
    addEdge(4, 3, 399, g);

    std::vector<int> expected{100, 69, 420, 42, 200, 399};
    AdjacencyList<tags::Directed, NoProp, int>::EdgeRange eRange = edges(g);
    size_t i = 0;
    for (AdjacencyList<tags::Directed, NoProp, int>::EdgeDescriptor e : eRange) {
        assert(i < expected.size());
        assert(g[e] == expected.at(i));
        i++;
    }
    
    std::cout << "EdgeProperty: SUCCESS" << std::endl;
}


/**
 * These tests are verifying both MutableProperty and Property.
 */
void propertyAdjacencyList() {
    std::cout << "--- Property ---" << std::endl;
    testVertexProperty();
    testEdgeProperty();
}
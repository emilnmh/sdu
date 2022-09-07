#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/concepts.hpp"

#include "mutableTest.cpp"
#include "incidenceTest.cpp"
#include "bidirectionalTest.cpp"
#include "propertyTest.cpp"
#include "topo.cpp"
#include <array>


// Checking if the different types of adjacency list graphs follow the right criterias. 
static_assert(graph::IncidenceGraph<AdjacencyList<tags::Directed>>);
static_assert(graph::IncidenceGraph<AdjacencyList<tags::Bidirectional>>);
static_assert(!graph::BidirectionalGraph<AdjacencyList<tags::Directed>>);
static_assert(graph::BidirectionalGraph<AdjacencyList<tags::Bidirectional>>);
static_assert(graph::PropertyGraph<AdjacencyList<tags::Directed, int, int>>);
static_assert(!graph::PropertyGraph<AdjacencyList<tags::Directed>>);
static_assert(!graph::PropertyGraph<AdjacencyList<tags::Directed, graph::NoProp, graph::NoProp>>);
static_assert(graph::MutablePropertyGraph<AdjacencyList<tags::Directed, int, int>>);
static_assert(!graph::MutablePropertyGraph<AdjacencyList<tags::Directed>>);
static_assert(!graph::MutablePropertyGraph<AdjacencyList<tags::Directed, graph::NoProp, graph::NoProp>>);


// A non moveable object.
struct NonMoveable {
    NonMoveable(int x) : v(x) { }
    NonMoveable(const NonMoveable&) = delete;
    NonMoveable(NonMoveable&&) = delete;
    std::array<int, 1024> arr;
    int v;
};

// Testing for copyable and movable.
static_assert(std::movable<AdjacencyList<tags::Directed>>);
static_assert(std::move_constructible<AdjacencyList<tags::Directed>>);
static_assert(std::movable<AdjacencyList<tags::Bidirectional>>);
static_assert(std::move_constructible<AdjacencyList<tags::Bidirectional>>);
// static_assert(!std::movable<AdjacencyList<tags::Directed, NonMoveable>>); // Fails because it is required that the type is moveable.
// static_assert(!std::move_constructible<AdjacencyList<tags::Directed, NonMoveable>>); // Fails because it is required that the type is moveable.
static_assert(std::movable<AdjacencyList<tags::Directed, int>>);
static_assert(std::move_constructible<AdjacencyList<tags::Directed, int>>);

// Running all the tests.
int main() {
    mutableAdjacencyList();
    incidenceAdjacencyList();
    bidirectionalAdjacencyList();
    propertyAdjacencyList();
    topoSort();
}
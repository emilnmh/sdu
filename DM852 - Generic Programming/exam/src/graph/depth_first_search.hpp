#ifndef GRAPH_DEPTH_FIRST_SEARCH_HPP
#define GRAPH_DEPTH_FIRST_SEARCH_HPP
#include "traits.hpp"
#include <vector>

namespace graph {

struct DFSNullVisitor {
	template<typename G, typename V>
	void initVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void startVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void discoverVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void finishVertex(const V&, const G&) { }	

	template<typename G, typename E>
	void examineEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void treeEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void backEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void forwardOrCrossEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void finishEdge(const E&, const G&) { }	
};

namespace detail {

enum struct DFSColour {
	White, Grey, Black
};

template<typename Graph, typename Visitor>
void dfsVisit(const Graph &g, Visitor visitor, typename Traits<Graph>::VertexDescriptor u,
		std::vector<DFSColour> &colour) {
	using EdgeDescriptor = typename Graph::EdgeDescriptor;
	using VertexDescriptor = typename Graph::VertexDescriptor;
	using OutEdgeRange = typename Graph::OutEdgeRange;
	
	visitor.discoverVertex(u, g);
	colour[u] = DFSColour::Grey;

	OutEdgeRange oEdges = outEdges(u, g);
	for (EdgeDescriptor e : oEdges) {
		VertexDescriptor v = getIndex(e.tar, g);
		visitor.examineEdge(e, g);
		if (colour[v] == DFSColour::White) {
			visitor.treeEdge(e, g);
			dfsVisit(g, visitor, v, colour);
		} else if (colour[v] == DFSColour::Grey) {
			visitor.backEdge(e, g);
		} else if (colour[v] == DFSColour::Black) {
			visitor.forwardOrCrossEdge(e, g);
		}
	}
	colour[u] = DFSColour::Black;
	visitor.finishVertex(u, g);
}

} // namespace detail
template<typename Graph, typename Visitor>
void dfs(const Graph &g, Visitor visitor) {
	using VertexRange = typename Graph::VertexRange;
	using VertexDescriptor = typename Graph::VertexDescriptor;

	std::vector<detail::DFSColour> colour(numVertices(g));
	VertexRange vRange = vertices(g);
	for (VertexDescriptor u : vRange) {
		colour[u] = detail::DFSColour::White;
		visitor.initVertex(u, g);
	}

	for (VertexDescriptor u : vRange) {
		if (colour[u] == detail::DFSColour::White) {
			visitor.startVertex(u, g);
			dfsVisit(g, visitor, u, colour);
		}
	}

}

} // namespace graph
#endif // GRAPH_DEPTH_FIRST_SEARCH_HPP
#ifndef GRAPH_ADJACENCY_LIST_HPP
#define GRAPH_ADJACENCY_LIST_HPP
#include "tags.hpp"
#include "traits.hpp"
#include "properties.hpp"
#include "concepts.hpp"
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <cassert>
#include <list>
#include <vector>
#include <iostream>
#include <type_traits>

namespace graph {

template<typename DirectedCategoryT,
         typename VertexPropT = NoProp,
         typename EdgePropT = NoProp>
	requires std::copyable<VertexPropT> && std::copyable<EdgePropT> && std::default_initializable<VertexPropT>
struct AdjacencyList {
private:
	struct OutEdge {
		OutEdge(std::size_t index) : storedEdgeIdx(index) {}
		std::size_t storedEdgeIdx;
	};

	using OutEdgeList = std::vector<OutEdge>;
	
	struct StoredVertexSimple {
		StoredVertexSimple() = default;
		StoredVertexSimple(VertexPropT data) : data(data) {}
		OutEdgeList eOut;
		VertexPropT data;
	};

	struct InEdge {
		InEdge(std::size_t index) : storedEdgeIdx(index) {}
		std::size_t storedEdgeIdx;
	};

	using InEdgeList = std::vector<InEdge>;
	
	struct StoredVertexComplex : StoredVertexSimple {
		InEdgeList eIn;
	};

	using StoredVertex = typename std::conditional_t<std::is_same<DirectedCategoryT, tags::Directed>::value, StoredVertexSimple, StoredVertexComplex>;

	struct StoredEdge {
		StoredEdge() = default;
		StoredEdge(EdgePropT data) : data(data) {}
		std::size_t src, tar;
		EdgePropT data; 
	};

	using VList = std::vector<StoredVertex>;
	using EList = std::vector<StoredEdge>;
public: // Graph
	using DirectedCategory = DirectedCategoryT;
	using VertexDescriptor = std::size_t;

	struct EdgeDescriptor {
		EdgeDescriptor() = default;
		EdgeDescriptor(std::size_t src, std::size_t tar,
		               std::size_t storedEdgeIdx)
			: src(src), tar(tar), storedEdgeIdx(storedEdgeIdx) {}
	public:
		std::size_t src, tar;
		std::size_t storedEdgeIdx;
	public:
		friend bool operator==(const EdgeDescriptor &a,
		                       const EdgeDescriptor &b) {
			return a.storedEdgeIdx == b.storedEdgeIdx;
		}
	};

public: // PropertyGraph
	using VertexProp = VertexPropT;
	using EdgeProp = EdgePropT;
public: // VertexListGraph


	struct VertexRange {
		// the iterator is simply a counter that returns its value when
		// dereferenced
		using iterator = boost::counting_iterator<VertexDescriptor>;
	public:
		VertexRange(std::size_t n) : n(n) {}
		iterator begin() const { return iterator(0); }
		iterator end()   const { return iterator(n); }
	private:
		std::size_t n;
	};

public: // EdgeListGraph
	struct EdgeRange {
		// We want to adapt the edge list,
		// so it dereferences to EdgeDescriptor instead of StoredEdge
		using EListIterator = typename EList::const_iterator;

		struct iterator : boost::iterator_adaptor<
				iterator, // because we use CRTP
				EListIterator, // the iterator we adapt
				// we want to convert the StoredEdge into an EdgeDescriptor:
				EdgeDescriptor,
				// we can use RA as the underlying iterator supports it:
				std::random_access_iterator_tag,
				// when we dereference we return by value, not by reference
				EdgeDescriptor
			> {
			using Base = boost::iterator_adaptor<
				iterator, EListIterator, EdgeDescriptor,
				std::random_access_iterator_tag, EdgeDescriptor>;
		public:
			iterator() = default;
			iterator(EListIterator i, EListIterator first) : Base(i), first(first) {}
		private:
			// let the Boost machinery use our methods:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				// get our current position stored in the
				// boost::iterator_adaptor base class
				const EListIterator &i = this->base_reference();
				return EdgeDescriptor{i->src, i->tar,
					static_cast<std::size_t>(i - first)};
			}
		private:
			EListIterator first;
		};

	public:
		EdgeRange(const AdjacencyList &g) : g(&g) {}

		iterator begin() const {
			return iterator(g->eList.begin(), g->eList.begin());
		}

		iterator end() const {
			return iterator(g->eList.end(), g->eList.begin());
		}
	private:
		const AdjacencyList *g;
	};


public: // IncidenceGraph
	struct OutEdgeRange {
		using OutEdgeIterator = typename OutEdgeList::const_iterator;
		struct iterator : boost::iterator_adaptor<
				iterator,
				OutEdgeIterator,
				EdgeDescriptor,
				std::bidirectional_iterator_tag,
				EdgeDescriptor
			> {

			using Base = boost::iterator_adaptor<
				iterator, OutEdgeIterator, EdgeDescriptor,
				std::bidirectional_iterator_tag, EdgeDescriptor>;
			
		public:
			// Constructor.
			iterator() = default;
			iterator(OutEdgeIterator i, const EList *eList) : Base(i), eList(eList) {}
		private:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				const OutEdgeIterator &i = this->base_reference(); // Stored edge index 
				return EdgeDescriptor{
					(*eList)[i->storedEdgeIdx].src,
					(*eList)[i->storedEdgeIdx].tar,
					i->storedEdgeIdx
				};
			}

		private:
			const EList *eList;
		};
	public:
		/**
		 * Constructor.
		 * 
		 * @param eList List of edges in graph.
		 * @param outEdgeList List of out-edges of vertex.
		 */
		OutEdgeRange(const EList &eList, const OutEdgeList &outEdgeList) : eList(&eList), outEdgeList(&outEdgeList) {}
		
		/**
		 * Returns the first edge.
		 * 
		 * @return iterator 
		 */
		iterator begin() const {
			return iterator(outEdgeList->begin(), eList);
		}

		/**
		 * Returns the past-end edge.
		 * 
		 * @return iterator 
		 */
		iterator end() const {
			return iterator(outEdgeList->end(), eList);
		}

	private:
		const EList *eList;
		const OutEdgeList *outEdgeList;
	};

public: // BidirectionalGraph

	/**
	 * Iterator for all the in-edges.
	 */
	struct InEdgeRange {
		using InEdgeIterator = typename InEdgeList::const_iterator;
		struct iterator : boost::iterator_adaptor<
				iterator,
				InEdgeIterator,
				EdgeDescriptor,
				std::bidirectional_iterator_tag,
				EdgeDescriptor
			> {

			using Base = boost::iterator_adaptor<
				iterator, InEdgeIterator, EdgeDescriptor,
				std::bidirectional_iterator_tag, EdgeDescriptor>;
			
		public:
			// Constructor.
			iterator() = default;
			iterator(InEdgeIterator i, const EList *eList) : Base(i), eList(eList) {}
		private:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				const InEdgeIterator &i = this->base_reference(); // Stored edge index 
				return EdgeDescriptor{
					(*eList)[i->storedEdgeIdx].src,
					(*eList)[i->storedEdgeIdx].tar,
					i->storedEdgeIdx
				};
			}

		private:
			const EList *eList;
		};
	public:
		/**
		 * Construct.
		 * 
		 * @param eList The list of edges in the graph.
		 * @param inEdgeList The list of in-edges of vertex.
		 */
		InEdgeRange(const EList &eList, const InEdgeList &inEdgeList) : eList(&eList), inEdgeList(&inEdgeList) {}
		
		/**
		 * Returns the first edge.
		 * 
		 * @return iterator 
		 */
		iterator begin() const {
			return iterator(inEdgeList->begin(), eList);
		}

		/**
		 * Returns the past-end edge.
		 * 
		 * @return iterator 
		 */
		iterator end() const {
			return iterator(inEdgeList->end(), eList);
		}

	private:
		const EList *eList;
		const InEdgeList *inEdgeList;
	};

public:
	/**
	 * Constructor
	 */
	AdjacencyList() = default;


	/**
	 * Constructor
	 * 
	 * @param n The number of vertixes that the graph should consists of.
	 */
	AdjacencyList(std::size_t n) : vList(n) {}


	/**
	 * Copy constructor.
	 * 
	 * @param g The graph which should be copied from.
	 */
	AdjacencyList(const AdjacencyList &g) {
		copyVertices(g);
		copyEdges(g);
	}

private: // Helper copy functions.

	/**
	 * Copies without property.
	 * 
	 * @param g Graph to copy from.
	 */
	void copyVertices(const AdjacencyList &g) {
		vList = VList();
		VertexRange vRange = vertices(g);
		for (VertexDescriptor v : vRange) {
			addVertex(*this);
		}
	}


	/**
	 * Copies with property.
	 * 
	 * @param g Graph to copy from.
	 */
	void copyVertices(const AdjacencyList &g) requires (!std::same_as<VertexProp, NoProp>) {
		vList = VList();
		VertexRange vRange = vertices(g);
		for (VertexDescriptor v : vRange) {
			addVertex(g[v], *this);
		}
	}


	/**
	 * Copies without property.
	 * 
	 * @param g Graph to copy from.
	 */
	void copyEdges(const AdjacencyList &g) {
		eList = EList();
		EdgeRange eRange = edges(g);
		for (EdgeDescriptor e : eRange) {
			addEdge(e.src, e.tar, *this);
		}
	}


	/**
	 * Copies with property.
	 * 
	 * @param g Graph to copy from.
	 */
	void copyEdges(const AdjacencyList &g) requires (!std::same_as<EdgeProp, NoProp>) {
		eList = EList();
		EdgeRange eRange = edges(g);
		for (EdgeDescriptor e : eRange) {
			addEdge(e.src, e.tar, g[e], *this);
		}
	}

public: 

	/**
	 * Move constructor.
	 * 
	 * @param g The graph which should be moved from.
	 */
	AdjacencyList(const AdjacencyList &&g) {
		this->eList = g.eList;
		this->vList = g.vList;
	}


	/**
	 * Copy assignment.
	 * 
	 * @param g 
	 * @return AdjacencyList& 
	 */
	AdjacencyList &operator=(const AdjacencyList &g) {
		copyVertices(g);
		copyEdges(g);
		return *this;
	}


	/**
	 * Move assignement
	 * 
	 * @param g The graph which should be moved from.
	 * @return AdjacencyList& 
	 */
	AdjacencyList &operator=(const AdjacencyList &&g) {
		this->eList = g.eList;
		this->vList = g.vList;
		return *this;
	}
private:
	VList vList; // List of vertices.
	EList eList; // List of edges.
public: // Graph
	/**
	 * Returns the source of edge.
	 * 
	 * @pre e is a valid edge in graph g.
	 * 
	 * @param e EdgeDescriptor for the edge.
	 * @param g The graph.
	 * @return VertexDescriptor of the source vertex.
	 */
	friend VertexDescriptor source(EdgeDescriptor e, const AdjacencyList &g) {
		return e.src;
	}

	/**
	 * Returns the target of edge.
	 * 
	 * @pre e is a valid edge in graph g.
	 * 
	 * @param e EdgeDescriptor for the edge.
	 * @param g The graph.
	 * @return VertexDescriptor of the target vertex.
	 */
	friend VertexDescriptor target(EdgeDescriptor e, const AdjacencyList &g) {
		return e.tar;
	}
public: // VertexListGraph
	/**
	 * Returns the number of vertices in the graph.
	 * 
	 * @param g The graph.
	 * @return std::size_t which is the number of edges.
	 */
	friend std::size_t numVertices(const AdjacencyList &g) {
		return g.vList.size();
	}


	/**
	 * Returns iterator for all the vertices in the graph.
	 * 
	 * @param g The graph.
	 * @return VertexRange all the vertices in the graph.
	 */
	friend VertexRange vertices(const AdjacencyList &g) {
		return VertexRange(numVertices(g));
	}
public: // EdgeListGraph
	/**
	 * Returns the number of edges in the graph.
	 * 
	 * @param g The graph.
	 * @return std::size_t which is the number of edges.
	 */
	friend std::size_t numEdges(const AdjacencyList &g) {
		return g.eList.size();
	}


	/**
	 * Returns iterator for all the edges in the graph.
	 * 
	 * @param g The graph.
	 * @return EdgeRange all the edges in the graph.
	 */
	friend EdgeRange edges(const AdjacencyList &g) {
		return EdgeRange(g);
	}
public: // Other
	/**
	 * Returns the index of vertex. 
	 * 
	 * @pre v is a valid Vertex Descriptor in the graph.
	 * 
	 * @param v The vertex.
	 * @param g The grpah from where the vertex v is from.
	 * @return std::size_t which is the vertex index of v in g.
	 */
	friend std::size_t getIndex(VertexDescriptor v, const AdjacencyList &g) {
		return v;
	}
public: // IncidenceGraph
	/**
	 * Returns iterator over all the out-edges of the vertex.
	 * 
	 * @pre v is a valid Vertex Descriptor in the graph.
	 * 
	 * @param v The vertex.
	 * @param g The graph from where the vertex v is from.
	 * @return OutEdgeRange for all the out-edges of v.
	 */
	friend OutEdgeRange outEdges(VertexDescriptor v, const AdjacencyList &g) {
		return OutEdgeRange(g.eList, g.vList[v].eOut);
	}

	/**
	 * Returns the out-degree of the vertex in the graph.
	 * 
	 * @pre v is a valid Vertex Descriptor in the graph.
	 * 
	 * @param v The VertexDescriptor in the graph.
	 * @param g The graph.
	 * 
	 * @return std::size_t The out-degree of the vertex.
	 */
	friend std::size_t outDegree(const VertexDescriptor v, const AdjacencyList &g) {
		return g.vList[v].eOut.size();
	}

public: // BidirectionalGraph
	/**
	 * Returns iterator over all the in-edges of the vertex.
	 * 
	 * @pre v is a valid Vertex Descriptor in the graph.
	 * 
	 * @param v The vertex.
	 * @param g The graph from where the vertex is from.
	 * @return InEdgeRange for all the in-edges of v.
	 */
	friend InEdgeRange inEdges(VertexDescriptor v, const AdjacencyList &g) requires std::same_as<AdjacencyList::DirectedCategory, tags::Bidirectional> {
		return InEdgeRange(g.eList, g.vList[v].eIn);
	}

	/**
	 * Returns the in-degree of the vertex.
	 * 
	 * @pre v is a valid Vertex Descriptor in the graph.
	 * 
	 * @param v The vertex where the in-degree is wanted.
	 * @param g The graph from which the vertex is part of.
	 * @return size_t which is the in-degree of the vertex.
	 */
	friend size_t inDegree(VertexDescriptor v, const AdjacencyList &g) requires std::same_as<AdjacencyList::DirectedCategory, tags::Bidirectional> {
		return g.vList[v].eIn.size();
	}
	
public: // MutableGraph
	
	/**
	 * Adds a new vertex to the graph.
	 * 
	 * @param g The graph which should have a new vertex added.
	 * @return VertexDescriptor of the added vertex.
	 */
	friend VertexDescriptor addVertex(AdjacencyList &g) {
		StoredVertex v;
		g.vList.push_back(v);
		return VertexDescriptor(g.vList.size() - 1);
	}


	/**
	 * Adds a new edge to the graph.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex.
	 * @param g The graph which the edge should be inserted.
	 * @return EdgeDescriptor 
	 */
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, AdjacencyList &g) {
		typename graph::AdjacencyList<DirectedCategory, VertexProp, EdgeProp>::DirectedCategory c;
		return addEdgeImp(u, v, g, c);
	}

private:
	/**
	 * Implementation for addEdge when the DirectedProperty is Directed and the vertex has a no property.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex.
	 * @param g Graph which the edge should be added to.
	 * @return EdgeDescriptor for the inserted edge
	 */
	friend EdgeDescriptor addEdgeImp(const VertexDescriptor u, const VertexDescriptor v, AdjacencyList &g, tags::Directed) {
		StoredEdge edge;
		edge.src = u;
		edge.tar = v;
		g.eList.push_back(edge);
		// Also add the edge to the vertex.
		g.vList[u].eOut.push_back(OutEdge(g.eList.size() - 1));
		return EdgeDescriptor(u, v, static_cast<std::size_t>(g.eList.size() - 1));
	}


	/**
	 * Implementation for addEdge when the DirectedProperty is Bidirectional and the vertex has a no property.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex.
	 * @param g Graph which the edge should be added to.
	 * @return EdgeDescriptor for the inserted edge
	 */
	friend EdgeDescriptor addEdgeImp(const VertexDescriptor u, const VertexDescriptor v, AdjacencyList &g, tags::Bidirectional) {
		StoredEdge edge;
		edge.src = u;
		edge.tar = v;
		std::size_t idx = g.eList.size();
		g.eList.push_back(edge);
		g.vList[u].eOut.push_back(OutEdge(idx));
		g.vList[v].eIn.push_back(InEdge(idx));
		return EdgeDescriptor(u, v, static_cast<std::size_t>(g.eList.size() - 1));
	}
public: // MutablePropertyGraph
	
	/**
	 * Adds a new vertex to the graph which has labels on the vertices.
	 * 
	 * @param vp Vertex label.
	 * @param g The graph which the vertex should be added to.
	 * @return VertexDescriptor for the added vertex.
	 */
	friend VertexDescriptor addVertex(const VertexProp vp, AdjacencyList &g) requires (!std::same_as<VertexProp, NoProp>) {
		StoredVertex v(vp);
		g.vList.push_back(v);
		return VertexDescriptor(g.vList.size() - 1);
	}


	/**
	 * Adds a new edge to the graph which has labels on the edges.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex.
	 * @param ep Edge label.
	 * @param g The graph which the edge should be added to.
	 * @return VertexDescriptor for the added vertex.
	 */
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, const EdgeProp ep, AdjacencyList &g) requires (!std::same_as<EdgeProp, NoProp>) {
		typename graph::AdjacencyList<DirectedCategory, VertexProp, EdgeProp>::DirectedCategory c;
		return addEdgeImp(u, v, ep, g, c);
	}

private:
	/**
	 * Implementation for addEdge when the DirectedProperty is Directed and the vertex has a property.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex.
	 * @param ep Label for the edge.
	 * @param g Graph which the edge should be added to.
	 * @return EdgeDescriptor for the inserted edge
	 */
	friend EdgeDescriptor addEdgeImp(const VertexDescriptor u, const VertexDescriptor v, const EdgeProp ep, AdjacencyList &g, tags::Directed) requires (!std::same_as<EdgeProp, NoProp>) {
		StoredEdge edge(ep);
		edge.src = u;
		edge.tar = v;
		g.eList.push_back(edge);
		g.vList[u].eOut.push_back(OutEdge(g.eList.size() - 1));
		return EdgeDescriptor(u, v, static_cast<std::size_t>(g.eList.size() - 1));
	}


	/**
	 * Implementation for addEdge when the DirectedProperty is Bidirectional and the vertex has a property.
	 * 
	 * @pre u and v are valid vertex descriptors for g.
	 * 
	 * @param u Source vertex.
	 * @param v Target vertex
	 * @param ep Label for the edge.
	 * @param g Graph which the edge should be added to.
	 * @return EdgeDescriptor for the inserted edge.
	 */
	friend EdgeDescriptor addEdgeImp(const VertexDescriptor u, const VertexDescriptor v, const EdgeProp ep, AdjacencyList &g, tags::Bidirectional) requires (!std::same_as<EdgeProp, NoProp>) {
		StoredEdge edge(ep);
		edge.src = u;
		edge.tar = v;
		std::size_t idx = g.eList.size();
		g.eList.push_back(edge);
		g.vList[u].eOut.push_back(OutEdge(idx));
		g.vList[v].eIn.push_back(InEdge(idx));
		return EdgeDescriptor(u, v, static_cast<std::size_t>(g.eList.size() - 1));
	}
public: // PropertyGraph
	
	/**
	 * Returns label on the vertex.
	 * 
	 * @param e VertexDescriptor that the label should be returned from.
	 * @return EdgeProp& 
	 */
	VertexProp& operator[](VertexDescriptor v) requires (!std::same_as<VertexProp, NoProp>) {
		return vList[v].data;
	}


	/**
	 * Returns label on the vertex.
	 * 
	 * @param e VertexDescriptor that the label should be returned from.
	 * @return const EdgvertexProp& 
	 */
	const VertexProp& operator[](const VertexDescriptor v) const requires (!std::same_as<VertexProp, NoProp>) {
		return vList[v].data;
	}

	
	/**
	 * Returns label on the edge.
	 * 
	 * @param e EdgeDescriptor that the label should be returned from.
	 * @return EdgeProp& 
	 */
	EdgeProp& operator[](EdgeDescriptor e) requires (!std::same_as<EdgeProp, NoProp>) {
		return eList[e.storedEdgeIdx].data;
	}


	/**
	 * Returns const label on the edge.
	 * 
	 * @param e EdgeDescriptor that the label should be returned from.
	 * @return const EdgeProp& 
	 */
	const EdgeProp& operator[](EdgeDescriptor e) const requires (!std::same_as<EdgeProp, NoProp>) {
		return eList[e.storedEdgeIdx].data;
	}

};

} // namespace graph
#endif // GRAPH_ADJACENCY_LIST_HPP
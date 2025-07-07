#pragma once

#include <cmath>
#include <format>
#include <string>
#include <ostream>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "../utils/pair.hpp"
#include "../list/linear_list.hpp"

#include "edge.hpp"
#include "data_structure.hpp"
#include "implementations/adjacency_list.hpp"
#include "implementations/adjacency_matrix.hpp"
#include "implementations/fast_adjacency_list.hpp"
#include "implementations/fast_adjacency_list.hpp"
#include "implementations/adjacency_matrix_pointers.hpp"

// Custom hash specialization for std::pair
namespace std {
	template <>
	struct hash<Pair<Vertex, Vertex>> {
		size_t operator()(const Pair<Vertex, Vertex>& p) const {
			return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
		}
	};
}

/**
 * @brief Constructs a graph with the specified structure, size, and properties.
 *
 * @param choice The data structure to be used for graph representation.
 * @param n Number of vertices.
 * @param m Number of edges.
 * @param directed Whether the graph is directed.
 * @param weighted Whether the graph is weighted.
 */
class Graph {

    public:

	enum DataStructures {
		AdjacencyMatrix, AdjacencyMatrixPointers,
		AdjacencyList, FastAdjacencyList
	};

	using Prop = Pair<std::string, std::string>;

	private:

	Pair<std::string, std::string> base = {"", ""};

	class EdgeMap : public std::unordered_map<Pair<Vertex, Vertex>, Pair<std::string, std::string>> {
		Pair<std::string, std::string> base = {"", ""};
	public:
		Pair<std::string, std::string> Find(const Pair<Vertex, Vertex>& edge) const {
			auto it = find(edge);
			return it != end() ? it->second : base;
		}
	};

	class VertexMap : public std::unordered_map<Vertex, Pair<std::string, std::string>> {

		Pair<std::string, std::string> base = {"", ""};

		public:

		Pair<std::string, std::string> Find(const Vertex& v) const {
			auto it = find(v);
			return it != end() ? it->second : base;
		}
	};

	class VertexLabel : public std::unordered_map<Vertex, std::string> {

		std::string base = "";

		public:

		std::string Find(const Vertex& v) const {
			auto it = find(v);
			return it != end() ? it->second : base;
		}
	};

	DataStructure* dataStructure;
	bool directed, weighted;
	DataStructures choice;
	EdgeMap edgeMap;
	VertexMap vertexMap;
	VertexLabel vertexLabel;

	Graph(DataStructure* dataStructure, size_t n, bool directed, bool weighted) {

		this->directed = directed;
		this->weighted = weighted;
		this->n = n;
		this->dataStructure = dataStructure;
	}

	DataStructure* dataStructureChoice(DataStructures choice) {

		switch (choice) {
			case AdjacencyMatrix:
				return new class AdjacencyMatrix(n);
			case AdjacencyMatrixPointers:
				return new class AdjacencyMatrixPointers(n);
			case AdjacencyList:
				return new class AdjacencyList;
			case FastAdjacencyList:
				return new class FastAdjacencyList;
			break;
		}

		return nullptr;
	}

	public:

	size_t n, m;

	Graph(DataStructures choice, size_t n, size_t m, bool directed, bool weighted) {

		this->directed = directed;
		this->weighted = weighted;
		this->choice = choice;
		this->n = n;
		this->m = m;

		dataStructure = dataStructureChoice(choice);

		this->dataStructure->directed(directed);
		this->dataStructure->weighted(weighted);
	}

	/**
	 * @brief Copy constructor for the Graph.
	 *
	 * @param clone Graph to be copied.
	 */
	Graph (const Graph& clone) {

		// NOTE: This is the function called by the "return graph;" "context".

		directed = clone.directed;
		weighted = clone.weighted;
		choice = clone.choice;
		n = 0;
		m = 0;

		dataStructure = dataStructureChoice(choice);

		this->dataStructure->directed(directed);
		this->dataStructure->weighted(weighted);

		for (const Vertex& v : clone.vertices()) {
			addVertex(v);
		}

		for (const Edge& e : clone.edges()) {
			addEdge(e);
		}
	}

	/**
	 * @brief Assignment operator for the Graph.
	 *
	 * @param G Graph to be assigned.
	 * @return Reference to this graph after assignment.
	 */
	Graph& operator=(const Graph& G) {

		if (this != &G) {

			directed = G.directed;
			weighted = G.weighted;
			choice = G.choice;
			n = G.n;
			m = G.m;

			delete dataStructure;

			dataStructure = dataStructureChoice(choice);

			dataStructure->directed(directed);
			dataStructure->weighted(weighted);

			for (const Vertex& v : G.vertices()) {
				addVertex(v);
			}

			for (const Edge& e : G.edges()) {
				addEdge(e);
			}
		}

		return *this;
	}

	/**
	 * @brief Clones the current graph's structure without its data.
	 *
	 * @param n Optional new vertex count for the clone.
	 * @return A new Graph with the same structure type and properties.
	 */
	Graph cloneDataStructure(size_t n = 0) const {
		return { choice, n, m, directed, weighted };
	}

	/**
	 * @brief Prints the current underlying data structure.
	 */
	void printDataStructure() { dataStructure->print(); }

	/**
	 * @brief Destructor that deallocates the data structure.
	 */
	~Graph() { delete dataStructure; }


	/**
	 * @brief Adds a vertex to the graph.
	 *
	 * @param v Vertex identifier.
	 */
	void addVertex(const Vertex& v) {
		dataStructure->addVertex(v);
		n++;
	}

	/**
	 * @brief Adds a vertex with associated properties.
	 *
	 * @param v Vertex identifier.
	 * @param props Pair of string properties associated with the vertex.
	 */
	void addVertex(const Vertex& v, const Pair<std::string, std::string>& props) {
		addVertex(v);
		vertexMap.insert({v, props});
	}

	/**
	 * @brief Adds a vertex with a label.
	 *
	 * @param v Vertex identifier.
	 * @param label Label associated with the vertex.
	 */
	void addVertex(const Vertex& v, std::string& label) {
		addVertex(v);
		vertexLabel.insert({v, label});
	}

	/**
	 * @brief Adds a vertex with both label and properties.
	 *
	 * @param v Vertex identifier.
	 * @param label Label for the vertex.
	 * @param props Properties associated with the vertex.
	 */
	void addVertex(const Vertex& v, const std::string& label, const Pair<std::string, std::string>& props) {
		addVertex(v);
		vertexMap.insert({v, props});
		vertexLabel.insert({v, label});
	}

	/**
	 * @brief Retrieves the label of a given vertex.
	 *
	 * @param v Vertex identifier.
	 * @return Label as a string.
	 */
	std::string getLabel(const Vertex& v) const {
		return vertexLabel.Find(v);
	}

	/**
	 * @brief Adds a weighted edge between two vertices.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @param weight Weight of the edge.
	 */
	void addEdge(const Vertex& u, const Vertex& v, const float& weight) {

		if (hasEdge({u, v})) return;

		if (!weighted) {
			throw std::runtime_error("You should not pass the weight of the edge");
		}

		if (!dataStructure->hasVertex(u)) addVertex(u);

		if (!dataStructure->hasVertex(v)) addVertex(v);

		dataStructure->addEdge(u, v, weight);

		if (!directed) dataStructure->addEdge(v, u, weight);

		m++;
	}

	/**
	 * @brief Adds an unweighted edge between two vertices.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 */
	void addEdge(const Vertex& u, const Vertex& v) {

		if (weighted) {
			throw std::runtime_error("You must pass the weight of the edge");
		}

		if (!dataStructure->hasVertex(u)) addVertex(u);

		if (!dataStructure->hasVertex(v)) addVertex(v);

		dataStructure->addEdge(u, v);

		if (!directed) dataStructure->addEdge(v, u);

		m++;
	}

	/**
	 * @brief Adds an edge with associated properties.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @param props Pair of string properties.
	 */
	void addEdge(const Vertex& u, const Vertex& v, const Pair<std::string, std::string>& props) {
		if (hasEdge({u, v})) return;
		addEdge(u, v);
		edgeMap.insert({{u, v}, props});
	}


	/**
	 * @brief Adds an edge to the graph.
	 *
	 * @param e Edge object.
	 */
	void addEdge(const Edge& e) {

		if (hasEdge(e)) return;

		if (!dataStructure->hasVertex(e.u)) addVertex(e.u);

		if (!dataStructure->hasVertex(e.v)) addVertex(e.v);

		if (weighted) {

			dataStructure->addEdge(e.u, e.v, e.weight);

			if (!directed) dataStructure->addEdge(e.v, e.u, e.weight);
		}

		else {

			dataStructure->addEdge(e.u, e.v);

			if (!directed) dataStructure->addEdge(e.v, e.u);
		}

		m++;
	}

	/**
	 * @brief Adds an edge with properties.
	 *
	 * @param e Edge object.
	 * @param props Properties associated with the edge.
	 */
	void addEdge(const Edge& e, const Pair<std::string, std::string>& props) {
		if (hasEdge(e)) return;
		addEdge(e);
		edgeMap.insert({{e.u, e.v}, props});
	}

	/**
	 * @brief Removes an edge from the graph.
	 *
	 * @param e Edge to be removed.
	 */
	void removeEdge(const Edge& e) {
		dataStructure->removeEdge(e.u, e.v);
		m--;
	}

	/**
	 * @brief Updates the properties of an edge.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @param prop New properties.
	 */
	void changeEdgeProps(const Vertex& u, const Vertex& v, const Prop prop) {
		if (hasEdge(u, v)) {
			edgeMap[{u, v}] = prop;
		}
	}

	/**
	 * @brief Updates the properties of an edge.
	 *
	 * @param e Edge to be updated.
	 * @param prop New properties.
	 */
	void changeEdgeProps(const Edge& e, const Prop prop) {
		if (hasEdge(e.u, e.v)) {
			edgeMap[{e.u, e.v}] = prop;
		}
	}

	/**
	 * @brief Updates the properties of a vertex.
	 *
	 * @param v Vertex to be updated.
	 * @param prop New properties.
	 */
	void changeVertexProps(const Vertex& v, const Prop& prop) {
		// NOTE: There probably should be a check if there is the vertex
		vertexMap[v] = prop;
	}

	/**
	 * @brief Updates the label of a vertex.
	 *
	 * @param v Vertex to be updated.
	 * @param label New label.
	 */
	void changeVertexLabel(const Vertex& v, const std::string& label) {
		vertexLabel[v] = label;
	}

	/**
	 * @brief Updates the weight of an edge.
	 *
	 * @param e Edge to update.
	 * @param weight New weight.
	 */
	void changeEdgeWeight(const Edge& e, const float& weight) {
		dataStructure->changeEdgeWeight(e, weight);
	}

	/**
	 * @brief Updates the weight of an edge.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @param weight New weight.
	 */
	void changeEdgeWeight(const Vertex& u, const Vertex& v, const float& weight) {
		dataStructure->changeEdgeWeight({ u, v }, weight);
	}

	/**
	 * @brief Retrieves an edge object.
	 *
	 * @param e Edge to retrieve.
	 * @return The edge object from the structure.
	 */
	Edge getEdge(const Edge& e) const {
		return dataStructure->getEdge(e);
	}

	/**
	 * @brief Retrieves an edge object.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @return The edge object from the structure.
	 */
	Edge getEdge(const Vertex& u, const Vertex& v) const {
		return dataStructure->getEdge({u, v});
	}

	/**
	 * @brief Checks if an edge exists in the graph.
	 *
	 * @param e Edge to check.
	 * @return True if exists, false otherwise.
	 */
	bool hasEdge(const Edge& e) const {
		return dataStructure->hasEdge(e.u, e.v);
	}

	/**
	 * @brief Checks if an edge exists in the graph.
	 *
	 * @param u Source vertex.
	 * @param v Destination vertex.
	 * @return True if exists, false otherwise.
	 */
	bool hasEdge(const Vertex& u, const Vertex& v) const {
		return dataStructure->hasEdge(u, v);
	}

	/**
	 * @brief Returns the degree of a vertex.
	 *
	 * @param v Vertex identifier.
	 * @return Number of neighbors.
	 */
	size_t degree(const Vertex& v) const {
		return dataStructure->degree(v);
	}

	/**
	 * @brief Retrieves a list of all edges.
	 *
	 * @return List of edges.
	 */
	LinearList<Edge> edges() const { return dataStructure->edges(); }


	/**
	 * @brief Retrieves a list of all vertices.
	 *
	 * @return List of vertices.
	 */
	LinearList<Vertex> vertices() const {
		return dataStructure->vertices();
	}

	/**
	 * @brief Retrieves a list of neighboring vertices and weights.
	 *
	 * @param u Vertex identifier.
	 * @return List of pairs (neighbor, weight).
	 */
	LinearList<Pair<Vertex, float>> edgesOf(const Vertex& u) const {
		return dataStructure->neighbors(u);
	}

	/**
	 * @brief Retrieves a list of neighboring vertices.
	 *
	 * @param u Vertex identifier.
	 * @return List of neighbors.
	 */
	LinearList<Vertex> neighbors(const Vertex& u) const {

		LinearList<Vertex> _neighbors;

		for (Pair<Vertex, float> p : dataStructure->neighbors(u)) {
			_neighbors.push_back(p.first);
		}

		return _neighbors;
	}

	/**
	 * @brief Returns the list of neighbors of a vertex.
	 *
	 * @param u Vertex identifier.
	 * @return List of neighbor pairs (vertex, weight).
	 */
	LinearList<Pair<Vertex, float>> operator[](const Vertex& u) {
		return dataStructure->neighbors(u);
	}

	/**
	 * @brief Exports the graph to a PNG file using Graphviz.
	 *
	 * @param filename Output file name (without extension).
	 * @param engine Layout engine (e.g., "dot", "neato").
	 * @param dpi Resolution in dots per inch.
	 */
	void export_to_png(const std::string& filename, const std::string& engine = "dot", size_t dpi = 300) const {

		// NOTE: This is possible to define all nodes and edges at once
		// node [shape=ellipse, style=filled, fillcolor=lightblue];
		// edge [color=gray, style=dotted];

		// 1. Create a DOT file representing the graph
		std::string dotFilename = filename + ".dot";
		std::ofstream dotFile(dotFilename);

		if (!dotFile) {
			throw std::runtime_error("Could not create the DOT file.");
		}

		dotFile << (directed ? "digraph" : "graph") << " G {\n";

		dotFile << std::format("    dpi = {};", dpi) << std::endl;

		// for (const auto& [v, propsObject] : vertexMap) {
		for (const Vertex& v : vertices()) {
			// auto [color, line] = propsObject;
			auto [color, line] = vertexMap.Find(v);
			std::string label = vertexLabel.Find(v) == "" ? std::format("{}", v) : vertexLabel.Find(v);
			std::string props = "color = \"" + color + "\", style = \"" + line;
			dotFile << "    " << v << " [ " << props << format("\", label=\"{}\"]", label) << ";\n";
		}

		dotFile << std::endl;

		for (const Edge& e : edges()) {

			if (!directed && e.u > e.v) continue;  // Avoid duplicate edges for undirected graphs

			Prop prop = edgeMap.Find({e.u, e.v});
			auto [color, line] = prop == base && !directed ? edgeMap.Find({e.v, e.u}) : prop;

			std::string props = "color = \"" + color + "\", style = \"" + line;

			dotFile << "    " << e.u << (directed ? " -> " : " -- ") << e.v << " [label=\"" << (weighted ? std::format("{:.6g}", e.weight) : "") << "\", " << props << "\"];\n";
		}

		dotFile << "}\n";
		dotFile.close();

		// 2. Use the system() function to run the Graphviz command
		std::string command = engine + " -Tpng " + dotFilename + " -o " + filename + ".png";

		if (system(command.c_str()) != 0) {
			throw std::runtime_error("Failed to execute Graphviz command.");
		}

		std::cout << "Graph exported successfully to " << filename << ".png\n";
	}

	/**
	 * @brief Converts the graph to a string representation.
	 *
	 * @return Stringified version of the graph.
	 */
	std::string str() const {

		// TODO: Separar as arestas por virgulas
		std::stringstream os;

		for (Vertex& u : vertices()) {

			os << u << ": { ";

			for (auto& [v, w] : edgesOf(u)) {

				if (weighted) {
					os << "(" << v << ", " << w << ") ";
				}

				else os << v << " ";
			}

			os << "}" << std::endl;
		}

		std::string result = os.str();

		return result.empty() ? result : result.substr(0, result.size() - 1);
	}

	friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
		os << g.str();
		return os;
	}

	/**
	 * @brief Computes the graph's density.
	 *
	 * @param precision Number of decimal places in the result.
	 * @return Density of the graph.
	 */
	double density(const int& precision = 3) const {

		double n = this->n;
		double m = this->m;

		double density = m / ((n * n - n) / (directed ? 1 : 2));

		double factor = std::pow(10, precision);
		return std::round(density * factor) / factor;
	}

	/**
	 * @brief Computes the sparsness of the graph.
	 *
	 * @return Sparsness value between 0 and 1.
	 */
	float sparsness(const int& precision = 3) const { return 1 - density(precision); }

	/**
	 * @brief Exports the graph to a binary file.
	 *
	 * @param filename Output file name.
	 */
	void export_to_binary(const std::string& filename) const {

		std::ofstream file(filename, std::ios::binary);

		if (!file) {
			throw std::runtime_error("Não foi possível abrir o arquivo para escrita.");
		}

		// Salva configurações do grafo
		uint8_t ds_type = static_cast<uint8_t>(this->choice); // Supondo que você armazene essa informação em um membro
		bool is_directed = this->directed;
		bool is_weighted = this->weighted;

		file.write(reinterpret_cast<char*>(&ds_type), sizeof(uint8_t));
		file.write(reinterpret_cast<char*>(&is_directed), sizeof(bool));
		file.write(reinterpret_cast<char*>(&is_weighted), sizeof(bool));

		// Salva número de vértices e arestas
		uint32_t vertex_count = static_cast<uint32_t>(n);
		uint32_t edge_count = static_cast<uint32_t>(m);

		file.write(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
		file.write(reinterpret_cast<char*>(&edge_count), sizeof(uint32_t));

		// Salva arestas
		for (const Edge& e : edges()) {
			uint32_t u = static_cast<uint32_t>(e.u);
			uint32_t v = static_cast<uint32_t>(e.v);
			file.write(reinterpret_cast<char*>(&u), sizeof(uint32_t));
			file.write(reinterpret_cast<char*>(&v), sizeof(uint32_t));
		}

		file.close();
	}

	/**
	 * @brief Imports a graph from a binary file.
	 *
	 * @param filename Input file name.
	 * @return Loaded Graph object.
	 */
	static Graph import_from_binary(const std::string& filename) {

		std::ifstream file(filename, std::ios::binary);

		if (!file) {
			throw std::runtime_error("Não foi possível abrir o arquivo para leitura.");
		}

		// Lê configurações do grafo
		uint8_t ds_type_raw;
		bool directed, weighted;

		file.read(reinterpret_cast<char*>(&ds_type_raw), sizeof(uint8_t));
		file.read(reinterpret_cast<char*>(&directed), sizeof(bool));
		file.read(reinterpret_cast<char*>(&weighted), sizeof(bool));

		Graph::DataStructures ds_type = static_cast<Graph::DataStructures>(ds_type_raw);

		// Lê número de vértices e arestas
		uint32_t vertex_count, edge_count;
		file.read(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&edge_count), sizeof(uint32_t));

		Graph G(ds_type, 0, 0, directed, weighted);

		for (int i = 0; i < vertex_count; i++) {
			G.addVertex(i);
		}

		// Lê arestas
		for (uint32_t i = 0; i < edge_count; i++) {
			uint32_t u, v;
			file.read(reinterpret_cast<char*>(&u), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&v), sizeof(uint32_t));
			G.addEdge(u, v);
		}

		file.close();
		return G;
	}
};

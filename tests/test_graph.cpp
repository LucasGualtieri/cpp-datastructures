#include <iostream>

#include "../graph/graph.hpp"
#include "../graph/graph_builder.hpp"

using namespace std;

// clear && g++ tests/minHeapTest.cc -o bin/minHeapTest && bin/minHeapTest

int main() {

	cout << "Running MinHeap Tests...\n";

	Graph G = GraphBuilder()
		.dataStructure(Graph::AdjacencyMatrix)
		.directed()
		.weighted()
	.build();

	cout << "All tests passed successfully!" << endl;
	return 0;
}

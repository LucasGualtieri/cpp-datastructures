#include <iostream>

#include "../graph/graph.hpp"
#include "../graph/graph_builder.hpp"

using namespace std;

// clear && g++ tests/minHeapTest.cc -o bin/minHeapTest && bin/minHeapTest

int main() {

	cout << "Running Graph Tests...\n";

	Graph G = GraphBuilder()
		.dataStructure<int>(Graph::AdjacencyMatrix)
		.weighted()
		.directed()
	.build();

	// G.addVertex();

	cout << "All tests passed successfully!" << endl;

	return 0;
}

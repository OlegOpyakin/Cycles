#include "../inc/Graph.h"
#include <iostream>

void TestFunc1(){
    Graph graph(5);
    
    // Cycle 1: 0 -> 1 -> 2 -> 0
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 0);
    
    // Cycle 2: 1 -> 3 -> 4 -> 1
    graph.AddEdge(1, 3);
    graph.AddEdge(3, 4);
    graph.AddEdge(4, 1);
    
    // Additional edge creating a larger cycle: 2 -> 4
    graph.AddEdge(2, 4);
    
    auto cycles = graph.findAllCycles();
    std::cout << "Found " << cycles.size() << " cycles in the graph:\n\n";
    
    for (size_t i = 0; i < cycles.size(); ++i) {
        std::cout << "Cycle " << i + 1 << ": ";
        for (size_t j = 0; j < cycles[i].size(); ++j) {
            std::cout << cycles[i][j];
            if (j < cycles[i].size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    
    graph.DumpGraph("test_cycles", "CycleAnalysis");
}

void TestFunc2(){
    Graph graph(4);

    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);
    graph.AddEdge(3, 2);

    graph.TraceReachabaleValues();
    graph.DumpQuasiInvariants();
}

void TestFunc3(){
    Graph graph(10);

    graph.GenerateEdges(20);

    graph.TraceReachabaleValues();
    graph.DumpQuasiInvariants();
    graph.DumpGraph("graph");
}

int main() {
    TestFunc1();
    
    return 0;
}

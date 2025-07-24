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
    Graph graph1(8);
    Graph graph2(10);
    Graph graph3(10);
    Graph graph4(12);


    graph1.GenerateEdges(14);
    graph1.TraceReachabaleValues();
    graph1.DumpQuasiInvariants();
    graph1.DumpGraph("Example1", "Example1");

    graph2.GenerateEdges(13);
    graph2.TraceReachabaleValues();
    graph2.DumpQuasiInvariants();
    graph2.DumpGraph("Example2", "Example2");

    graph3.GenerateEdges(14);
    graph3.TraceReachabaleValues();
    graph3.DumpQuasiInvariants();
    graph3.DumpGraph("Example3", "Example3");

    graph4.GenerateEdges(17);
    graph4.TraceReachabaleValues();
    graph4.DumpQuasiInvariants();
    graph4.DumpGraph("Example4", "Example4");
}

void TestFunc3(){
    Graph graph(10);

    graph.GenerateEdges(14);
    graph.TraceReachabaleValues();
    graph.DumpQuasiInvariants();
    graph.DumpGraph("Example", "Example");
}

int main() {
    //TestFunc1();
    TestFunc3();
    return 0;
}

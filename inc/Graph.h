#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <random>
#include "BasicBlock.h"

#pragma once

using Cycle = std::vector<size_t>;
using AdjacencyList = std::vector<std::vector<size_t>>;

class Graph{
private:
    /*
    struct DepthInfo{
        size_t depth_;
        bool is_cycle_invariant_;
    };
    */

    std::map<size_t, BasicBlock> blocks_;
    size_t blocks_counter_;
    
    AdjacencyList buildAdjacencyList() const;

    std::unordered_set<size_t> findAllReachableIndicesFor(size_t start, 
                                                          const AdjacencyList& adj) const;
    std::vector<std::unordered_set<size_t>> findAllReachableIndices(const AdjacencyList& adj) const;

    void dfs(size_t startIndex, size_t currentIndex,
             std::unordered_set<size_t>& visited,
             std::vector<size_t>& path,
             const std::vector<std::unordered_set<size_t>>& reachable,
             std::vector<Cycle>& result,
             const AdjacencyList& adj) const;

    void findCycles(size_t startIndex,
                    const AdjacencyList& adj,
                    const std::vector<std::unordered_set<size_t>>& reachable,
                    std::vector<Cycle>& result) const;

    void InvestigateCycleReachability(size_t id); 
            // find all values, that are reachable from the cycle
    
    size_t InvestigateDepth(size_t id, size_t depth) const; 

    bool InvestigateLoopInvariant(size_t id) const; 

public:
    Graph(): blocks_counter_(0) {};
    Graph(size_t graph_size): blocks_counter_(0) {
        for(; blocks_counter_ < graph_size; ++blocks_counter_) {
            BasicBlock block;
            blocks_.insert(std::pair(blocks_counter_,  block));
        }
    }

    void AddBlock(){
        ++blocks_counter_;
        BasicBlock block;
        blocks_.insert(std::pair(blocks_counter_,  block));
    }

    void AddEdge(size_t edge_start, size_t edge_end){
        blocks_[edge_start].AddSucc(edge_end);
        blocks_[edge_end].AddPred(edge_start);
    }

    std::map<size_t, BasicBlock> GetMap() const{
        return blocks_;
    }

    size_t GetBlocksNumber() const{
        return blocks_counter_;
    }

    void GenerateEdges(size_t num_of_edges); // randomly generate edges

    void DumpGraph(const std::string& filename = "graph", 
                   const std::string& graph_name = "CFG"); // dump graph with graphvis

    std::vector<Cycle> findAllCycles();

    void TraceReachabaleValues();

    size_t GetQuasiInvariantDepth(const size_t id) const;

    bool GetLoopInvariant(const size_t id) const;
    
    void DumpQuasiInvariants();
};

/*
Добрый день, хотео задать вопрос по заданию из 7 лекции про VDG.
Я же правильно понимаю, что можно сначала найти в графе все замкнутые пути, 
в том числе и просто связанные блоки. 
Потом определить вершины, достижимые из этих циклов. Оставщиеся вершины -- 
и будут наши квази-инварианты. В конце можно для каждого квази-инварианта 
найти самый дальний достижимый из цикла узел. Его дальность, получается, 
и есть то, на какой итерации его можно вынести
*/
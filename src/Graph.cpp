#include <list>
#include "Graph.h"

#define MY_CONST 16790

AdjacencyList Graph::buildAdjacencyList() const {
    AdjacencyList adj(blocks_counter_);
    for (const auto& [index, block] : blocks_) {
        adj[index] = block.successors_;
    }
    return adj;
}

std::unordered_set<size_t> Graph::findAllReachableIndicesFor(size_t start, const AdjacencyList& adj) const {
    std::unordered_set<size_t> visited;
    std::queue<size_t> q;
    q.push(start);
    visited.insert(start);
    while (!q.empty()) {
        size_t u = q.front();
        q.pop();
        for (size_t v : adj[u]) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                q.push(v);
            }
        }
    }
    return visited;
}

std::vector<std::unordered_set<size_t>> Graph::findAllReachableIndices(const AdjacencyList& adj) const {
    std::vector<std::unordered_set<size_t>> reachable(blocks_counter_);
    for (size_t i = 0; i < blocks_counter_; ++i) {
        reachable[i] = findAllReachableIndicesFor(i, adj);
    }
    return reachable;
}

void Graph::dfs(size_t startIndex, size_t currentIndex,
    std::unordered_set<size_t>& visited,
    std::vector<size_t>& path,
    const std::vector<std::unordered_set<size_t>>& reachable,
    std::vector<Cycle>& result,
    const AdjacencyList& adj) const {
    
    if (currentIndex == startIndex && !visited.empty()) {
        Cycle cycle = path;
        cycle.push_back(startIndex);
        result.push_back(cycle);
        return;
    }
    
    if (visited.find(currentIndex) != visited.end()) {
        return;
    }
    
    if (currentIndex < startIndex || reachable[currentIndex].find(startIndex) == reachable[currentIndex].end()) {
        return;
    }

    visited.insert(currentIndex);
    path.push_back(currentIndex);

    for (size_t neighbor : adj[currentIndex]) {
        dfs(startIndex, neighbor, visited, path, reachable, result, adj);
    }

    visited.erase(currentIndex);
    path.pop_back();
}

void Graph::findCycles(size_t startIndex,
    const AdjacencyList& adj,
    const std::vector<std::unordered_set<size_t>>& reachable,
    std::vector<Cycle>& result) const {
    std::unordered_set<size_t> visited;
    std::vector<size_t> path;
    dfs(startIndex, startIndex, visited, path, reachable, result, adj);
}

std::vector<Cycle> Graph::findAllCycles() const {
    if (blocks_counter_ == 0) return {};

    auto adj = buildAdjacencyList();
    auto reachable = findAllReachableIndices(adj);
    std::vector<Cycle> result;

    for (size_t startIndex = 0; startIndex < blocks_counter_; ++startIndex) {
        findCycles(startIndex, adj, reachable, result);
    }

    return result;
}

// sets the given id's reachable_from_cycle_=true
// recursively calls it's successors
void Graph::InvestigateCycleReachability(size_t id){
    if(blocks_[id].reachable_from_cycle_) return;
    blocks_[id].reachable_from_cycle_ = true;
    for(auto it: blocks_[id].successors_){
        if(blocks_[it].reachable_from_cycle_) continue;
        InvestigateCycleReachability(it);
    }
}

void Graph::TraceReachabaleValues(){
    std::vector<Cycle> cycles = findAllCycles();

    for(Cycle cycle: cycles){
        if(!blocks_[cycle.front()].reachable_from_cycle_){
            InvestigateCycleReachability(cycle.front());
        }
    }
}

size_t Graph::InvestigateDepth(size_t id, size_t depth, size_t current) const{
    if(blocks_.at(id).reachable_from_cycle_){
        return depth;
    }

    if(blocks_.at(id).successors_.empty()) return 0;

    std::vector<size_t> depths;
    for(auto it: blocks_.at(id).successors_){
        depths.push_back(InvestigateDepth(it, ++depth, current));
    }
    //for(auto it: blocks_.at(id).predcessors_){
    //    depths.push_back(InvestigateDepth(it, ++depth, current));
    //}
    return *max_element(depths.begin(), depths.end());
}

size_t Graph::GetQuasiInvariantDepth(const size_t id) const{
    return InvestigateDepth(id, 0, id);
}

void Graph::DumpQuasiInvariants() const {
    std::cout << "Dumping unreachable from cycle nodes: \n";
    for(const auto& [block_id, block]: blocks_){
        if(!block.reachable_from_cycle_){
            std::cout << "basic block "<< block_id << ", depth: ";
            size_t depth = GetQuasiInvariantDepth(block_id);
            std::cout << depth << "\n";
        }
    }
    std::cout << "\n";
}

void Graph::GenerateEdges(size_t num_of_edges){
    if (blocks_counter_ == 0) {
        throw std::runtime_error("Cannot generate edges for empty graph!");
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> block_dist(0, blocks_counter_ - 1);
    
    for (auto& [id, block] : blocks_) {
        block.predcessors_.clear();
        block.successors_.clear();
    }
    
    for (size_t i = 0; i < blocks_counter_ - 1; ++i) {
        size_t target = block_dist(gen);
        if(i == target) target++;
        AddEdge(i, target);
        num_of_edges--;
    }
    
    size_t attempts = 0;
    const size_t max_attempts = num_of_edges * 10;
    
    while (num_of_edges > 0 && attempts < max_attempts) {
        size_t from = block_dist(gen);
        size_t to = block_dist(gen);
        
        if (from == to && gen() % 3 == 0) {
            attempts++;
            continue;
        }
        
        bool edge_exists = false;
        for (size_t succ : blocks_[from].successors_) {
            if (succ == to) {
                edge_exists = true;
                break;
            }
        }
        
        if (!edge_exists) {
            AddEdge(from, to);
            num_of_edges--;
        }
        
        attempts++;
    }
    
    blocks_[0].predcessors_.clear();
    
    for (auto& [id, block] : blocks_) {
        if (id != 0) {
            block.successors_.erase(
                std::remove(block.successors_.begin(), block.successors_.end(), 0),
                block.successors_.end()
            );
        }
    }
}


void Graph::DumpGraph(const std::string& filename, const std::string& graph_name){
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "Error opening DOT file\n";
        return;
    }

    dotFile << "digraph " << graph_name << " {\n";
    dotFile << "    rankdir=\"TB\";\n";
    dotFile << "    node [shape=box, style=\"rounded,filled\", fillcolor=\"#e6f3ff\"];\n";
    dotFile << "    edge [color=\"#0066cc\"];\n";
    
    dotFile << "    label=\"Predecessor Tree\";\n";
    dotFile << "    labelloc=\"t\";\n";
    dotFile << "    fontsize=16;\n";
    dotFile << "    fontweight=\"bold\";\n\n";

    for (const auto& [id, block] : blocks_) {
        dotFile << "    block" << id << " [label=\"Block " << id << "\"];\n";
    }
    dotFile << "\n";

    for (const auto& [id, block] : blocks_) {
        for (size_t predId : block.predcessors_) {
            dotFile << "    block" << predId << " -> block" << id << ";\n";
        }
    }

    dotFile << "}\n";
    dotFile.close();
    std::cout << "DOT file generated: " << filename << "\n";
    system(("dot -Tpng " + filename + " -o ../plots/" + filename + ".png").c_str());
}
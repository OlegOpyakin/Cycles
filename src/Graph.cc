#include <list>
#include "Graph.h"

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

std::vector<Cycle> Graph::findAllCycles() {
    if (blocks_counter_ == 0) return {};

    auto adj = buildAdjacencyList();
    auto reachable = findAllReachableIndices(adj);
    std::vector<Cycle> result;

    for (size_t startIndex = 0; startIndex < blocks_counter_; ++startIndex) {
        findCycles(startIndex, adj, reachable, result);
    }

    for(const auto& cycle: result){
        for(const auto& id: cycle){
            blocks_.at(id).cycle_member_ = true; 
        }
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

/*
struct Graph::DepthInfo Graph::InvestigateDepth(size_t id, struct Graph::DepthInfo& depth_info) const{
    if(blocks_.at(id).reachable_from_cycle_){
        return depth_info;
    }

    if(blocks_.at(id).successors_.empty()){
        depth_info.is_cycle_invariant_ = true;
        return depth_info;
    } 

    std::vector<size_t> depths;
    std::vector<bool> is_cycle_invariant;

    for(auto it: blocks_.at(id).predcessors_){
        
        //struct Graph::DepthInfo tmp_depth_info = depth_info;
        //tmp_depth_info.depth_ = tmp_depth_info.depth_ + 1;
        //std::clog << "tmp depth info after init: " << tmp_depth_info.depth_ << "\n";
        

        struct Graph::DepthInfo tmp_depth_info = depth_info;
        ++tmp_depth_info.depth_;
        tmp_depth_info = InvestigateDepth(it, tmp_depth_info);
        is_cycle_invariant.push_back(tmp_depth_info.is_cycle_invariant_);
        //std::clog << "tmp depth info after InvestigateDepth: " << new_tmp_depth_info.depth_ << "\n";
        depths.push_back(tmp_depth_info.depth_);
    }

    size_t max_depth = 0;
    bool is_cycle_invariant_total = true;

    for(auto it: is_cycle_invariant) is_cycle_invariant_total = is_cycle_invariant_total && it;
    max_depth = *std::max_element(depths.begin(), depths.end());

    depth_info.depth_ = max_depth;
    depth_info.is_cycle_invariant_ = is_cycle_invariant_total;

    return depth_info;
}
*/

size_t Graph::InvestigateDepth(size_t id, size_t depth) const{
    if(blocks_.at(id).predcessors_.empty()){
        return depth;
    } 
    
    std::vector<size_t> depths;
    for(auto it: blocks_.at(id).predcessors_){
        size_t tmp_depth = depth;
        depths.push_back(InvestigateDepth(it, ++tmp_depth));
    }

    return *std::max_element(depths.begin(), depths.end());
}

size_t Graph::GetQuasiInvariantDepth(const size_t id) const{
    return InvestigateDepth(id, 0);
}

bool Graph::InvestigateLoopInvariant(size_t id) const{
    if(blocks_.at(id).reachable_from_cycle_){
        return false;
    }

    if(blocks_.at(id).successors_.empty()){
        return true;
    }

    //std::vector<bool> is_loop_invariant_vec;
    bool is_cycle_invariant_total = true;

    for(auto it: blocks_.at(id).successors_){
        //is_loop_invariant_vec.push_back(InvestigateLoopInvariant(it));
        is_cycle_invariant_total = is_cycle_invariant_total && InvestigateLoopInvariant(it);
    }

    return is_cycle_invariant_total;
}

bool Graph::GetLoopInvariant(const size_t id) const{
    return InvestigateLoopInvariant(id);
}

void Graph::DumpQuasiInvariants() {
    std::vector<size_t> depth;
    std::cout << "Dumping unreachable from cycle nodes: \n";
    for(auto& [block_id, block]: blocks_){
        if(!block.reachable_from_cycle_){
            std::cout << "basic block "<< block_id << ", depth: ";
            //block.depth_ = GetQuasiInvariantDepth(block_id);
            block.depth_ = GetQuasiInvariantDepth(block_id);
            block.loop_invariant_ = GetLoopInvariant(block_id);

            if(block.loop_invariant_) std::cout << "Invariant\n";
            else std::cout << block.depth_ << "\n";
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
            if(from == to) continue;
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
    std::ofstream dotFile(filename + ".dot");
    if (!dotFile.is_open()) {
        std::cerr << "Error opening DOT file\n";
        return;
    }

    dotFile << "digraph " << graph_name << " {\n";
    dotFile << "    rankdir=\"TB\";\n";
    dotFile << "    node [shape=box, style=\"rounded,filled\"];\n";
    dotFile << "    edge [color=\"#0066cc\"];\n";
    
    dotFile << "    label=\"Graph\";\n";
    dotFile << "    labelloc=\"t\";\n";
    dotFile << "    fontsize=16;\n";
    dotFile << "    fontweight=\"bold\";\n\n";

    for (const auto& [id, block] : blocks_) {
        dotFile << "    block" << id << " [label=\"Block " << id;
        if (!block.reachable_from_cycle_) {
            if(block.loop_invariant_){
                dotFile << "\\nLoop invariant ";
            }
            else{
                dotFile << "\\nDepth: ";
                dotFile << block.depth_;
            }

        }
        dotFile << "\", fillcolor=\"";
        
        if (block.reachable_from_cycle_) {
            dotFile << "#ffcccc\"];\n";
        } else {
            dotFile << "#e6f3ff\"];\n";
        }
    }
    dotFile << "\n";

    for (const auto& [id, block] : blocks_) {
        for (size_t predId : block.predcessors_) {
            dotFile << "    block" << predId << " -> block" << id;
            if (blocks_[predId].cycle_member_ && block.cycle_member_) {
                dotFile << " [color=\"#ff0000ff\", penwidth=2];\n";
            } else {
                dotFile << " [color=\"#0066cc\"];\n";
            }
        }
    }

    dotFile << "}\n";
    dotFile.close();
    std::cout << "DOT file generated: " << filename << ".dot\n";
    system(("dot -Tpng " + filename + ".dot -o ../plots/" + filename + ".png").c_str());
}

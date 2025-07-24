#include <iostream>
#include <vector>
#include <algorithm>

#pragma once

class BasicBlock{
public:
    bool cycle_member_;
    bool reachable_from_cycle_;
    bool loop_invariant_;
    bool quasi_invariant_;
    size_t depth_;
    std::vector<size_t> predcessors_;
    std::vector<size_t> successors_;

    BasicBlock(): cycle_member_(false), 
                  reachable_from_cycle_(false), 
                  loop_invariant_(false), 
                  quasi_invariant_(false),
                  depth_(0) {}

    void AddPred(size_t pred){
        predcessors_.push_back(pred);
    }

    void AddSucc(size_t succ){
        successors_.push_back(succ);
    }
};

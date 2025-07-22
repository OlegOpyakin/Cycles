#include <iostream>
#include <vector>
#include <algorithm>

#pragma once

class BasicBlock{
public:
    bool reachable_from_cycle_;
    std::vector<size_t> predcessors_;
    std::vector<size_t> successors_;

    BasicBlock(): reachable_from_cycle_(false) {}

    void AddPred(size_t pred){
        predcessors_.push_back(pred);
    }

    void AddSucc(size_t succ){
        successors_.push_back(succ);
    }
};

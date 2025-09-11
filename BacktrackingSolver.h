#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#include "Instance.h"
#include "Solution.h"
#include <set>

class BacktrackingSolver {
private:
    int nodesExplored_ = 0;

public:
    inline int getNodesExplored() const { return nodesExplored_; }
    BacktrackingSolver();
    Solution solve(const Instance& inst);
};

#endif // BACKTRACKING_SOLVER_H
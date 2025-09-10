#ifndef OG_BACKTRACKING_SOLVER_H
#define OG_BACKTRACKING_SOLVER_H

#include "Instance.h"
#include "Solution.h"
#include <set>

class BacktrackingSolver {
private:
  

public:
    BacktrackingSolver();
    Solution solve(const Instance& inst);
};

#endif // BACKTRACKING_SOLVER_H

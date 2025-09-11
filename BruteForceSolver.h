#ifndef BRUTEFORCE_SOLVER_H
#define BRUTEFORCE_SOLVER_H

#include "Instance.h"
#include "Solution.h"

class BruteForceSolver {
private:

public:
    BruteForceSolver();
    Solution solve(const Instance& instance);   
};

#endif // BRUTEFORCE_SOLVER_H

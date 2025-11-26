#ifndef BRUTEFORCE_SOLVER_H
#define BRUTEFORCE_SOLVER_H

#include "Instance.h"
#include "Solution.h"
#include <set>
#include <vector>

class BruteForceSolver {
private:
    void bruteForceRec(int idx, int M, int N, const Instance& instance, std::set<int>& covered, int currentCost, std::vector<int>& currentSelection, int& bestCost, std::vector<int>& bestSelection);

public:
    BruteForceSolver();
    Solution solve(const Instance& instance);
};

#endif // BRUTEFORCE_SOLVER_H

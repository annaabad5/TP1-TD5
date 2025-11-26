#include "BruteForceSolver.h"
#include <iostream>
#include <set>
#include <vector>
#include <limits>

BruteForceSolver::BruteForceSolver() {}

void BruteForceSolver::bruteForceRec(int idx,
                                     int M,
                                     int N,
                                     const Instance& instance,
                                     std::set<int>& covered,
                                     int currentCost,
                                     std::vector<int>& currentSelection,
                                     int& bestCost,
                                     std::vector<int>& bestSelection) {
    // caso base si ya consideramos todos los influencers
    if (idx == M) {
        // verificar si cubrimos todos los segmentos
        if ((int)covered.size() == N && currentCost < bestCost) {
            bestCost = currentCost;
            bestSelection = currentSelection;
        }
        return;
    }

    // NO incluir al influencer idx
    bruteForceRec(idx + 1, M, N, instance, covered,
                  currentCost, currentSelection, bestCost, bestSelection);

    // SI incluir al influencer idx 
    int cost = instance.getInfluencerCost(idx);
    const std::vector<int>& segs = instance.getInfluencerSegments(idx);

    std::set<int> prevCovered = covered;
    covered.insert(segs.begin(), segs.end());

    currentSelection.push_back(idx);
    bruteForceRec(idx + 1, M, N, instance, covered,
                  currentCost + cost, currentSelection, bestCost, bestSelection);
    currentSelection.pop_back();

    // restaurar el estado original (backtrack)
    covered = prevCovered;
}

Solution BruteForceSolver::solve(const Instance& instance) {
    int N = instance.getNumSegments();
    int M = instance.getNumInfluencers();

    int bestCost = std::numeric_limits<int>::max();
    std::vector<int> bestSelection;

    std::set<int> covered;
    std::vector<int> currentSelection;

    bruteForceRec(0, M, N, instance, covered, 0, currentSelection, bestCost, bestSelection);

    Solution bestSolution;
    if (bestCost != std::numeric_limits<int>::max()) {
        bestSolution.setCost(bestCost);
        bestSolution.setSelectedInfluencers(bestSelection);
    } else {
        bestSolution.setCost(-1);
    }

    return bestSolution;
}
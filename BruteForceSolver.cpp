#include "BruteForceSolver.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

BruteForceSolver::BruteForceSolver() {}

Solution BruteForceSolver::solve(const Instance& instance) {
    int N = instance.getNumSegments();
    int M = instance.getNumInfluencers();

    int minCost = -10;
    std::vector<int> mejores;

    for (int mask = 0; mask < (1 << M); ++mask) {
        int totalCost = 0;
        std::set<int> coveredSegments;
        std::vector<int> selectedInfluencers;

        for (int i = 0; i < M; ++i) {
            if (mask & (1 << i)) { // Si el influencer i está seleccionado:
                
                totalCost += instance.getInfluencerCost(i);
                const std::vector<int>& segmentos = instance.getInfluencerSegments(i);
                coveredSegments.insert(segmentos.begin(), segmentos.end());
                selectedInfluencers.push_back(i);

            }
        }

        // Chequea si todos los segmentos están cubiertos
        if (coveredSegments.size() == static_cast<size_t>(N)) {
            if (minCost == -10 || totalCost < minCost) {
                minCost = totalCost;
                mejores = selectedInfluencers;
            }
        }
    }

    
    Solution bestSolution(M); 
    if (minCost != -10) {
        bestSolution.setCost(minCost);
        bestSolution.setSelectedInfluencers(mejores);
    } else {

        bestSolution.setCost(-1);
        bestSolution.setSelectedInfluencers({});
    }
    return bestSolution;
}


#include "Solution.h"
#include <iostream>
#include <algorithm>
#include <set>

Solution::Solution(int numInfluencers) {}

void Solution::addInfluencer(int influencer) {}

void Solution::removeInfluencer(int influencer) {}

bool Solution::containsInfluencer(int influencer) const {
    return false;
}

void Solution::printSolution() const {
    std::cout << "COST=" << cost << std::endl;
    if (!selectedInfluencers.empty()) {
        std::cout << "INFLUENCERS={ ";
        for (size_t i = 0; i < selectedInfluencers.size(); i++) {
            std::cout << selectedInfluencers[i];
            if (i + 1 < selectedInfluencers.size()) std::cout << ", ";
        }
        std::cout << " }" << std::endl;
    } else {
        std::cout << "INFLUENCERS={ }" << std::endl;
    }
    std::cout << "Solucion: " << std::endl;

}

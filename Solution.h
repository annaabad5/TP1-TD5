#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include "Instance.h"

class Solution {
private:
    int cost;
    std::vector<int> selectedInfluencers;

public:

    Solution(int numInfluencers);
    void addInfluencer(int influencer);
    void removeInfluencer(int influencer);
    bool containsInfluencer(int influencer) const;
    void printSolution() const;
    
    void setCost(int c) { cost = c; }
    void setSelectedInfluencers(const std::vector<int>& v) { selectedInfluencers = v; }

    int getCost() const { return cost; }
    const std::vector<int>& getSelectedInfluencers() const { return selectedInfluencers; }
    
    void setChosen(const std::vector<int>& v) { selectedInfluencers = v; }
    const std::vector<int>& getChosen() const { return selectedInfluencers; }
};

#endif // SOLUTION_H

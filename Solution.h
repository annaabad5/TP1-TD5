#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include <set>
#include "Instance.h"

class Solution {
private:
    int _costoTotal;
    std::vector<int> _influencersUsados;
    std::vector<int> _segmentosCount;
    int _coveredCount;

public:
    // Constructors
    Solution();
    Solution(int numSegments);
    
    void addInfluencer(int influencer, int costo, const std::vector<int>& segments);
    bool containsInfluencer(int influencer) const;
    void printSolution() const;

    void setCost(int c) { _costoTotal = c; }
    void setSelectedInfluencers(const std::vector<int>& v) { _influencersUsados = v; }

    int getCost() const { return _costoTotal; }
    const std::vector<int>& getSelectedInfluencers() const { return _influencersUsados; }
    
    // Métodos adicionales necesarios para el backtracking
    int getCoveredCount() const;
    int InfluencerSize() const;
    int getInfluencerPos(int influencer) const;
    void initCover(int N);
};

#endif // SOLUTION_H
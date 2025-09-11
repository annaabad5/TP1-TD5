#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>
#include <limits>

std::vector<int> BacktrackingSolver::buildOrder(const Instance& inst) const {
    std::vector<int> order(M_);
    for (int i = 0; i < M_; ++i) {
        order[i] = i;
    }

    std::sort(order.begin(), order.end(), [&](int a, int b) {
        // ordenar por costo/segmento ascendente (con tie-breaks simples)
        int ca = inst.getInfluencerCost(a);
        int cb = inst.getInfluencerCost(b);
        int sa = std::max(1, (int)inst.getInfluencerSegments(a).size());
        int sb = std::max(1, (int)inst.getInfluencerSegments(b).size());
        long long lhs = 1LL * ca * sb;
        long long rhs = 1LL * cb * sa;
        if (lhs != rhs) 
            return lhs < rhs;
        if (sa != sb)   
            return sa > sb;   // más cobertura primero
        return ca < cb;                    // más barato primero
    });
    return order;
}

std::vector<std::vector<bool>>
BacktrackingSolver::buildSuffixUnion(const Instance& inst, const std::vector<int>& order) const {
    std::vector<std::vector<bool>> suf(M_ + 1, std::vector<bool>(N_ + 1, false));
    for (int k = M_ - 1; k >= 0; --k) {
        suf[k] = suf[k + 1];
        for (int s : inst.getInfluencerSegments(order[k])) {
            if (1 <= s && s <= N_) suf[k][s] = true;
        }
    }
    return suf;
}

void BacktrackingSolver::dfs(const Instance& inst,
                             const std::vector<int>& order,
                             const std::vector<std::vector<bool>>& sufUnion,
                             int idx,
                             int cost,
                             const std::vector<bool>& covered,
                             int coveredCount,
                             std::vector<int>& curSel) {
    // aumento 1 para los nodos
    nodesExplored_++;
    
    // poda por optimalidad
    if (cost >= bestCost_) 
        return;

    // solución completa
    if (coveredCount == N_) {
        bestCost_ = cost;
        bestSel_  = curSel;
        return;
    }

    // sin más influencers
    if (idx == M_) 
        return;

    // poda por factibilidad: si algún segmento faltante no está en la unión de lo que queda, corto
    for (int s = 1; s <= N_; ++s) {
        if (!covered[s] && !sufUnion[idx][s]) 
            return;
    }

    // 1) incluir influencer order[idx]
    {
        int i = order[idx];
        std::vector<bool> covInc = covered;  // copia simple para legibilidad
        int added = 0;
        for (int s : inst.getInfluencerSegments(i)) {
            if (1 <= s && s <= N_ && !covInc[s]) { 
                covInc[s] = true; 
                ++added; 
            }
        }
        curSel.push_back(i);
        dfs(inst, order, sufUnion, idx + 1, cost + inst.getInfluencerCost(i), covInc, coveredCount + added, curSel);
        curSel.pop_back();
    }

    // 2) NO incluir influencer order[idx]
    dfs(inst, order, sufUnion, idx + 1, cost, covered, coveredCount, curSel);
}

Solution BacktrackingSolver::solve(const Instance& instance) {
    N_ = instance.getNumSegments();
    M_ = instance.getNumInfluencers();

    bestCost_ = std::numeric_limits<int>::max();
    bestSel_.clear();
    
    nodesExplored_ = 0;

    auto order    = buildOrder(instance);
    auto sufUnion = buildSuffixUnion(instance, order);

    std::vector<bool> covered(N_ + 1, false);
    std::vector<int> curSel;
    dfs(instance, order, sufUnion, /*idx=*/0, /*cost=*/0, covered, /*coveredCount=*/0, curSel);

    Solution out(M_);
    if (bestCost_ < std::numeric_limits<int>::max()) {
        out.setCost(bestCost_);
        out.setSelectedInfluencers(bestSel_);
    } else {
        out.setCost(std::numeric_limits<int>::max()); // INFEASIBLE
        out.setSelectedInfluencers({});
    }
    return out;
}


#include "BruteForceSolver.h"
#include <limits>
#include <vector>

BruteForceSolver::BruteForceSolver() {}

static bool cubreTodos(const std::vector<int>& usados, const Instance& inst, int N) {
    std::vector<char> covered(N, 0);
    int cnt = 0;
    for (int inf : usados) {
        const auto& segs = inst.getInfluencerSegments(inf);
        for (int s : segs) {
            int idx = (s >= 1 && s <= N) ? s - 1 : s; // normaliza 1..N → 0..N-1
            if (0 <= idx && idx < N && !covered[idx]) {
                covered[idx] = 1;
                if (++cnt == N) return true;
            }
        }
    }
    return cnt == N;
}

Solution BruteForceSolver::solve(const Instance& inst) {
    const int N = inst.getNumSegments();
    const int M = inst.getNumInfluencers();

    int bestCost = std::numeric_limits<int>::max();
    std::vector<int> best;

    for (int mask = 1; mask < (1 << M); ++mask) { // ← empieza en 1 (evita vacío)
        int cost = 0;
        std::vector<int> pick;
        pick.reserve(M);
        for (int i = 0; i < M; ++i) if (mask & (1 << i)) {
            cost += inst.getInfluencerCost(i);
            if (cost >= bestCost) { pick.clear(); break; } // poda simple
            pick.push_back(i);
        }
        if (pick.empty()) continue;
        if (cubreTodos(pick, inst, N) && cost < bestCost) {
            bestCost = cost; best = std::move(pick);
        }
    }

    Solution sol(M);
    if (best.empty()) { sol.setCost(-1); sol.setSelectedInfluencers({}); }
    else { sol.setCost(bestCost); sol.setSelectedInfluencers(best); }
    return sol;
}

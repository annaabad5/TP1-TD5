#include "BacktrackingSolver.h"
#include <vector>
#include <limits>

BacktrackingSolver::BacktrackingSolver() = default;

// Normaliza un ID de segmento a [0..N-1]. Si viene 1..N, convierte a 0..N-1.
static inline int normSeg(int s, int N) {
    if (0 <= s && s < N) return s;         // 0-based ya ok
    if (1 <= s && s <= N) return s - 1;    // 1-based → 0-based
    return -1;                              // fuera de rango
}

// Poda de factibilidad: ¿los faltantes se pueden cubrir con algún j>=idx?
static bool canStillCoverAllMissing(const Instance& inst,
                                    int idx, int N, int M,
                                    const std::vector<char>& covered) {
    // Pre–compute “qué segmentos puede cubrir el resto”
    std::vector<char> canCover(N, 0);
    for (int j = idx; j < M; ++j) {
        const auto& segs = inst.getInfluencerSegments(j);
        for (int s : segs) {
            int t = normSeg(s, N);
            if (t >= 0) canCover[t] = 1;
        }
    }
    for (int t = 0; t < N; ++t) {
        if (!covered[t] && !canCover[t]) return false;
    }
    return true;
}

static void bt(const Instance& inst,
               int i, int N, int M,
               std::vector<char>& covered, int& coveredCnt,
               int currCost, int& bestCost,
               std::vector<int>& pick, std::vector<int>& best,
               int& nodesExplored)
{
    ++nodesExplored;

    // Completo: todos cubiertos
    if (coveredCnt == N) {
        if (currCost < bestCost) {
            bestCost = currCost;
            best = pick;
        }
        return;
    }
    // Sin más influencers o ya peor que el mejor
    if (i == M || currCost >= bestCost) return;

    // Poda de factibilidad
    if (!canStillCoverAllMissing(inst, i, N, M, covered)) return;

    // Opción A: NO tomar i
    bt(inst, i + 1, N, M, covered, coveredCnt, currCost, bestCost, pick, best, nodesExplored);

    // Opción B: tomar i
    const auto& segs = inst.getInfluencerSegments(i);
    std::vector<int> touched;
    touched.reserve(segs.size());
    for (int s : segs) {
        int t = normSeg(s, N);
        if (t >= 0 && !covered[t]) {
            covered[t] = 1; touched.push_back(t); ++coveredCnt;
        }
    }
    pick.push_back(i);
    bt(inst, i + 1, N, M, covered, coveredCnt,
       currCost + inst.getInfluencerCost(i),
       bestCost, pick, best, nodesExplored);
    pick.pop_back();
    for (int t : touched) { covered[t] = 0; --coveredCnt; }
}

Solution BacktrackingSolver::solve(const Instance& inst) {
    const int N = inst.getNumSegments();
    const int M = inst.getNumInfluencers();

    int bestCost = std::numeric_limits<int>::max();
    std::vector<int> best, pick;
    std::vector<char> covered(N, 0);
    int coveredCnt = 0;
    nodesExplored_ = 0;

    bt(inst, 0, N, M, covered, coveredCnt, 0, bestCost, pick, best, nodesExplored_);

    Solution sol(M);
    if (bestCost == std::numeric_limits<int>::max()) {
        sol.setCost(-1);                  // INFACTIBLE → -1, no 0 ni INT_MAX
        sol.setSelectedInfluencers({});
    } else {
        sol.setCost(bestCost);
        sol.setSelectedInfluencers(best);
    }
    return sol;
}

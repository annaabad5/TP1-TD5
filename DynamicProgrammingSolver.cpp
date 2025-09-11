#include "DynamicProgrammingSolver.h"
#include <vector>
#include <limits>
#include <algorithm> 
#include <cstdint>   

// asumimos que los segmentos están numerados 1..N.
//  Cada influencer i tiene un conjunto de segmentos: instance.getInfluencerSegments(i)

Solution DynamicProgrammingSolver::solve(const Instance& instance) {
    const int N = instance.getNumSegments();
    const int M = instance.getNumInfluencers();

    // Caso trivial: sin segmentos, costo 0 y selección vacía.
    if (N <= 0) {
        Solution out(M);
        out.setCost(0);
        out.setSelectedInfluencers({});
        return out;
    }

    // Si N es muy grande (>= 31) el bitmask con int se rompe. En este TP, N es moderado.
    if (N >= 31) {
        // Devolvemos "inviable" por seguridad (o podrías usar uint64_t si tu N lo permite)
        Solution out(M);
        out.setCost(std::numeric_limits<int>::max());
        out.setSelectedInfluencers({});
        return out;
    }

    // Precomputamos la máscara de cobertura de cada influencer.
    std::vector<int> infMask(M, 0);
    for (int i = 0; i < M; ++i) {
        int mask = 0;
        const auto& segs = instance.getInfluencerSegments(i);
        for (int s : segs) {
            if (s >= 1 && s <= N) {
                mask |= (1 << (s - 1)); // segmento 1..N -> bit 0..N-1
            }
        }
        infMask[i] = mask;
    }

    const int STATES = (1 << N);
    const int TARGET = STATES - 1;

    // dp[mask] = costo mínimo para cubrir 'mask'
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<int> dp(STATES, INF);

    // parent[mask] = (mask_previo, influencer_usado) para reconstrucción
    std::vector<std::pair<int,int>> parent(STATES, std::make_pair(-1, -1));

    dp[0] = 0;

    for (int mask = 0; mask < STATES; ++mask) {
        if (dp[mask] == INF) continue;
        const int baseCost = dp[mask];

        for (int i = 0; i < M; ++i) {
            const int nm = mask | infMask[i];
            const int nc = baseCost + instance.getInfluencerCost(i);
            if (nc < dp[nm]) {
                dp[nm] = nc;
                parent[nm] = std::make_pair(mask, i);
            }
        }
    }

    Solution out(M);

    if (dp[TARGET] >= INF) {
        // No hay forma de cubrir todos los segmentos
        out.setCost(std::numeric_limits<int>::max());
        out.setSelectedInfluencers({});
        return out;
    }

    // reconstruye el conjunto de influencers elegidos siguiendo 'parent'
    std::vector<int> pick;
    int cur = TARGET;
    while (cur != 0) {
        const auto pr = parent[cur];
        const int pm = pr.first;
        const int i  = pr.second;
        if (pm < 0 || i < 0) break; // seguridad
        pick.push_back(i);
        cur = pm;
    }

    std::sort(pick.begin(), pick.end());
    out.setCost(dp[TARGET]);
    out.setSelectedInfluencers(pick);
    return out;
}

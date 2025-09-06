from typing import List, Set, Tuple
import math
import time

# Clase para representar una instancia del problema
class Instance:
    def __init__(self, num_segments: int, costs: List[int], segments: List[List[int]]):
        self.num_segments = num_segments
        self.costs = costs
        self.segments = segments  # lista de listas con los segmentos que cubre cada influencer
    
    def load_from_file(self, filename):
        with open(filename, 'r') as f:
            lines = [line.strip() for line in f if line.strip()]
        self.num_segments = int(lines[0])
        M = int(lines[1])
        self.costs = []
        self.segments = []
        for i in range(M):
            parts = lines[2 + i].split(',')
            self.costs.append(int(parts[0]))
            self.segments.append([int(seg) for seg in parts[1:]])

    def getNumSegments(self) -> int:
        return self.num_segments

    def getNumInfluencers(self) -> int:
        return len(self.costs)

    def getInfluencerCost(self, i: int) -> int:
        return self.costs[i]

    def getInfluencerSegments(self, i: int) -> List[int]:
        return self.segments[i]


# Clase para representar una solución
class Solution:
    def __init__(self, M: int):
        self.cost = -1
        self.selected = []

    def setCost(self, c: int):
        self.cost = c

    def setSelectedInfluencers(self, sel: List[int]):
        self.selected = sel

    def __repr__(self):
        return f"Cost: {self.cost}, Influencers: {self.selected}"


# ========================
# 1) Fuerza Bruta
# ========================
def brute_force_solver(instance: Instance) -> Solution:
    N = instance.getNumSegments()
    M = instance.getNumInfluencers()
    minCost = math.inf
    bestSel = []

    for mask in range(1 << M):
        totalCost = 0
        covered = set()
        sel = []
        for i in range(M):
            if mask & (1 << i):
                totalCost += instance.getInfluencerCost(i)
                covered.update(instance.getInfluencerSegments(i))
                sel.append(i)
        if len(covered) == N and totalCost < minCost:
            minCost = totalCost
            bestSel = sel[:]

    sol = Solution(M)
    if minCost < math.inf:
        sol.setCost(minCost)
        sol.setSelectedInfluencers(bestSel)
    return sol


# ========================
# 2) Backtracking con podas
# ========================
def backtracking_solver(instance: Instance) -> Solution:
    N = instance.getNumSegments()
    M = instance.getNumInfluencers()
    minCost = [math.inf]
    bestSel = [[]]

    def backtrack(idx: int, cost: int, covered: Set[int], sel: List[int]):
        # poda por optimalidad
        if cost >= minCost[0]:
            return

        # solución completa
        if len(covered) == N:
            minCost[0] = cost
            bestSel[0] = sel[:]
            return

        if idx == M:
            return

        # opción 1: no incluir
        backtrack(idx + 1, cost, covered, sel)

        # opción 2: incluir
        newCovered = covered | set(instance.getInfluencerSegments(idx))
        sel.append(idx)
        backtrack(idx + 1, cost + instance.getInfluencerCost(idx), newCovered, sel)
        sel.pop()

    backtrack(0, 0, set(), [])

    sol = Solution(M)
    if minCost[0] < math.inf:
        sol.setCost(minCost[0])
        sol.setSelectedInfluencers(bestSel[0])
    return sol


# ========================
# 3) Programación Dinámica (bitmask)
# ========================
def dp_solver(instance: Instance) -> Solution:
    N = instance.getNumSegments()
    M = instance.getNumInfluencers()
    FULL_MASK = (1 << N) - 1

    dp = [math.inf] * (1 << N)
    selected = [[] for _ in range(1 << N)]
    dp[0] = 0

    for mask in range(1 << N):
        if dp[mask] == math.inf:
            continue
        for i in range(M):
            newMask = mask
            for seg in instance.getInfluencerSegments(i):
                newMask |= (1 << (seg - 1))  # cuidado: segmentos arrancan en 1
            newCost = dp[mask] + instance.getInfluencerCost(i)
            if newCost < dp[newMask]:
                dp[newMask] = newCost
                selected[newMask] = selected[mask] + [i]

    sol = Solution(M)
    if dp[FULL_MASK] < math.inf:
        sol.setCost(dp[FULL_MASK])
        sol.setSelectedInfluencers(selected[FULL_MASK])
    return sol



# Supón que tienes las clases Instance, Solution y los métodos brute_force_solver, backtracking_solver, dp_solver definidos

# Carga la instancia (puedes adaptar esto según tu clase)
instance = Instance()
instance.load_from_file("selected_instances/instancia1.txt")  // PONER NOMBRE DE INSTANCIA!!

# Fuerza Bruta
start = time.time()
sol_fb = brute_force_solver(instance)
end = time.time()
print("Solución Fuerza Bruta:", sol_fb)
print("Tiempo:", end - start, "segundos")

# Backtracking
start = time.time()
sol_bt = backtracking_solver(instance)
end = time.time()
print("Solución Backtracking:", sol_bt)
print("Tiempo:", end - start, "segundos")

# Programación Dinámica
start = time.time()
sol_dp = dp_solver(instance)
end = time.time()
print("Solución Programación Dinámica:", sol_dp)
print("Tiempo:", end - start, "segundos")
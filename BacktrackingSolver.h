#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#pragma once
#include "Instance.h"
#include "Solution.h"
#include <set>
#include <vector>

class BacktrackingSolver {
public:
    BacktrackingSolver() = default;
    Solution solve(const Instance& instance);

    //Para obtener los nodos
    inline int getNodesExplored() const { return nodesExplored_; }

private:
    // Estado global simple
    int N_ = 0, M_ = 0;
    int bestCost_ = 0;
    std::vector<int> bestSel_;

    int nodesExplored_ = 0;

    // Utilidades
    std::vector<int> buildOrder(const Instance& inst) const; // heurística costo/#segs.
    //devuelve un std::vector<int> con los índices de 
    //los influencers ordenados por una heurística (ej: costo/segmentos cubiertos).
    //recorre primero a los influencers “más prometedores” 
    //(buen ratio cobertura/costo) aumenta la chance de encontrar rápido una solución barata -> la poda por optimalidad funciona mejor.

    std::vector<std::vector<bool>> buildSuffixUnion(const Instance& inst,
                                                    const std::vector<int>& order) const;

    //construye un arreglo unionSufijo[k] que contiene la unión 
    //de todos los segmentos que pueden cubrirse con los influencers desde order[k] hasta el final.                                                 
    //es la base de la poda por factibilidad: Si en un estado faltan segmentos y ni siquiera 
    //la unión de lo que queda puede cubrirlos -> corto esa rama.
        


    // DFS con podas
    void dfs(const Instance& inst,
             const std::vector<int>& order,
             const std::vector<std::vector<bool>>& sufUnion,
             int idx,
             int cost,
             const std::vector<bool>& covered, // paso por valor en include (copia simple)
             int coveredCount,
             std::vector<int>& curSel);
};
    //es la búsqueda recursiva (backtracking). En cada paso decide:
    //incluir el influencer actual (order[idx]).
    //No incluirlo.
    //Para qué sirve: recorre el espacio de soluciones pero usando las dos podas:
    //Optimalidad: si el costo parcial >= mejor costo -> corto.
    //Factibilidad: si la unión de lo que queda no alcanza para cubrir -> corto.

    #endif // BACKTRACKING_SOLVER_H

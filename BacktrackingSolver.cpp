#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>

BacktrackingSolver::BacktrackingSolver() {}

Solution BacktrackingSolver::solve(const Instance& instance, Solution& sol_parcial, int i, Solution& bestSolution) {
    int bestCost = bestSolution.getCost(); //busco el costo total de la mejor solucion hasta ahora

    //Caso base
    if (i == instance.getNumInfluencers()) { //recorrí todos los influencers
        if (sol_parcial.getCoveredCount() == instance.getNumSegments() && sol_parcial.InfluencerSize() > 0) {
            return sol_parcial;
        } else { //esa rama no sirve
            Solution vacia = Solution();
            vacia.setCost(INT_MAX); //seteo costo infinito a rama a descartar pues infinito nunca será minimo
            return vacia;
        }

    //podas:
    }else if((sol_parcial.getCost() >= bestCost)){ //Optimalidad: si esta sol tiene MAYOR costo total que la mejor solución vista hasta ahora corto
        return bestSolution; //bestSolution sigue siendo la mejor hasta ahora
    }else if (sol_parcial.getCoveredCount() == instance.getNumSegments()){ //Factibilidad: si ya cubrí los N segmentos corto esta rama
        return sol_parcial;
    }
    
    //Caso recursivo
    int costoInfluencer = instance.getInfluencerCost(i);
    const std::vector<int>& segments = instance.getInfluencerSegments(i);
    
    // Opción 1: SIN incluir el influencer
    Solution without = solve(instance, sol_parcial, i+1, bestSolution);
    if (without.getCost() < bestSolution.getCost()){
        bestSolution = without;
    }
    
    // Opción 2: CON incluir el influencer (creamos una copia)
    Solution sol_con_influencer = sol_parcial; //COPIA
    sol_con_influencer.addInfluencer(i, costoInfluencer, segments);
    Solution with = solve(instance, sol_con_influencer, i+1, bestSolution);
    
    if (with.getCost() < bestSolution.getCost()){
        bestSolution = with;
    }

    return bestSolution;

}
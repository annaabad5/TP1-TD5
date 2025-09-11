#include <iostream>
#include <string>
#include "Instance.h"
#include "Solution.h"
#include "BruteForceSolver.h"
#include "BacktrackingSolver.h"
#include "DynamicProgrammingSolver.h"
#include <vector>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <instancia> <algoritmo>" << std::endl;
        std::cout << "Algoritmo: fb para Fuerza Bruta, bt para Backtracking, dp para Programacion Dinamica" << std::endl;
        return 1;
    }

    std::string filename = "selected_instances/" + std::string(argv[1]) + ".txt";
    // Load instance
    Instance instance;
    if (!instance.loadFromFile(filename)) {
        std::cerr << "Error al leer la instancia: " << filename << std::endl;
        return 1;
    }
    std::cout << "=== Instancia cargada correctamente ===" << std::endl;

    std::string algorithm = argv[2];

    if (algorithm == "fb") {
        BruteForceSolver bruteForceSolver;
        Solution bruteForceSolution = bruteForceSolver.solve(instance);
        std::cout << "Solucion de Fuerza Bruta:" << std::endl;
        bruteForceSolution.printSolution();
    }
    else if (algorithm == "bt") {
        BacktrackingSolver backtrackingSolver;
        Solution backtrackingSolution = backtrackingSolver.solve(instance);
        std::cout << "Solucion de Backtracking:" << std::endl;
        backtrackingSolution.printSolution();
        std::cout << "NODES=" << backtrackingSolver.getNodesExplored() << std::endl;
    } 
    else if (algorithm == "dp") {               
        DynamicProgrammingSolver dynamicProgrammingsolver;
        Solution dynamicProgrammingSolution = dynamicProgrammingsolver.solve(instance);
        std::cout << "Solucion de Programacion Dinamica:\n";
        dynamicProgrammingSolution.printSolution(); 
    } else {
        std::cerr << "Algoritmo no reconocido (use fb | bt | dp)\n";
        return 2;
    }
    
    return 0;
}

#include <iostream>
#include <string>
#include <climits>
#include "Instance.h"
#include "Solution.h"
#include "BruteForceSolver.h"
#include "BacktrackingSolver.h"
#include <vector>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <instancia> <algoritmo> [variante] [K]" << std::endl;
        std::cout << "Algoritmo: fb (Fuerza Bruta) o bt (Backtracking)" << std::endl;
        std::cout << "Variante opcional: todos | micro | macro" << std::endl;
        std::cout << "K opcional: umbral para separar micro/macro" << std::endl;
        return 1;
    }

    std::string filename = "selected_instances/" + std::string(argv[1]);
    // Load instance
    Instance instance;
    if (!instance.loadFromFile(filename)) {
        std::cerr << "Error al leer la instancia: " << filename << std::endl;
        return 1;
    }
    std::cout << "=== Instancia cargada correctamente ===" << std::endl;

    // Parsear argumentos 
    std::string algorithm = argv[2];
    std::string variant = (argc >= 4) ? std::string(argv[3]) : "todos";
    int K = (argc >= 5) ? std::stoi(argv[4]) : -1;

    // Aplicar división micro/macro 
    if (variant == "micro" || variant == "macro") {
        if (K <= 0) {
            std::cerr << "Error: se requiere un valor de K positivo para las variantes micro/macro." << std::endl;
            return 1;
        }

        #include <utility>  // arriba, por las dudas
        auto [micro, macro] = instance.splitBySegmentCount(K);  // devuelve par


        if (variant == "micro") {
            instance = micro;
            std::cout << "[Modo MICRO] Influencers con |S_i| < " << K << std::endl;
        } else if (variant == "macro") {
            instance = macro;
            std::cout << "[Modo MACRO] Influencers con |S_i| >= " << K << std::endl;
        }
    } else {
        std::cout << "[Modo TODOS] Se consideran todos los influencers." << std::endl;
    }

    if (algorithm == "fb") {
        BruteForceSolver bruteForceSolver;
        Solution bruteForceSolution = bruteForceSolver.solve(instance);
        std::cout << "Solucion de Fuerza Bruta:" << std::endl;
        bruteForceSolution.printSolution();
    }
    
    if (algorithm == "bt") {
        BacktrackingSolver backtrackingSolver;
        Solution sol_parcial;
        sol_parcial.initCover(instance.getNumSegments());
        Solution bestSolution;
        bestSolution.setCost(INT_MAX);
        Solution backtrackingSolution = backtrackingSolver.solve(instance, sol_parcial, 0, bestSolution);
        std::cout << "Solucion de Backtracking:" << std::endl;
        backtrackingSolution.printSolution();
    }
    
    return 0;
}

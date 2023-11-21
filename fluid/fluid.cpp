#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include "sim/progargs.hpp"
#include "sim/simulacion.hpp"


int main(int argc, char *argv[]) {
    unsigned const tiempo0=clock();
    std::span const args_view{argv, static_cast<std::size_t>(argc)};
    std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};

    Argumentos argumentos;

    Constantes::ErrorCode errorCode = comprobarArgsEntrada(argc, arguments, argumentos);
    if (errorCode != 0) {
        return errorCode;
    }

    // Calcula los valores utilizando la función calculateValues
    Grid malla(Constantes::limInferior, Constantes::limSuperior);
    auto result = malla.simular_malla(argumentos.fluid);
    double const smoothingLength = result.first;
    double const particleMass = result.second;

    std::vector<Block> blocks = ejecutarIteraciones(malla, argumentos, smoothingLength, particleMass);

    errorCode = comprobarArgsSalida(arguments, argumentos, blocks);
    unsigned const tiempo1= clock();
    double const time = (double(tiempo1-tiempo0)/CLOCKS_PER_SEC);
    std::cout << "Execution Time: " << time;
    return errorCode;
}

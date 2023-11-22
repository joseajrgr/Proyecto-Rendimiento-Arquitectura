#include <vector>
#include <string>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include "sim/progargs.hpp"
#include "sim/simulacion.hpp"


int main(int argc, char *argv[]) {
    // Prepara los argumentos para pasarlos al resto de funciones (esto incluye nuestro propio "struct")
    std::span const args_view{argv, static_cast<std::size_t>(argc)};
    std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};
    Argumentos argumentos;

    // Intenta obtener los valores del fichero de entrada (si no puede, devuelve error)
    Constantes::ErrorCode errorCode = comprobarArgsEntrada(argc, arguments, argumentos);
    if (errorCode != Constantes::ErrorCode::NO_ERROR) {
        return static_cast<int>(errorCode);
    }

    // Genera la malla y la simula (con esto obtiene resultados como los bloques o la longitud de suavizado)
    Grid malla(Constantes::limInferior, Constantes::limSuperior);
    auto result = malla.simular_malla(argumentos.fluid);
    double const smoothingLength = result.first;
    double const particleMass = result.second;

    // Se procesan todas las etapas de la simulacion, tantas veces como se haya especificado
    std::vector<Block> blocks = ejecutarIteraciones(malla, argumentos, smoothingLength, particleMass);

    // Intenta escribir el resultado en el fichero de salida (si no puede, devuelve error)
    errorCode = comprobarArgsSalida(arguments, argumentos, blocks);
    return static_cast<int>(errorCode);
}

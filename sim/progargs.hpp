//
// Created by quinc on 03/10/2023.
//

#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

#include "constantes.hpp"
#include "grid.hpp"


struct Argumentos {
    int iteraciones;
    std::string archivoEntrada;
    std::string archivoSalida;
    Fluid fluid;
};


void readFluid(std::ifstream& in, Fluid& fluid);
Constantes::ErrorCode comprobarArgsEntrada(int argc, std::vector<std::string> arguments, Argumentos& argumentos);
Constantes::ErrorCode comprobarArgsSalida(std::vector<std::string> arguments, Argumentos& argumentos);


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

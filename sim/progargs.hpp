#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <span>
#include "progargs.hpp"
#include "sim/constantes.hpp"
#include "sim/grid.hpp"


struct Argumentos {
    int iteraciones;
    std::string archivoEntrada;
    std::string archivoSalida;
    Fluid fluid;
    std::vector<Block> blocks;
};


void leerFluido(std::ifstream& in, Fluid& fluid);
Constantes::ErrorCode comprobarArgsEntrada(int argc, std::vector<std::string> arguments, Argumentos& argumentos);
Constantes::ErrorCode comprobarParticulas(std::vector<std::string> arguments, Argumentos& argumentos);
Constantes::ErrorCode comprobarArgsSalida(std::vector<std::string> arguments, Argumentos& argumentos);


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

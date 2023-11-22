#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <span>
#include "sim/constantes.hpp"
#include "sim/grid.hpp"


// Estructura para almacenar conjuntamente los argumentos
struct Argumentos {
    int iteraciones = 0;
    std::string archivoEntrada;
    std::string archivoSalida;
    Fluid fluid;
};


// Funciones para leer, comprobar y almacenar los valores del fichero de entrada
Constantes::ErrorCode comprobarArgsEntrada(int argc, std::vector<std::string> arguments, Argumentos &argumentos);

Constantes::ErrorCode comprobarParticulas(std::vector<std::string> arguments, Argumentos &argumentos);

Constantes::ErrorCode leerFluido(std::ifstream &in, Fluid &fluid);


// Funcion para comprobar los argumentos relativos a la salida y que esta se puede hacer correctamente
Constantes::ErrorCode
comprobarArgsSalida(std::vector<std::string> arguments, Argumentos &argumentos, std::vector<Block> &blocks);


#endif  // PROYECTO_RENDIMIENTO_ARQUITECTURA_PROGARGS_HPP

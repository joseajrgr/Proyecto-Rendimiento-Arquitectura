#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <span>
#include "progargs.hpp"
#include "sim/constantes.hpp"
#include "sim/grid.hpp"


//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void readFluid(std::ifstream& in, Fluid& fluid) {
    in.read(reinterpret_cast<char*>(&fluid.particlespermeter), sizeof(float));
    in.read(reinterpret_cast<char*>(&fluid.numberparticles), sizeof(int));
    fluid.particles.resize(fluid.numberparticles);
    for (int i = 0; i < fluid.numberparticles; ++i) {
        fluid.particles[i].id = i;
        for (double* attr : {&fluid.particles[i].px, &fluid.particles[i].py, &fluid.particles[i].pz,
                             &fluid.particles[i].hvx, &fluid.particles[i].hvy, &fluid.particles[i].hvz,
                             &fluid.particles[i].vx, &fluid.particles[i].vy, &fluid.particles[i].vz}) {
            float temp = 0;
            in.read(reinterpret_cast<char*>(&temp), sizeof(float));
            *attr = static_cast<double>(temp);
        }
    }
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)


Constantes::ErrorCode comprobarArgs(int argc, std::vector<std::string> arguments, Argumentos& argumentos) {
    // Comprueba el numero de argumentos
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments. Usage: " << arguments.size()
                  << " <nts> <inputfile> <outputfile>\n";
        return Constantes::ErrorCode::INVALID_ARGUMENTS;
    }

    // Comprueba el numero de iteraciones
    try {
        argumentos.iteraciones = std::stoi(arguments[0]);
        if (argumentos.iteraciones < 0) {
            std::cerr << "Error: Invalid number of time steps.\n";
            return Constantes::ErrorCode::INVALID_TIME_STEPS;
        }
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: time steps must be numeric.\n";
        return Constantes::ErrorCode::INVALID_NUMERIC_FORMAT;
    }

    // Comprueba si se puede leer el archivo de entrada
    argumentos.archivoEntrada = arguments[1];
    std::ifstream input(argumentos.archivoEntrada, std::ios::binary);
    argumentos.archivoSalida = arguments[2];
    if (!input) {
        std::cerr << "Error: Cannot open " << argumentos.archivoEntrada << " for reading\n";
        return Constantes::ErrorCode::CANNOT_OPEN_FILE_READING;
    }

    // Comprueba el numero de particulas del archivo de entrada
    readFluid(input, argumentos.fluid);
    if (argumentos.fluid.particles.size() <= 0) {
        std::cerr << "Error: Invalid number of particles: 0.\n";
        return Constantes::ErrorCode::INVALID_PARTICLE_COUNT;
    }

    // Comprueba si el numero de particulas leidas coincide con numberparticles
    if (argumentos.fluid.particles.size() != static_cast<std::vector<Particle>::size_type>(argumentos.fluid.numberparticles)) {
        std::cerr << "Error: Number of particles mismatch. Header: "
                  << argumentos.fluid.numberparticles << ", Found: " << argumentos.fluid.particles.size() << ".\n";
        return Constantes::ErrorCode::INVALID_PARTICLE_COUNT;
    }

    return Constantes::ErrorCode::NO_ERROR;
};

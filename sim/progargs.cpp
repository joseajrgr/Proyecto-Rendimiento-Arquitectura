#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <span>
#include "progargs.hpp"
#include "sim/constantes.hpp"
#include "sim/grid.hpp"


//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void leerFluido(std::ifstream& in, Fluid& fluid) {
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


//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
bool compareParticlesByID(const Particle& a, const Particle& b) {
    return a.id < b.id;
}

void escribirFluido(std::ofstream& out, Fluid& fluid, const std::vector<Block>& blocks) {
    float temp;
    temp = static_cast<float>(fluid.particlespermeter);
    out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
    out.write(reinterpret_cast<const char*>(&fluid.numberparticles), sizeof(int));

    // Vector to store all particles for sorting
    std::vector<Particle> allParticles;

    // Collect all particles from blocks into a single vector
    for (const auto& block : blocks) {
        allParticles.insert(allParticles.end(), block.particles.begin(), block.particles.end());
    }

    // Sort particles by ID
    std::sort(allParticles.begin(), allParticles.end(), compareParticlesByID);

    // Write sorted particles to the output file
    for (auto& particle : allParticles) {
        // Print particle information
//        std::cout << "La partícula " << " " << particle.density << " está en el bloque " << particle.idBloque << " x: "
//                  << particle.px << " y: " << particle.py << " z: " << particle.pz << "    Velocidad: (" << particle.vx << ", "
//                  << particle.vy << ", " << particle.vz << "     Aceleración: (" << particle.ax << ", " << particle.ay << ", "
//                  << particle.az << ")" << std::endl;

        temp = static_cast<float>(particle.px);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.py);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.pz);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.hvx);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.hvy);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.hvz);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.vx);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.vy);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        temp = static_cast<float>(particle.vz);
        out.write(reinterpret_cast<const char*>(&temp), sizeof(float));


    }
}


// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)


Constantes::ErrorCode comprobarArgsEntrada(int argc, std::vector<std::string> arguments, Argumentos& argumentos) {
    // Comprueba el numero de argumentos
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments. Usage: " << arguments.size()
            << " <nts> <inputfile> <outputfile>\n";
        return Constantes::ErrorCode::INVALID_ARGUMENTS;
    }

    try { // Comprueba el numero de iteraciones
        argumentos.iteraciones = std::stoi(arguments[0]);
        if (argumentos.iteraciones < 0) {
            std::cerr << "Error: Invalid number of time steps.\n";
            return Constantes::ErrorCode::INVALID_TIME_STEPS;
        }
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: time steps must be numeric.\n";
        return Constantes::ErrorCode::INVALID_NUMERIC_FORMAT;
    }

    // Por ultimo, revisamos el fichero de entrada y sus particulas (si tampoco da error, devuelve NO_ERROR)
    const Constantes::ErrorCode errorCode = comprobarParticulas(arguments, argumentos);
    if (errorCode != 0) {
        return errorCode;
    }
    return Constantes::ErrorCode::NO_ERROR;
}


Constantes::ErrorCode comprobarParticulas(std::vector<std::string> arguments, Argumentos& argumentos) {
    // Comprueba si se puede leer el archivo de entrada
    argumentos.archivoEntrada = arguments[1];
    std::ifstream input(argumentos.archivoEntrada, std::ios::binary);
    argumentos.archivoSalida = arguments[2];
    if (!input) {
        std::cerr << "Error: Cannot open " << argumentos.archivoEntrada << " for reading\n";
        return Constantes::ErrorCode::CANNOT_OPEN_FILE_READING;
    }

    // Comprueba el numero de particulas del archivo de entrada
    leerFluido(input, argumentos.fluid);
    if (argumentos.fluid.particles.empty()) {
        std::cerr << "Error: Invalid number of particles: 0.\n";
        return Constantes::ErrorCode::INVALID_PARTICLE_COUNT;
    }

    // Comprueba si el numero de particulas leidas coincide con numberparticles
    if (argumentos.fluid.particles.size() != static_cast<std::vector<Particle>::size_type>(argumentos.fluid.numberparticles)) {
        std::cerr << "Error: Number of particles mismatch. Header: "
                  << argumentos.fluid.numberparticles << ", Found: " << argumentos.fluid.particles.size() << ".\n";
        return Constantes::ErrorCode::INVALID_PARTICLE_COUNT;
    }

    return Constantes::NO_ERROR;
}


Constantes::ErrorCode comprobarArgsSalida(std::vector<std::string> arguments, Argumentos& argumentos, std::vector<Block>& blocks) {
    // Comprobar si se puede abrir el fichero de salida
    std::ofstream output(arguments[2], std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot open " << arguments[2] << " for writing\n";
        return Constantes::ErrorCode::CANNOT_OPEN_FILE_WRITING;
    }

    // Escribir el estado final del fluido en el archivo de salida
    escribirFluido(output, argumentos.fluid, blocks);
    output.close();
    std::cout << "Simulación completada. Estado final del fluido guardado en: " << arguments[2] << "\n";
    return Constantes::ErrorCode::NO_ERROR;
}

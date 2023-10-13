#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <span>
#include <cstring>  // Para std::memcpy
#include <array>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"


struct Fluid {
    float particlespermeter = 0;  // Partículas por metro
    int numberparticles = 0;  // Número de partículas
    std::vector<Particle> particles;
};

void mesh_simulation(const Fluid &fluid);

void print_simulation(int iteraciones, const Fluid &fluid);

void readFluid(std::ifstream &in, Fluid &fluid) {
    in.read(static_cast<char *>(static_cast<void *>(&fluid.particlespermeter)), sizeof(fluid.particlespermeter));
    in.read(static_cast<char *>(static_cast<void *>(&fluid.numberparticles)), sizeof(fluid.numberparticles));
    fluid.particles.resize(fluid.numberparticles);
    for (int i = 0; i < fluid.numberparticles; ++i) {
        in.read(static_cast<char *>(static_cast<void *>(&fluid.particles[i])), sizeof(Particle));
    }
}

void writeFluid(std::ofstream &out, const Fluid &fluid) {
    std::array<char, sizeof(fluid.particlespermeter)> ppmBuffer = {0};
    std::memcpy(ppmBuffer.data(), &fluid.particlespermeter, sizeof(fluid.particlespermeter));
    out.write(ppmBuffer.data(), sizeof(fluid.particlespermeter));

    std::array<char, sizeof(fluid.numberparticles)> npBuffer = {0};
    std::memcpy(npBuffer.data(), &fluid.numberparticles, sizeof(fluid.numberparticles));
    out.write(npBuffer.data(), sizeof(fluid.numberparticles));

    for (const auto &particle: fluid.particles) {
        std::array<char, sizeof(Particle)> particleBuffer = {0};
        std::memcpy(particleBuffer.data(), &particle, sizeof(Particle));
        out.write(particleBuffer.data(), sizeof(Particle));
    }
}

// Definir constantes para los códigos de error
const int ERROR_INVALID_PARTICLE_COUNT = -5;

int main(int argc, char *argv[]) {
    std::span const args_view{argv, static_cast<std::size_t>(argc)};
    std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments. Usage: " << arguments.size()
                  << " <nts> <inputfile> <outputfile>\n";
        return -1;
    }
    const int iteraciones = std::stoi(arguments[0]);
    try {
        if (iteraciones < 0) {
            std::cerr << "Error: Invalid number of time steps.\n";
            return -2;
        }
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: time steps must be numeric.\n";
        return -1;
    }

    const std::string &archivoEntrada = arguments[1];
    const std::string &archivoSalida = arguments[2];

    // Leer el archivo de entrada
    std::ifstream input(archivoEntrada, std::ios::binary);
    if (!input) {
        std::cerr << "Error: Cannot open " << archivoEntrada << " for reading\n";
        return -3;
    }

    Fluid fluid;
    readFluid(input, fluid);

    // Verificar si el número de partículas leídas coincide con numberparticles OPCIONAL
    if (fluid.particles.size() != static_cast<std::vector<Particle>::size_type>(fluid.numberparticles)) {
        std::cerr << "Error: El número de partículas leídas (" << fluid.particles.size()
                  << ") no coincide con numberparticles (" << fluid.numberparticles << ").\n";
        return ERROR_INVALID_PARTICLE_COUNT;
    }
    mesh_simulation(fluid);
    //print_simulation(iteraciones, fluid);

    // Escribir el estado final del fluido en el archivo de salida
    std::ofstream output(archivoSalida, std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot open " << archivoSalida << " for writing\n";
        return -4;
    }
    writeFluid(output, fluid);

    output.close();
    std::cout << "Simulación completada. Estado final del fluido guardado en: " << archivoSalida << "\n";

    return 0;
}

void print_simulation(const int iteraciones, const Fluid &fluid) {// Realizar la simulación
    for (int iter = 0; iter < iteraciones; ++iter) {
        std::cout << "Iteración " << iter + 1 << ":\n";
        for (int i = 0; i < fluid.numberparticles; ++i) {
            const Particle &particula = fluid.particles[i];
            // Aquí se puede hacer la simulación para la partícula actual
            std::cout << "Partícula " << i + 1 << ": px=" << particula.px << " py=" << particula.py << " pz="
                      << particula.pz << "\n";

        }
        std::cout << "\n";
    }
}

// Funcion que simula la malla
void mesh_simulation(const Fluid &fluid) {
    const double smoothingLength = Constantes::multRadio / fluid.particlespermeter;
    const double particleMass = std::pow(10.0, 3.0) / std::pow(fluid.particlespermeter, 3.0);

    // Calcula los valores utilizando la función calculateValues
    Grid malla(Constantes::limInferior, Constantes::limSuperior);
    malla.dividirEnBloques(smoothingLength);

    // Mostrar los valores por pantalla en el formato requerido
    std::cout << "Number of particles: " << fluid.numberparticles << "\n";
    std::cout << "Particles per meter: " << fluid.particlespermeter << "\n";
    std::cout << "Smoothing length: " << smoothingLength << "\n";
    std::cout << "Particle mass: " << particleMass << "\n";
    std::cout << "Grid size: " << malla.numberblocksx << " x " << malla.numberblocksy << " x " << malla.numberblocksz << "\n";
    std::cout << "Number of blocks: " << malla.numBlocks << "\n";
    std::cout << "Block size: " << malla.meshx << " x " << malla.meshy << " x " << malla.meshz << "\n";
}

void reposicionarParticulas(const Fluid &fluid, std::vector<Block> &blocks) {
    for (int i = 0; i < fluid.numberparticles; ++i) {
        const Particle &particula = fluid.particles[i];

        // Colocar la partícula en el bloque correspondiente
        for (auto &block : blocks) {

            // AQUI CREO QUE PODEMOS CAMBIARLO POR LA FUNCION particula_en_bloque!!!!
            if (particula.px >= block.minPoint.x && particula.px <= block.maxPoint.x &&
                particula.py >= block.minPoint.y && particula.py <= block.maxPoint.y &&
                particula.pz >= block.minPoint.z && particula.pz <= block.maxPoint.z) {
                block.addParticle(particula);
                break;  // La partícula solo puede estar en un bloque, así que pasamos a la siguiente partícula
            }
        }
    }
}

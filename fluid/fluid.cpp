#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <span>
#include <cstring>  // Para std::memcpy
#include <array>


struct Particle {
    float px, py, pz;
    //float hvx, hvy, hvz;
    //float vx, vy, vz;
};
struct Fluid {
    float ppm = 0;  // Partículas por metro
    int np = 0;  // Número de partículas
    std::vector<Particle> particles;
};

void mesh_simulation(const Fluid &fluid);

void print_simulation(int iteraciones, const Fluid &fluid);

void readFluid(std::ifstream &in, Fluid &fluid) {
    in.read(static_cast<char *>(static_cast<void *>(&fluid.ppm)), sizeof(fluid.ppm));
    in.read(static_cast<char *>(static_cast<void *>(&fluid.np)), sizeof(fluid.np));
    fluid.particles.resize(fluid.np);
    for (int i = 0; i < fluid.np; ++i) {
        in.read(static_cast<char *>(static_cast<void *>(&fluid.particles[i])), sizeof(Particle));
    }
}

void writeFluid(std::ofstream &out, const Fluid &fluid) {
    std::array<char, sizeof(fluid.ppm)> ppmBuffer = {0};
    std::memcpy(ppmBuffer.data(), &fluid.ppm, sizeof(fluid.ppm));
    out.write(ppmBuffer.data(), sizeof(fluid.ppm));

    std::array<char, sizeof(fluid.np)> npBuffer = {0};
    std::memcpy(npBuffer.data(), &fluid.np, sizeof(fluid.np));
    out.write(npBuffer.data(), sizeof(fluid.np));

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
    if (fluid.particles.size() != static_cast<std::vector<Particle>::size_type>(fluid.np)) {
        std::cerr << "Error: El número de partículas leídas (" << fluid.particles.size()
                  << ") no coincide con numberparticles (" << fluid.np << ").\n";
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
        for (int i = 0; i < fluid.np; ++i) {
            const Particle &particula = fluid.particles[i];
            // Aquí se puede hacer la simulación para la partícula actual
            std::cout << "Partícula " << i + 1 << ": px=" << particula.px << " py=" << particula.py << " pz="
                      << particula.pz << "\n";

        }
        std::cout << "\n";
    }
}

void mesh_simulation(const Fluid &fluid) {
    const double smoothingLength = 1.695 / fluid.np;
    const double particleMass = std::pow(10.0, 3.0) / std::pow(fluid.np, 3.0);
    const double xmax = 0.065;
    const double ymax = 0.1;
    const double zmax = 0.065;
    const double xmin = -0.065;
    const double ymin = -0.08;
    const double zmin = -0.065;
    const double numberblocksx = floor((xmax - xmin) / smoothingLength);
    const double numberblocksy = floor((ymax - ymin) / smoothingLength);
    const double numberblocksz = floor((zmax - zmin) / smoothingLength);
    const double numBlocks = numberblocksx * numberblocksy * numberblocksz;
    const double meshx = (xmax - xmin) / numberblocksx;
    const double meshy = (ymax - ymin) / numberblocksy;
    const double meshz = (zmax - zmin) / numberblocksz;
    // Mostrar los valores por pantalla en el formato requerido
    std::cout << "Number of particles: " << fluid.np << "\n";
    std::cout << "Particles per meter: " << fluid.ppm << "\n";
    std::cout << "Smoothing length: " << smoothingLength << "\n";
    std::cout << "Particle mass: " << particleMass << "\n";
    std::cout << "Grid size: " << numberblocksx << " x " << numberblocksy << " x " << numberblocksz << "\n";
    std::cout << "Number of blocks: " << numBlocks << "\n";
    std::cout << "Block size: " << meshx << " x " << meshy << " x " << meshz << "\n";
}


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <numbers>
#include <span>
#include <cstring>  // Para std::memcpy
#include <array>
#include <ctime>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include <iomanip>
unsigned t0, t1;

std::pair<double, double> mesh_simulation(const Fluid &fluid, Grid &malla);

void print_simulation(int iteraciones, const Fluid &fluid);
// void reposicionarParticulas(const Fluid &fluid, std::vector<Block> &blocks);

void incrementDensities(Fluid &fluid, double smoothingLength);
void transformDensities(Fluid &fluid, double h, double particleMass);
void transferAcceleration(Fluid &fluid, double h, double particleMass);
void initAccelerations(Fluid &fluid);
void transferAccelerationMejorada(Fluid &fluid, double h, double ps, double particleMass, double factor1, double factor2);
void performSPHCalculations(Fluid &fluid, double smoothingLength, double particleMass, double mu);
void particleColissions(Fluid &fluid, std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);
void particlesMovement(Fluid &fluid);
void limitInteractions(Fluid &fluid, std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);

void readFluid(std::ifstream& in, Fluid& fluid) {
    in.read(reinterpret_cast<char*>(&fluid.particlespermeter), sizeof(float));
    in.read(reinterpret_cast<char*>(&fluid.numberparticles), sizeof(int));

    fluid.particles.resize(fluid.numberparticles);
    for (int i = 0; i < fluid.numberparticles; ++i) {
        fluid.particles[i].id = i;
        float temp = 0;
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].px = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].py = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].pz = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].hvx = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].hvy = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].hvz = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].vx = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].vy = static_cast<double>(temp);
        in.read(reinterpret_cast<char*>(&temp), sizeof(float));
        fluid.particles[i].vz = static_cast<double>(temp);
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
    t0=clock();
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

    // Calcula los valores utilizando la función calculateValues
    Grid malla(Constantes::limInferior, Constantes::limSuperior);
    auto result = mesh_simulation(fluid, malla);
    double smoothingLength = result.first;
    double particleMass = result.second;

    std::vector<Block> blocks = malla.getBlocks();
    //const double factor1 = 15.0 / (M_PI * std::pow(smoothingLength, 6));
    //const double factor2 = 45.0 / (M_PI * std::pow(smoothingLength, 6) * Constantes::viscosidad * particleMass);
    for (int iter = 0; iter < iteraciones; ++iter) {
        std::cout << "Iteración " << iter + 1 << "\n";
        initAccelerations(fluid);
        malla.reposicionarParticulas(fluid, blocks);
        incrementDensities(fluid,  smoothingLength);
        transformDensities(fluid, smoothingLength, particleMass);
        transferAcceleration(fluid, smoothingLength, particleMass);
        if (iter == iteraciones-1) {
            for (auto & particle : fluid.particles) {
                // La aceleracion es la por defecto?
                std::cout << std::setprecision(17) <<"La partícula " << particle.id << " está en el bloque "
                          << particle.idBloque << " x: " << particle.px << " y: " << particle.py
                          << " z: " << particle.pz << " " << "Velocidad: (" << particle.vx << ", " << particle.vy << ", "
                          << particle.vz << ")"
                          << "     Aceleración: (" << particle.ax << ", " << particle.ay << ", "
                          << particle.az << ")" << std::endl;

            }
        }
        //transferAccelerationMejorada(fluid, smoothingLength, Constantes::presRigidez,  particleMass, factor1,factor2);
        /* for (int i = 0; i < fluid.numberparticles; ++i) {
            std::cout << "La partícula " << fluid.particles[i].id << " x: " << fluid.particles[i].ax <<  " y: " << fluid.particles[i].ay<<  " z: " << fluid.particles[i].az<< std::endl;
        } */
        particleColissions(fluid, blocks, malla.numberblocksx, malla.numberblocksy, malla.numberblocksz);
        particlesMovement(fluid);
        limitInteractions(fluid, blocks, malla.numberblocksx, malla.numberblocksy, malla.numberblocksz);

    }
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
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    std::cout << "Execution Time: " << time;
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
std::pair<double, double> mesh_simulation(const Fluid &fluid, Grid &malla) {
    const double smoothingLength = Constantes::multRadio / fluid.particlespermeter;
    const double particleMass = std::pow(10.0, 3.0) / std::pow(fluid.particlespermeter, 3.0);


    malla.dividirEnBloques(smoothingLength);

    // Mostrar los valores por pantalla en el formato requerido
    std::cout << "Number of particles: " << fluid.numberparticles << "\n";
    std::cout << "Particles per meter: " << fluid.particlespermeter << "\n";
    std::cout << "Smoothing length: " << smoothingLength << "\n";
    std::cout << "Particle mass: " << particleMass << "\n";
    std::cout << "Grid size: " << malla.numberblocksx << " x " << malla.numberblocksy << " x " << malla.numberblocksz << "\n";
    std::cout << "Number of blocks: " << malla.numBlocks << "\n";
    std::cout << "Block size: " << malla.meshx << " x " << malla.meshy << " x " << malla.meshz << "\n";

    return std::make_pair(smoothingLength, particleMass);
}

double calculateDistanceSquared(const Particle &particle1, const Particle &particle2) {
    double dx = particle1.px - particle2.px;
    double dy = particle1.py - particle2.py;
    double dz = particle1.pz - particle2.pz;
    return dx * dx + dy * dy + dz * dz;
}

double calculateDeltaDensity(double h, double distSquared) {
    if (distSquared < h * h) {
        return std::pow(((h * h) - distSquared), 3);
    }
    return 0.0;
}

void incrementDensities(Fluid &fluid, double h) {
    for (int i = 0; i < fluid.numberparticles; ++i) {
        for (int j = i + 1; j < fluid.numberparticles; ++j) {
            double distSquared = calculateDistanceSquared(fluid.particles[i], fluid.particles[j]);

            // Calcula el incremento de densidad ∆ρij
            double deltaDensity = calculateDeltaDensity(h, distSquared);

            // Incrementa la densidad de ambas partículas
            fluid.particles[i].density += deltaDensity;
            fluid.particles[j].density += deltaDensity;
        }
    }
}


void transformDensities(Fluid &fluid, double h, double particleMass) {
    const double factor = (315.0 / (64.0 * std::numbers::pi * std::pow(h, 9))) * particleMass;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        fluid.particles[i].density = (fluid.particles[i].density + std::pow(h, 6)) * factor;
    }
}

void transferAcceleration(Fluid &fluid, double h, double particleMass) {
    const double smoothingLengthSquared = h * h;
    const double smallQ = 10e-12;
    const double factor1 = 15 / (std::numbers::pi * std::pow(h, 6));
    const double factor2 = (45 / (std::numbers::pi * std::pow(h, 6)) * Constantes::viscosidad * particleMass);

    for (int i = 0; i < fluid.numberparticles; ++i) {
        for (int j = i + 1; j < fluid.numberparticles; ++j) {
            const double distSquared = calculateDistanceSquared(fluid.particles[i], fluid.particles[j]);

            if (distSquared >= smoothingLengthSquared) {
                continue;
            }

            const double q = std::max(distSquared, smallQ);
            const double dist = std::sqrt(q);
            const double distX = fluid.particles[i].px - fluid.particles[j].px;
            const double distY = fluid.particles[i].py - fluid.particles[j].py;
            const double distZ = fluid.particles[i].pz - fluid.particles[j].pz;
            const double distdiv = 1 / dist;

            const double hMinusDistSquared = std::pow(h - dist, 2);
            const double deltaDensity = (fluid.particles[i].density + fluid.particles[j].density - 2 * Constantes::densFluido);

            const double deltaAijX = ((distX * factor1 * ((3 * particleMass * Constantes::presRigidez) * 0.5) * (hMinusDistSquared * distdiv) * deltaDensity +
                                       (fluid.particles[j].vx - fluid.particles[i].vx) * factor2) / (fluid.particles[i].density * fluid.particles[j].density));
            const double deltaAijY = ((distY * factor1 * (3.0 * particleMass * Constantes::presRigidez * 0.5) * (hMinusDistSquared * distdiv) * deltaDensity +
                                       (fluid.particles[j].vy - fluid.particles[i].vy) * factor2) / (fluid.particles[i].density * fluid.particles[j].density));
            const double deltaAijZ = ((distZ * factor1 * (3.0 * particleMass * Constantes::presRigidez * 0.5) * (hMinusDistSquared * distdiv) * deltaDensity +
                                       (fluid.particles[j].vz - fluid.particles[i].vz) * factor2) / (fluid.particles[i].density * fluid.particles[j].density));

            fluid.particles[i].ax += deltaAijX;
            fluid.particles[i].ay += deltaAijY;
            fluid.particles[i].az += deltaAijZ;
            fluid.particles[j].ax -= deltaAijX;
            fluid.particles[j].ay -= deltaAijY;
            fluid.particles[j].az -= deltaAijZ;
        }
    }
}


// Función para inicializar las aceleraciones
void initAccelerations(Fluid &fluid) {
    for (int i = 0; i < fluid.numberparticles; ++i) {
        // Inicializa la densidad
        fluid.particles[i].density = 0.0;
        fluid.particles[i].ax = Constantes::gravedad.x;
        fluid.particles[i].ay = Constantes::gravedad.y; // Configura la aceleración de gravedad
        fluid.particles[i].az = Constantes::gravedad.z;
    }
}
void transferAccelerationMejorada(Fluid &fluid, double h, double ps, double particleMass, double factor1, double factor2) {
    const double smoothingLengthSquared = h * h;
    const double smallQ = 10e-12;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        for (int j = i + 1; j < fluid.numberparticles; ++j) {
            const double distSquared = calculateDistanceSquared(fluid.particles[i], fluid.particles[j]);

            if (distSquared < smoothingLengthSquared) {
                const double q = std::max(distSquared, smallQ);
                const double dist = std::sqrt(q);
                const double invDist = 1.0 / dist;  // Calcula 1.0 / dist una vez y reutilízalo
                const double distX = fluid.particles[i].px - fluid.particles[j].px;
                const double distY = fluid.particles[i].py - fluid.particles[j].py;
                const double distZ = fluid.particles[i].pz - fluid.particles[j].pz;

                const double deltaDensity = (fluid.particles[i].density + fluid.particles[j].density - 2 * 10000.0);
                const double hMinusDistSquaredTimesInvDist = (h - dist) * (h - dist) * invDist;  // Reemplaza la división por una multiplicación

                const double commonFactor = factor1 * (3.0 * particleMass * ps / 2) * hMinusDistSquaredTimesInvDist * deltaDensity;  // Calcula el factor común una vez y reutilízalo
                const double velocityDifferenceFactor = factor2 / (fluid.particles[i].density * fluid.particles[j].density);  // Calcula el factor de diferencia de velocidad una vez y reutilízalo

                const double deltaAijX = distX * commonFactor + (fluid.particles[j].vx - fluid.particles[i].vx) * velocityDifferenceFactor;
                const double deltaAijY = distY * commonFactor + (fluid.particles[j].vy - fluid.particles[i].vy) * velocityDifferenceFactor;
                const double deltaAijZ = distZ * commonFactor + (fluid.particles[j].vz - fluid.particles[i].vz) * velocityDifferenceFactor;

                fluid.particles[i].ax += deltaAijX;
                fluid.particles[i].ay += deltaAijY;
                fluid.particles[i].az += deltaAijZ;
                fluid.particles[j].ax -= deltaAijX;
                fluid.particles[j].ay -= deltaAijY;
                fluid.particles[j].az -= deltaAijZ;
            }
        }
    }
}

void handleXCollisions(Particle& particle, int cx, double numberblocksx) {
    double x = particle.px + particle.hvx * Constantes::pasoTiempo;
    double deltaX;

    if (cx == 0) {
        deltaX = Constantes::tamParticula - (x - Constantes::limInferior.x);
        if (deltaX > 1e-10) {
            particle.ax += Constantes::colisRigidez * deltaX - Constantes::amortiguamiento * particle.vx;
        }
    } else if (cx == static_cast<int>(numberblocksx - 1) ){
        deltaX = Constantes::tamParticula - (Constantes::limSuperior.x - x);
        if (deltaX > 1e-10) {
            particle.ax -= Constantes::colisRigidez * deltaX + Constantes::amortiguamiento * particle.vx;
        }
    }
}

void handleYCollisions(Particle& particle, int cy, double numberblocksy) {
    double y = particle.py + particle.hvy * Constantes::pasoTiempo;
    double deltaY = 0;

    if (cy == 0) {
        deltaY = Constantes::tamParticula - (y - Constantes::limInferior.y);
        if (deltaY > 1e-10) {
            particle.ay += Constantes::colisRigidez * deltaY - Constantes::amortiguamiento * particle.vy;
        }
    } else if (cy == static_cast<int>(numberblocksy - 1)) {
        deltaY = Constantes::tamParticula - (Constantes::limSuperior.y - y);
        if (deltaY > 1e-10) {
            particle.ay -= Constantes::colisRigidez * deltaY + Constantes::amortiguamiento * particle.vy;
        }
    }
}

void handleZCollisions(Particle& particle, int cz, double numberblocksz) {
    double z = particle.pz + particle.hvz * Constantes::pasoTiempo;
    double deltaZ;

    if (cz == 0) {
        deltaZ = Constantes::tamParticula - (z - Constantes::limInferior.z);
        if (deltaZ > 1e-10) {
            particle.az += Constantes::colisRigidez * deltaZ - Constantes::amortiguamiento * particle.vz;
        }
    } else if (cz == static_cast<int>(numberblocksz - 1)) {
        deltaZ = Constantes::tamParticula - (Constantes::limSuperior.z - z);
        if (deltaZ > 1e-10) {
            particle.az -= Constantes::colisRigidez * deltaZ + Constantes::amortiguamiento * particle.vz;
        }
    }
}

// Funcion para calcular las colisiones de particulas
void particleColissions(Fluid &fluid, std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz) {
    for (int blockIndex = 0; blockIndex < numberblocksx * numberblocksy * numberblocksz; ++blockIndex) {
        Block &block = blocks[blockIndex];
        for (auto& particula : fluid.particles) {
            if (particula.idBloque == blockIndex) {
                /* si un bloque tiene cx==0 o cx== numbrblocks-1 se actualiza el ax de todas las particulas de ese bloque, llamando
                a handleXCollisions*/
                if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
                    handleXCollisions(particula, block.cx, numberblocksx);
                }
                /* si un bloque tiene cy==0 o cy== numbrblocks-1 se actualiza el ay de todas las particulas de ese bloque, llamando
                a handleYCollisions*/
                if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
                    handleYCollisions(particula, block.cy, numberblocksy);
                }
                /* si un bloque tiene cz==0 o cz== numbrblocks-1 se actualiza el az de todas las particulas de ese bloque, llamando
                a handleZCollisions*/
                if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
                    handleZCollisions(particula, block.cz, numberblocksz);
                }
            }
        }
    }
}

// Funcion para realizar el movimiento de particulas
void particlesMovement(Fluid &fluid){
    for (Particle& particle : fluid.particles) {
        particle.px = particle.px + particle.hvx * Constantes::pasoTiempo + particle.ax * std::pow(Constantes::pasoTiempo,2);
        particle.py = particle.py + particle.hvy * Constantes::pasoTiempo + particle.ay * std::pow(Constantes::pasoTiempo,2);
        particle.pz = particle.pz + particle.hvz * Constantes::pasoTiempo + particle.az * std::pow(Constantes::pasoTiempo,2);

        particle.vx = particle.hvx + (particle.ax * Constantes::pasoTiempo) * 0.5;
        particle.vy = particle.hvy + (particle.ay * Constantes::pasoTiempo) * 0.5;
        particle.vz = particle.hvz + (particle.az * Constantes::pasoTiempo) * 0.5;

        particle.hvx = particle.hvx + particle.ax * Constantes::pasoTiempo;
        particle.hvy = particle.hvy + particle.ay * Constantes::pasoTiempo;
        particle.hvz = particle.hvz + particle.az * Constantes::pasoTiempo;
    }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la X (si hay interaccion)
void InteractionLimitX(Particle& particle, int cx, double numberblocksx){
    double dx;
    if (cx == 0){
        dx = particle.px - Constantes::limInferior.x;
        if (dx < 0){
            particle.px = Constantes::limInferior.x - dx;
            particle.vx = -particle.vx;
            particle.hvx = -particle.hvx;
        }
    }
    else if(cx == static_cast<int>(numberblocksx - 1)){
        dx = Constantes::limSuperior.x - particle.px;
        if (dx < 0){
            particle.px = Constantes::limSuperior.x + dx;
            particle.vx = -particle.vx;
            particle.hvx = -particle.hvx;
        }
    }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la Y (si hay interaccion)
void InteractionLimitY(Particle& particle, int cy, double numberblocksy){
   double dy;
   if (cy == 0){
          dy = particle.py - Constantes::limInferior.y;
          if (dy < 0){
              particle.py = Constantes::limInferior.y - dy;
              particle.vy = -particle.vy;
              particle.hvy = -particle.hvy;
          }
   }
   else if(cy == static_cast<int>(numberblocksy - 1)){
          dy = Constantes::limSuperior.y - particle.py;
          if (dy < 0){
              particle.py = Constantes::limSuperior.y + dy;
              particle.vy = -particle.vy;
              particle.hvy = -particle.hvy;
          }
   }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la Z (si hay interaccion)
void InteractionLimitZ(Particle& particle, int cz, double numberblocksz){
   double dz;
   if (cz == 0){
          dz = particle.pz - Constantes::limInferior.z;
          if (dz < 0){
            particle.pz = Constantes::limInferior.z - dz;
            particle.vz = -particle.vz;
            particle.hvz = -particle.hvz;
          }
   }
   else if(cz == static_cast<int>(numberblocksz - 1)){
          dz = Constantes::limSuperior.z - particle.pz;
          if (dz < 0){
            particle.pz = Constantes::limSuperior.z + dz;
            particle.vz = -particle.vz;
            particle.hvz = -particle.hvz;
          }
   }
}

// Funcion para las interacciones con los límites del recinto de una particula
void limitInteractions(Fluid& fluid,std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz) {
   for (int blockIndex = 0; blockIndex < numberblocksx * numberblocksy * numberblocksz;
        ++blockIndex) {
          Block & block = blocks[blockIndex];
          for (auto & particula : fluid.particles) {
            if (particula.idBloque == blockIndex) {
                /* si un bloque tiene cx==0 o cx== numbrblocks-1 se actualiza el ax de todas las
                particulas de ese bloque, llamando a handleXCollisions*/
                if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
                    InteractionLimitX(particula, block.cx, numberblocksx);
                }
                /* si un bloque tiene cy==0 o cy== numbrblocks-1 se actualiza el ay de todas las
                particulas de ese bloque, llamando a handleYCollisions*/
                if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
                    InteractionLimitY(particula, block.cy, numberblocksy);
                }
                /* si un bloque tiene cz==0 o cz== numbrblocks-1 se actualiza el az de todas las
                particulas de ese bloque, llamando a handleZCollisions*/
                if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
                    InteractionLimitZ(particula, block.cz, numberblocksz);
                }
            }
          }
   }
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <span>
#include <cstring>  // Para std::memcpy
#include <array>
#include <ctime>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
unsigned t0, t1;

std::pair<double, double> mesh_simulation(const Fluid &fluid, Grid &malla);

void print_simulation(int iteraciones, const Fluid &fluid);
// void reposicionarParticulas(const Fluid &fluid, std::vector<Block> &blocks);

void incrementDensities(Fluid &fluid, double smoothingLength);
void transformDensities(Fluid &fluid, double h, double particleMass);
void transferAcceleration(Fluid &fluid, double h, double ps, double mu, double particleMass);
void transferAccelerationMejorada(Fluid &fluid, double h, double ps, double particleMass, double factor1, double factor2);
void performSPHCalculations(Fluid &fluid, double smoothingLength, double particleMass, double mu);
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);
void particlesMovement(Fluid &fluid);
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);

void readFluid(std::ifstream& in, Fluid& fluid) {
    in.read(reinterpret_cast<char*>(&fluid.particlespermeter), sizeof(float));
    in.read(reinterpret_cast<char*>(&fluid.numberparticles), sizeof(int));

    fluid.particles.resize(fluid.numberparticles);
    for (int i = 0; i < fluid.numberparticles; ++i) {
        fluid.particles[i].id = i;
        in.read(reinterpret_cast<char*>(&fluid.particles[i].px), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].py), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].pz), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].hvx), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].hvy), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].hvz), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].vx), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].vy), sizeof(float));
        in.read(reinterpret_cast<char*>(&fluid.particles[i].vz), sizeof(float));
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
    const double factor1 = 15.0 / (M_PI * std::pow(smoothingLength, 6));
    const double factor2 = 45.0 / (M_PI * std::pow(smoothingLength, 6) * Constantes::viscosidad * particleMass);
    for (int iter = 0; iter < iteraciones; ++iter) {
        std::cout << "Iteración " << iter + 1 << "\n";
        malla.reposicionarParticulas(fluid);
        incrementDensities(fluid,  smoothingLength);
        transformDensities(fluid, smoothingLength, particleMass);
        //transferAcceleration(fluid, smoothingLength, Constantes::presRigidez, Constantes::viscosidad, particleMass);
        transferAccelerationMejorada(fluid, smoothingLength, Constantes::presRigidez,  particleMass, factor1,factor2);

        particleColissions(blocks, malla.numberblocksx, malla.numberblocksy, malla.numberblocksz);
        particlesMovement(fluid);
        limitInteractions(blocks, malla.numberblocksx, malla.numberblocksy, malla.numberblocksz);
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
        double q = 1.0 - distSquared / (h * h);
        return (h * h * h) * (1.0 - q) * (1.0 - q) * (1.0 - q);
    }
    return 0.0;
}

void incrementDensities(Fluid &fluid, double smoothingLength) {
    const double smoothingLengthSquared = smoothingLength * smoothingLength;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        for (int j = i + 1; j < fluid.numberparticles; ++j) {
            const double distSquared = calculateDistanceSquared(fluid.particles[i], fluid.particles[j]);

            if (distSquared < smoothingLengthSquared) {
                const double deltaDensity = calculateDeltaDensity(smoothingLength, distSquared);
                fluid.particles[i].density += deltaDensity;
                fluid.particles[j].density += deltaDensity;
            }
        }
    }
}

void transformDensities(Fluid &fluid, double h, double particleMass) {
    const double factor = (315.0 / (64.0 * M_PI * std::pow(h, 9))) * particleMass * h * h * h * h * h * h;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        fluid.particles[i].density = (fluid.particles[i].density + 10000.0) * factor;
    }
}

void transferAcceleration(Fluid &fluid, double h, double ps, double mu, double particleMass) {
    const double smoothingLengthSquared = h * h;
    const double smallQ = 10e-12;
    const double factor1 = 15.0 / (M_PI * std::pow(h, 6));
    const double factor2 = 45.0 / (M_PI * std::pow(h, 6) * mu * particleMass);

    for (int i = 0; i < fluid.numberparticles; ++i) {
        for (int j = i + 1; j < fluid.numberparticles; ++j) {
            const double distSquared = calculateDistanceSquared(fluid.particles[i], fluid.particles[j]);

            if (distSquared < smoothingLengthSquared) {
                const double q = std::max(distSquared, smallQ);
                const double dist = std::sqrt(q);
                const double distX = fluid.particles[i].px - fluid.particles[j].px;
                const double distY = fluid.particles[i].py - fluid.particles[j].py;
                const double distZ = fluid.particles[i].pz - fluid.particles[j].pz;

                const double deltaDensity = (fluid.particles[i].density + fluid.particles[j].density - 2 * 10000.0);
                const double deltaAijX = (distX * factor1 * (3.0 * particleMass * ps / 2) * (std::pow(h - dist, 2) / dist) * deltaDensity +
                                          (fluid.particles[j].vx - fluid.particles[i].vx) * (factor2 / fluid.particles[i].density * fluid.particles[j].density));
                const double deltaAijY = (distY * factor1 * (3.0 * particleMass * ps / 2) * (std::pow(h - dist, 2) / dist) * deltaDensity +
                                          (fluid.particles[j].vy - fluid.particles[i].vy) * (factor2 / fluid.particles[i].density * fluid.particles[j].density));
                const double deltaAijZ = (distZ * factor1 * (3.0 * particleMass * ps / 2) * (std::pow(h - dist, 2) / dist) * deltaDensity +
                                          (fluid.particles[j].vz - fluid.particles[i].vz) * (factor2 / fluid.particles[i].density * fluid.particles[j].density));

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
    double deltaY;

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
//funcion para calcular las colisiones de particulas
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz) {
    for (auto& block : blocks) {
        /* si un bloque tiene cx==0 o cx== numbrblocks-1 se actualiza el ax de todas las particulas de ese bloque, llamando
        a handleXCollisions*/
        if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
            for (auto& particle : block.particles) {
                handleXCollisions(particle, block.cx,numberblocksx);
            }
        }
        /* si un bloque tiene cy==0 o cy== numbrblocks-1 se actualiza el ay de todas las particulas de ese bloque, llamando
        a handleYCollisions*/
        if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
            for (auto& particle : block.particles) {
                handleYCollisions(particle, block.cy,numberblocksy);
            }
        }
        /* si un bloque tiene cz==0 o cz== numbrblocks-1 se actualiza el az de todas las particulas de ese bloque, llamando
        a handleZCollisions*/
        if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
            for (auto& particle : block.particles) {
                handleZCollisions(particle, block.cz,numberblocksz);
            }
        }
    }
}
//funcion para realizar el movimiento de particulas
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

//función que hace la interacción con el borde del recinto respecto a la X (si hay interacción)
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

//función que hace la interacción con el borde del recinto respecto a la Y (si hay interacción)
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

//función que hace la interacción con el borde del recinto respecto a la Z (si hay interacción)
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

//funcion para las interacciones con los límites del recinto de una partícula
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz){
    for(auto& block : blocks){
        /* si un bloque tiene cx==0 o cx== numbrblocks-1 se va a comprobar si la partícula está fuera del límite x
        en InteractionLimitX*/
        if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
            for (auto& particle : block.particles) {
                InteractionLimitX(particle, block.cx,numberblocksx);
            }
        }
        /* si un bloque tiene cy==0 o cy== numbrblocks-1 se va a comprobar si la partícula está fuera del límite y
        en InteractionLimitY*/
        if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
            for (auto& particle : block.particles) {
                InteractionLimitY(particle, block.cy,numberblocksy);
            }
        }
        /* si un bloque tiene cz==0 o cz== numbrblocks-1 se va a comprobar si la partícula está fuera del límite z
        en InteractionLimitZ*/
        if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
            for (auto& particle : block.particles) {
                InteractionLimitZ(particle, block.cz,numberblocksz);
            }
        }
    }
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <numbers>
#include <span>
#include <array>
#include <ctime>
#include <iomanip>
#include <limits>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include "sim/progargs.hpp"


std::pair<double, double> mesh_simulation(const Fluid &fluid, Grid &malla);

void initAccelerations(std::vector<Block>& blocks);
void incrementDensities(std::vector<Block>& blocks, double h, Grid& malla);
void transformDensities(std::vector<Block>& blocks, double h, double factorDensTransf);
void transferAcceleration(std::vector<Block>& blocks, double h, Constantes::ConstAccTransf& constAccTransf, Grid& malla);
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);
void particlesMovement(std::vector<Block>& blocks);
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);




//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void writeFluid(std::ofstream& out, const Fluid& fluid) {
    out.write(reinterpret_cast<const char*>(&fluid.particlespermeter), sizeof(float));
    out.write(reinterpret_cast<const char*>(&fluid.numberparticles), sizeof(int));
    for (const auto& particle : fluid.particles) {
        for (const double* attr : {&particle.px, &particle.py, &particle.pz,
                                   &particle.hvx, &particle.hvy, &particle.hvz,
                                   &particle.vx, &particle.vy, &particle.vz}) {
            auto temp = static_cast<float>(*attr);
            out.write(reinterpret_cast<const char*>(&temp), sizeof(float));
        }
    }
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

int main(int argc, char *argv[]) {
    unsigned const tiempo0=clock();
    std::span const args_view{argv, static_cast<std::size_t>(argc)};
    std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};


    Argumentos argumentos;

    const Constantes::ErrorCode errorCode = comprobarArgs(argc, arguments, argumentos);
    if (errorCode != 0) {
        return errorCode;
    }

    // Calcula los valores utilizando la función calculateValues
    Grid malla(Constantes::limInferior, Constantes::limSuperior);
    auto result = mesh_simulation(argumentos.fluid, malla);
    double const smoothingLength = result.first;
    double const particleMass = result.second;

    // Para la transferencia de densidades
    const double factorDensTransf = (315.0 / (64.0 * std::numbers::pi * std::pow(smoothingLength, 9))) * particleMass;

    // Para la transferencia de aceleraciones
    Constantes::ConstAccTransf constAccTransf;
    constAccTransf.hSquared = smoothingLength * smoothingLength;
    constAccTransf.factor2 = (45 / (std::numbers::pi * std::pow(smoothingLength, 6)) * Constantes::viscosidad * particleMass);
    constAccTransf.commonFactor = (15 / (std::numbers::pi * std::pow(smoothingLength, 6))) * ((3 * particleMass * Constantes::presRigidez) * Constantes::factor05);

    std::vector<Block> blocks = malla.getBlocks();
    //const double factor1 = 15.0 / (M_PI * std::pow(smoothingLength, 6));
    //const double factor2 = 45.0 / (M_PI * std::pow(smoothingLength, 6) * Constantes::viscosidad * particleMass);
    for (int iter = 0; iter < argumentos.iteraciones; ++iter) {
        // std::cout << "Iteración " << iter + 1 << "\n";

        if (iter == 0) {
            malla.reposicionarParticulasFluid(argumentos.fluid, blocks);
        } else {
            malla.reposicionarParticulasBloque(blocks);
        }

        initAccelerations(blocks);
        incrementDensities(blocks, smoothingLength, malla);
        transformDensities(blocks, smoothingLength, factorDensTransf);
        transferAcceleration(blocks, smoothingLength, constAccTransf, malla);
        particleColissions(blocks, malla.getNumberblocksx(), malla.getNumberblocksy(), malla.getNumberblocksz());
        particlesMovement(blocks);
        limitInteractions(blocks, malla.getNumberblocksx(), malla.getNumberblocksy(), malla.getNumberblocksz());


        /* if (iter == argumentos.iteraciones - 1) {
            for (const Block &block: blocks) {
                // Itera sobre las partículas en el bloque actual
                for (const Particle &particle: block.particles) {
                    std::cout << std::setprecision(15) << "La partícula " << particle.id << " " << particle.density
                            << " está en el bloque "
                            << particle.idBloque << " x: " << particle.px << " y: " << particle.py
                            << " z: " << particle.pz << "    Velocidad: (" << particle.vx << ", " << particle.vy << ", "
                            << particle.vz << ")"
                            << "     Aceleración: (" << particle.ax << ", " << particle.ay << ", "
                            << particle.az << ")" << '\n';
                }
            }
        } */
    }

    // Escribir el estado final del fluido en el archivo de salida
    std::ofstream output(arguments[2], std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot open " << arguments[2] << " for writing\n";
        return Constantes::ErrorCode::CANNOT_OPEN_FILE_WRITING;
    }
    writeFluid(output, argumentos.fluid);

    output.close();
    std::cout << "Simulación completada. Estado final del fluido guardado en: " << arguments[2] << "\n";
    unsigned const tiempo1= clock();
    double const time = (double(tiempo1-tiempo0)/CLOCKS_PER_SEC);
    std::cout << "Execution Time: " << time;
    return 0;
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
    std::cout << "Grid size: " << malla.getNumberblocksx() << " x " << malla.getNumberblocksy() << " x " << malla.getNumberblocksz() << "\n";
    std::cout << "Number of blocks: " << malla.getNumBlocks() << "\n";
    std::cout << "Block size: " << malla.getMeshx() << " x " << malla.getMeshy() << " x " << malla.getMeshz() << "\n";

    return std::make_pair(smoothingLength, particleMass);
}

// Función para inicializar las aceleraciones
void initAccelerations(std::vector<Block>& blocks) {
    for (auto& block : blocks) {
        for (auto& particle : block.particles) {
            // Inicializa la densidad
            particle.density = 0.0;
            particle.ax = Constantes::gravedad.x;
            particle.ay = Constantes::gravedad.y; // Configura la aceleración de gravedad
            particle.az = Constantes::gravedad.z;
        }
    }
}

double calculateDistanceSquared(const Particle &particle1, const Particle &particle2) {
    double const deltaX = particle1.px - particle2.px;
    double const deltaY = particle1.py - particle2.py;
    double const deltaZ = particle1.pz - particle2.pz;
    return deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
}



void incrementDensities(std::vector<Block>& blocks, double h, Grid& malla) {
    for (auto& block1 : blocks) {
        for (auto& particle1 : block1.particles) {
            // Considera solo los bloques que son vecinos inmediatos de block1
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        const int neighbor_cx = block1.cx + dx;
                        const int neighbor_cy = block1.cy + dy;
                        const int neighbor_cz = block1.cz + dz;

                        // Asegúrate de que las coordenadas del vecino estén dentro de los límites de la cuadrícula
                        if (neighbor_cx >= 0 && neighbor_cx < malla.getNumberblocksx() &&
                            neighbor_cy >= 0 && neighbor_cy < malla.getNumberblocksy() &&
                            neighbor_cz >= 0 && neighbor_cz < malla.getNumberblocksz()) {

                            // Calcula el índice del bloque vecino
                            const int neighborIndex = neighbor_cz + neighbor_cy * malla.getNumberblocksz()
                                    + neighbor_cx * malla.getNumberblocksz() * malla.getNumberblocksy();

                            Block& block2 = blocks[neighborIndex];
                            for (auto& particle2 : block2.particles) {
                                if (particle1.id < particle2.id) {
                                    double const distSquared = calculateDistanceSquared(particle1, particle2);
                                    if (distSquared < h * h) {
                                        // Calcula el incremento de densidad ∆ρij
                                        double const deltaDensity= std::pow(((h * h) - distSquared), 3);
                                        // Incrementa la densidad de ambas partículas
                                        particle1.density += deltaDensity;
                                        particle2.density += deltaDensity;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}




void transformDensities(std::vector<Block>& blocks, double h, double factorDensTransf) {
    for (auto& block : blocks) {
        for (auto& particle : block.particles) {
            particle.density = (particle.density + std::pow(h, 6)) * factorDensTransf;
        }
    }
}

void transferAcceleration(std::vector<Block>& blocks, double h, Constantes::ConstAccTransf& constAccTransf, Grid& malla) {
    for (auto& block1 : blocks) {
        for (auto& particle1 : block1.particles) {
            // Considera solo los bloques que son vecinos inmediatos de block1
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        const int neighbor_cx = block1.cx + dx;
                        const int neighbor_cy = block1.cy + dy;
                        const int neighbor_cz = block1.cz + dz;

                        // Asegúrate de que las coordenadas del vecino estén dentro de los límites de la cuadrícula
                        if (neighbor_cx >= 0 && neighbor_cx < malla.getNumberblocksx() &&
                            neighbor_cy >= 0 && neighbor_cy < malla.getNumberblocksy() &&
                            neighbor_cz >= 0 && neighbor_cz < malla.getNumberblocksz()) {

                            // Calcula el índice del bloque vecino
                            const int neighborIndex = neighbor_cz + neighbor_cy * malla.getNumberblocksz()
                                    + neighbor_cx * malla.getNumberblocksz() * malla.getNumberblocksy();

                            Block &block2 = blocks[neighborIndex];
                            for (auto &particle2: block2.particles) {
                                if (particle1.id < particle2.id) {
                                    const double distSquared = calculateDistanceSquared(particle1, particle2);
                                    if (distSquared >= constAccTransf.hSquared) {
                                        continue;
                                    }
                                    const double maxDistanceSquared = std::max(distSquared, Constantes::smallQ);
                                    const double dist = std::sqrt(maxDistanceSquared);
                                    const double distX = particle1.px - particle2.px;
                                    const double distY = particle1.py - particle2.py;
                                    const double distZ = particle1.pz - particle2.pz;

                                    const double distdiv = 1 / dist;
                                    const double hMinusDistSquared = std::pow(h - dist, 2);
                                    const double deltaDensity = (particle1.density + particle2.density -
                                                                 2 * Constantes::densFluido);

                                    const double densitydivmul = 1 /
                                                                 (particle1.density * particle2.density);
                                    const double factorcomun = constAccTransf.commonFactor * hMinusDistSquared * distdiv * deltaDensity;
                                    const double deltaAijX =
                                            ((distX * (factorcomun) +
                                              (particle2.vx - particle1.vx) * constAccTransf.factor2)) * densitydivmul ;

                                    const double deltaAijY =
                                            ((distY * (factorcomun) +
                                              (particle2.vy - particle1.vy) * constAccTransf.factor2)) * densitydivmul;

                                    const double deltaAijZ =
                                            ((distZ * (factorcomun) +
                                              (particle2.vz - particle1.vz) * constAccTransf.factor2)) * densitydivmul;

                                    particle1.ax += deltaAijX;
                                    particle1.ay += deltaAijY;
                                    particle1.az += deltaAijZ;
                                    particle2.ax -= deltaAijX;
                                    particle2.ay -= deltaAijY;
                                    particle2.az -= deltaAijZ;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}





void handleXCollisions(Particle& particle, int cx, double numberblocksx) {
    double const newPositionX = particle.px + particle.hvx * Constantes::pasoTiempo;
    double deltaX = NAN;

    if (cx == 0) {
        deltaX = Constantes::tamParticula - (newPositionX - Constantes::limInferior.x);
        if (deltaX > Constantes::factor1e10) {
            particle.ax += Constantes::colisRigidez * deltaX - Constantes::amortiguamiento * particle.vx;
        }
    } else if (cx == static_cast<int>(numberblocksx - 1) ){
        deltaX = Constantes::tamParticula - (Constantes::limSuperior.x - newPositionX);
        if (deltaX > Constantes::factor1e10) {
            particle.ax -= Constantes::colisRigidez * deltaX + Constantes::amortiguamiento * particle.vx;
        }
    }
}


void handleYCollisions(Particle& particle, int cy, double numberblocksy) {
    double const newPositionY = particle.py + particle.hvy * Constantes::pasoTiempo;
    double deltaY = NAN;

    if (cy == 0) {
        deltaY = Constantes::tamParticula - (newPositionY - Constantes::limInferior.y);
        if (deltaY > Constantes::factor1e10) {
            particle.ay += Constantes::colisRigidez * deltaY - Constantes::amortiguamiento * particle.vy;
        }
    } else if (cy == static_cast<int>(numberblocksy - 1)) {
        deltaY = Constantes::tamParticula - (Constantes::limSuperior.y - newPositionY);
        if (deltaY > Constantes::factor1e10) {
            particle.ay -= Constantes::colisRigidez * deltaY + Constantes::amortiguamiento * particle.vy;
        }
    }
}

void handleZCollisions(Particle& particle, int cz, double numberblocksz) {
    double const newPositionZ = particle.pz + particle.hvz * Constantes::pasoTiempo;
    double deltaZ = NAN;

    if (cz == 0) {
        deltaZ = Constantes::tamParticula - (newPositionZ - Constantes::limInferior.z);
        if (deltaZ > Constantes::factor1e10) {
            particle.az += Constantes::colisRigidez * deltaZ - Constantes::amortiguamiento * particle.vz;
        }
    } else if (cz == static_cast<int>(numberblocksz - 1)) {
        deltaZ = Constantes::tamParticula - (Constantes::limSuperior.z - newPositionZ);
        if (deltaZ > Constantes::factor1e10) {
            particle.az -= Constantes::colisRigidez * deltaZ + Constantes::amortiguamiento * particle.vz;
        }
    }
}

// Funcion para calcular las colisiones de particulas
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz) {
    for (auto& block : blocks) {
        for (auto& particula : block.particles) {
            /* si un bloque tiene cx==0 o cx== numbrblocks-1 se actualiza el ax de todas las particulas de ese bloque, llamando
            a handleXCollisions*/
            if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
                handleZCollisions(particula, block.cx, numberblocksx);
            }
            /* si un bloque tiene cy==0 o cy== numbrblocks-1 se actualiza el ay de todas las particulas de ese bloque, llamando
            a handleYCollisions*/
            if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
                handleYCollisions(particula, block.cy, numberblocksy);
            }
            /* si un bloque tiene cz==0 o cz== numbrblocks-1 se actualiza el az de todas las particulas de ese bloque, llamando
            a handleZCollisions*/
            if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
                handleXCollisions(particula, block.cz, numberblocksz);
            }
        }
    }
}


// Funcion para realizar el movimiento de particulas
void particlesMovement(std::vector<Block>& blocks){
    for (auto& block : blocks) {
        for (auto &particle: block.particles) {
            particle.px = particle.px + particle.hvx * Constantes::pasoTiempo +
                          particle.ax * std::pow(Constantes::pasoTiempo, 2);
            particle.py = particle.py + particle.hvy * Constantes::pasoTiempo +
                          particle.ay * std::pow(Constantes::pasoTiempo, 2);
            particle.pz = particle.pz + particle.hvz * Constantes::pasoTiempo +
                          particle.az * std::pow(Constantes::pasoTiempo, 2);

            particle.vx = particle.hvx + (particle.ax * Constantes::pasoTiempo) * Constantes::factor05;
            particle.vy = particle.hvy + (particle.ay * Constantes::pasoTiempo) * Constantes::factor05;
            particle.vz = particle.hvz + (particle.az * Constantes::pasoTiempo) * Constantes::factor05;

            particle.hvx = particle.hvx + particle.ax * Constantes::pasoTiempo;
            particle.hvy = particle.hvy + particle.ay * Constantes::pasoTiempo;
            particle.hvz = particle.hvz + particle.az * Constantes::pasoTiempo;
        }
    }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la X (si hay interaccion)
void InteractionLimitX(Particle& particle, int cx, double numberblocksx){
    if (cx == 0){
        double const deltax = particle.px - Constantes::limInferior.x;
        if (deltax < 0){
            particle.px = Constantes::limInferior.x - deltax;
            particle.vx = -particle.vx;
            particle.hvx = -particle.hvx;
        }
    }
    else if(cx == static_cast<int>(numberblocksx - 1)){
        double const deltax = Constantes::limSuperior.x - particle.px;
        if (deltax < 0){
            particle.px = Constantes::limSuperior.x + deltax;
            particle.vx = -particle.vx;
            particle.hvx = -particle.hvx;
        }
    }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la Y (si hay interaccion)
void InteractionLimitY(Particle& particle, int cy, double numberblocksy){
    if (cy == 0){
        double const deltay = particle.py - Constantes::limInferior.y;
        if (deltay < 0){
            particle.py = Constantes::limInferior.y - deltay;
            particle.vy = -particle.vy;
            particle.hvy = -particle.hvy;
        }
    }
    else if(cy == static_cast<int>(numberblocksy - 1)){
        double const deltay = Constantes::limSuperior.y - particle.py;
        if (deltay < 0){
            particle.py = Constantes::limSuperior.y + deltay;
            particle.vy = -particle.vy;
            particle.hvy = -particle.hvy;
        }
    }
}

// Funcion que hace la interaccion con el borde del recinto respecto a la Z (si hay interaccion)
void InteractionLimitZ(Particle& particle, int cz, double numberblocksz){
    if (cz == 0){
        double const deltaz = particle.pz - Constantes::limInferior.z;
        if (deltaz < 0){
            particle.pz = Constantes::limInferior.z - deltaz;
            particle.vz = -particle.vz;
            particle.hvz = -particle.hvz;
        }
    }
    else if(cz == static_cast<int>(numberblocksz - 1)){
        double const deltaz = Constantes::limSuperior.z - particle.pz;
        if (deltaz < 0){
            particle.pz = Constantes::limSuperior.z + deltaz;
            particle.vz = -particle.vz;
            particle.hvz = -particle.hvz;
        }
    }
}

// Funcion para las interacciones con los límites del recinto de una particula
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz) {
    for (auto& block : blocks) {
        for (auto& particula : block.particles) {
            /* si un bloque tiene cx==0 o cx== numbrblocks-1 se actualiza el ax de todas las particulas de ese bloque, llamando a handleXCollisions*/
            if (block.cx == 0 || block.cx == static_cast<int>(numberblocksx) - 1) {
                InteractionLimitZ(particula, block.cx, numberblocksx);
            }
            /* si un bloque tiene cy==0 o cy== numbrblocks-1 se actualiza el ay de todas las particulas de ese bloque, llamando a handleYCollisions*/
            if (block.cy == 0 || block.cy == static_cast<int>(numberblocksy) - 1) {
                InteractionLimitY(particula, block.cy, numberblocksy);
            }
            /* si un bloque tiene cz==0 o cz== numbrblocks-1 se actualiza el az de todas las particulas de ese bloque, llamando a handleZCollisions*/
            if (block.cz == 0 || block.cz == static_cast<int>(numberblocksz) - 1) {
                InteractionLimitX(particula, block.cz, numberblocksz);
            }
        }
    }
}

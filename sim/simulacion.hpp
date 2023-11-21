#ifndef FLUID_SIMULACION_HPP
#define FLUID_SIMULACION_HPP

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
#include <tuple>
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include "sim/progargs.hpp"

// Funcion llamada una vez por iteracion, llama al resto de funciones
std::vector<Block> ejecutarIteraciones(Grid& malla, Argumentos& argumentos, double smoothingLength, double particleMass);

// Inicializacion de la densidad y las aceleraciones
void initAccelerations(std::vector<Block>& blocks);

// Incremento de densidades
void incrementDensities(std::vector<Block>& blocks, double h, Grid& malla);
void comprobarParticula2Dens(std::vector<Block>& blocks, Particle& particle1, double hsq, int neighborIndex);

// Transformacion de densidades
void transformDensities(std::vector<Block>& blocks, double h, double factorDensTransf);

// Transferencia de aceleraciones
void transferAcceleration(std::vector<Block>& blocks, Constantes::ConstAccTransf& constAccTransf, Grid& malla);
void comprobarParticula2Acc(std::vector<Block>& blocks, Particle& particle1, Constantes::ConstAccTransf& constAccTransf, int neighborIndex);
std::tuple<double, double, double> calcularDeltas(Particle& particle1, Particle& particle2, Constantes::ConstAccTransf& constAccTransf, double distSquared);

// Colisiones de particulas
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);

// Movimiento de particulas
void particlesMovement(std::vector<Block>& blocks);

// Interaciones con los limites del recinto
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);


#endif //FLUID_SIMULACION_HPP

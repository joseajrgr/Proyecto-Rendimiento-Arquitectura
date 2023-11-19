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
#include "sim/grid.hpp"
#include "sim/constantes.hpp"
#include "sim/progargs.hpp"

// Relativo a la simulacion
std::vector<Block> ejecutarIteraciones(Grid& malla, Argumentos& argumentos, double smoothingLength, double particleMass);
void initAccelerations(std::vector<Block>& blocks);
void incrementDensities(std::vector<Block>& blocks, double h, Grid& malla);
void transformDensities(std::vector<Block>& blocks, double h, double factorDensTransf);
void transferAcceleration(std::vector<Block>& blocks, double h, Constantes::ConstAccTransf& constAccTransf, Grid& malla);
void particleColissions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);
void particlesMovement(std::vector<Block>& blocks);
void limitInteractions(std::vector<Block>& blocks, double numberblocksx, double numberblocksy, double numberblocksz);


#endif //FLUID_SIMULACION_HPP

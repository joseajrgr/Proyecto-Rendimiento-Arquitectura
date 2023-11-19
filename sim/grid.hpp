#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

#include <vector>
#include "block.hpp"  // Incluimos block.hpp para tener acceso a la estructura "Punto"

struct Fluid {
    float particlespermeter = 0;  // Partículas por metro
    int numberparticles = 0;  // Número de partículas
    std::vector<Particle> particles;
};

class Grid {
public:
    [[nodiscard]] inline double getNumberblocksx() const { return numberblocksx; }
    [[nodiscard]] inline double getNumberblocksy() const { return numberblocksy; }
    [[nodiscard]] inline double getNumberblocksz() const { return numberblocksz; }
    [[nodiscard]] inline double getNumBlocks() const { return numBlocks; }
    [[nodiscard]] inline double getMeshx() const { return meshx; }
    [[nodiscard]] inline double getMeshy() const { return meshy; }
    [[nodiscard]] inline double getMeshz() const { return meshz; }

    Grid(const Punto& bmin, const Punto& bmax);
    void dividirEnBloques(double smoothingLength);
    std::pair<double, double> simular_malla(const Fluid &fluid);
    void reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &blocks) const;
    void reposicionarParticulasBloque(std::vector<Block> &blocks) const;
    static std::vector<Particle>::iterator comprobarBloque(Block &block, Particle &particula, Block &newBlock, std::vector<Particle>::iterator iterador);
    [[nodiscard]] inline const std::vector<Block>& getBlocks() const { return blocks; }

private:
    double numberblocksx;
    double numberblocksy;
    double numberblocksz;
    double numBlocks;
    double meshx;
    double meshy;
    double meshz;
    double invmeshx;
    double invmeshy;
    double invmeshz;
    Punto bmin; // Limite inferior del recinto
    Punto bmax; // Limite superior del recinto
    std::vector<Block> blocks;
};



#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

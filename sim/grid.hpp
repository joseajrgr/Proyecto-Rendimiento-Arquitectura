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
        double numberblocksx;
        double numberblocksy;
        double numberblocksz;
        double numBlocks;
        double meshx;
        double meshy;
        double meshz;

        Grid(const Punto& bmin, const Punto& bmax);
        void dividirEnBloques(double smoothingLength);
        void reposicionarParticulas(Fluid &fluid, std::vector<Block> &blocks);
        const std::vector<Block>& getBlocks() const;

    private:
        Punto bmin; // Limite inferior del recinto
        Punto bmax; // Limite superior del recinto
        std::vector<Block> blocks;
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

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
        [[nodiscard]] double getNumberblocksx() const;
        [[nodiscard]] double getNumberblocksy() const;
        [[nodiscard]] double getNumberblocksz() const;
        [[nodiscard]] double getNumBlocks() const;
        [[nodiscard]] double getMeshx() const;
        [[nodiscard]] double getMeshy() const;
        [[nodiscard]] double getMeshz() const;

        Grid(const Punto& bmin, const Punto& bmax);
        void dividirEnBloques(double smoothingLength);
        void reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &blocks) const;
        void reposicionarParticulasBloque(std::vector<Block> &blocks) const;
        static std::vector<Particle>::iterator comprobarBloque(Block &block, Particle &particula, Block &newBlock, std::vector<Particle>::iterator iterador);
        [[nodiscard]] const std::vector<Block>& getBlocks() const;

    private:
        double numberblocksx;
        double numberblocksy;
        double numberblocksz;
        double numBlocks;
        double meshx;
        double meshy;
        double meshz;
        Punto bmin; // Limite inferior del recinto
        Punto bmax; // Limite superior del recinto
        std::vector<Block> blocks;
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

#include <vector>
#include "block.hpp"  // Incluimos block.hpp para tener acceso a la estructura "Punto"

struct Fluid {
    float particlespermeter = 0;  // Particulas por metro
    int numberparticles = 0;  // Numero total de particulas
    std::vector<Particle> particles;
};

class Grid {
public:
    [[nodiscard]] inline double getNumberblocksx() const { return numberblocksx; }

    [[nodiscard]] inline double getNumberblocksy() const { return numberblocksy; }

    [[nodiscard]] inline double getNumberblocksz() const { return numberblocksz; }

    [[nodiscard]] inline double getMeshx() const { return meshx; }

    [[nodiscard]] inline double getMeshy() const { return meshy; }

    [[nodiscard]] inline double getMeshz() const { return meshz; }

    // Relativo a dividir la malla
    Grid(const Punto &bmin, const Punto &bmax);

    void dividirEnBloques(double smoothingLength);

    std::pair<double, double> simular_malla(const Fluid &fluid);

    void reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &bloques) const;

    void reposicionarParticulasBloque(std::vector<Block> &bloques) const;

    [[nodiscard]] inline const std::vector<Block> &getBlocks() const { return blocks; }

private:
    double numberblocksx{0.0};
    double numberblocksy{0.0};
    double numberblocksz{0.0};
    double numBlocks{0.0};
    double meshx{0.0};
    double meshy{0.0};
    double meshz{0.0};
    double invmeshx{0.0};
    double invmeshy{0.0};
    double invmeshz{0.0};
    Punto bmin; // Limite inferior del recinto
    Punto bmax; // Limite superior del recinto
    std::vector<Block> blocks;

    void dividirVectorBloques(std::vector<Block> &nuevosBloques) const;
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

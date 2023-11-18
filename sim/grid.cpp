#include <iostream>
#include <cmath>
#include "grid.hpp"

// Inicializar los valores para el constructor
Grid::Grid(const Punto& bmin, const Punto& bmax) : numberblocksx(0.0), numberblocksy(0.0),
                                                   numberblocksz(0.0), numBlocks(0.0), meshx(0.0), meshy(0.0), meshz(0.0), bmin(bmin), bmax(bmax) {}


// Getters para los valores privados


// Funcion quue divide la malla en bloques
void Grid::dividirEnBloques(double smoothingLength) {
    if (smoothingLength <= 0.0) {
        //excepción si smoothinglength es menor o igual que 0
        throw std::invalid_argument("Smoothing length must be a positive value.");
    }
    numberblocksx = floor(((bmax.x - bmin.x)) / smoothingLength);
    numberblocksy = floor(((bmax.y - bmin.y)) / smoothingLength);
    numberblocksz = floor(((bmax.z - bmin.z)) / smoothingLength);
    numBlocks = numberblocksx * numberblocksy * numberblocksz;
    meshx = (bmax.x - bmin.x) / numberblocksx;
    meshy = (bmax.y - bmin.y) / numberblocksy;
    meshz = (bmax.z - bmin.z) / numberblocksz;
    // Limpia el vector de blocks
    blocks.clear();

    for (int k = 0; k < numberblocksx; ++k) {
        for (int j = 0; j < numberblocksy; ++j) {
            for (int i = 0; i < numberblocksz; ++i) {

                // Añade el bloque al final del vector
                const int blockId = static_cast<int>(i + j * numberblocksx + k * numberblocksx * numberblocksy);
                blocks.emplace_back(blockId, k ,j, i);
            }
        }
    }
}


// Funcion que reposiciona las particulas en la primera iteracion, a partir del fluido
void Grid::reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &bloques) const {
    // Precalculamos valores para optimizar el rendimiento
    const double inv_meshx = 1.0 / meshx;
    const double inv_meshy = 1.0 / meshy;
    const double inv_meshz = 1.0 / meshz;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        Particle &particula = fluid.particles[i];

        const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * inv_meshx), static_cast<int>(numberblocksx) - 1));
        const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * inv_meshy), static_cast<int>(numberblocksy) - 1));
        const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * inv_meshz), static_cast<int>(numberblocksz) - 1));

        const int blockId = static_cast<int>(indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy);
        Block &block = bloques[blockId];
        particula.idBloque = block.id;
        block.addParticle(particula);
    }
}


// Funcion que reposiciona las particulas almacenadas en los bloques
void Grid::reposicionarParticulasBloque(std::vector<Block> &bloques) const {
    // Precalculamos valores para optimizar el rendimiento
    const double inv_meshx = 1.0 / meshx;
    const double inv_meshy = 1.0 / meshy;
    const double inv_meshz = 1.0 / meshz;

    for (Block &block : bloques) {
        auto iterador = block.particles.begin();
        while (iterador != block.particles.end()) {
            Particle &particula = *iterador;

            const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * inv_meshx), static_cast<int>(numberblocksx) - 1));
            const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * inv_meshy), static_cast<int>(numberblocksy) - 1));
            const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * inv_meshz), static_cast<int>(numberblocksz) - 1));

            const int blockId = static_cast<int>(indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy);
            Block &newBlock = bloques[blockId];
            iterador = comprobarBloque(block, particula, newBlock, iterador);
        }
    }
}


// Funcion que comprueba si el nuevo id del bloque es igual al bloque actual y mueve la particula si es necesario
std::vector<Particle>::iterator Grid::comprobarBloque(Block &block, Particle &particula, Block &newBlock, std::vector<Particle>::iterator iterador) {
    if (newBlock.id != block.id) {
        particula.idBloque = newBlock.id;
        newBlock.addParticle(particula);
        return block.particles.erase(iterador);  // Elimina la partícula del bloque antiguo
    }
    return ++iterador;
}


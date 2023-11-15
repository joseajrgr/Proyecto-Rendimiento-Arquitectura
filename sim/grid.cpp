#include <iostream>
#include <algorithm>
#include <cmath>
#include "grid.hpp"
Grid::Grid(const Punto& bmin, const Punto& bmax) : bmin(bmin), bmax(bmax) {}

void Grid::dividirEnBloques(double smoothingLength) {
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
                const int blockId = i + j * numberblocksx + k * numberblocksx * numberblocksy;
                blocks.emplace_back(blockId, k ,j, i);
            }
        }
    }
}



void Grid::reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &bloques) {
    // Precalculamos valores para optimizar el rendimiento
    const double inv_meshx = 1.0 / meshx;
    const double inv_meshy = 1.0 / meshy;
    const double inv_meshz = 1.0 / meshz;

    for (int i = 0; i < fluid.numberparticles; ++i) {
        Particle &particula = fluid.particles[i];

        const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * inv_meshx), static_cast<int>(numberblocksx) - 1));
        const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * inv_meshy), static_cast<int>(numberblocksy) - 1));
        const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * inv_meshz), static_cast<int>(numberblocksz) - 1));

        const int blockId = indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy;
        Block &block = bloques[blockId];
        particula.idBloque = block.id;
        block.addParticle(particula);
        // std::cout << "La partícula " << i << " está en el bloque " << block.id << " x: "<< block.cx << " y: "<< block.cy << " z: "<< block.cz <<'\n';
    }
}

void Grid::reposicionarParticulasBloque(std::vector<Block> &bloques) {
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

            const int blockId = indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy;
            Block &newBlock = bloques[blockId];
            if (newBlock.id != block.id) {
                particula.idBloque = newBlock.id;
                newBlock.addParticle(particula);
                iterador = block.particles.erase(iterador);  // Elimina la partícula del bloque antiguo
            } else {
                ++iterador;
            }
        }
    }
}

const std::vector<Block>& Grid::getBlocks() const {
    return blocks;
}

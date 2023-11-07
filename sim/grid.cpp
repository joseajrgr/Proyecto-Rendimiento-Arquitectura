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

    // Generar los bloques y almacenarlos en el vector de blocks
    blocks.clear();
    int blockId = 0;
    for (int i = 0; i < numberblocksx; ++i) {
        for (int j = 0; j < numberblocksy; ++j) {
            for (int k = 0; k < numberblocksz; ++k) {
                Punto blockBmin = {bmin.x + i * smoothingLength, bmin.y + j * smoothingLength, bmin.z + k * smoothingLength};
                Punto blockBmax = {blockBmin.x + smoothingLength, blockBmin.y + smoothingLength, blockBmin.z + smoothingLength};
                blocks.emplace_back(blockId++, blockBmin, blockBmax , i ,j, k);
            }
        }
    }
}

void Grid::reposicionarParticulas(const Fluid &fluid) {
    for (int i = 0; i < fluid.numberparticles; ++i) {
        const Particle &particula = fluid.particles[i];

        int ix = floor((particula.px - bmin.x) / meshx);
        int iy = floor((particula.py - bmin.y) / meshy);
        int iz = floor((particula.pz - bmin.z) / meshz);

        ix = std::max(0, std::min(ix, static_cast<int>(numberblocksx) - 1));
        iy = std::max(0, std::min(iy, static_cast<int>(numberblocksy) - 1));
        iz = std::max(0, std::min(iz, static_cast<int>(numberblocksz) - 1));

        Block &block = blocks[ix * numberblocksy * numberblocksz + iy * numberblocksz + iz];
        block.addParticle(particula);
        std::cout << "La partícula " << i << " está en el bloque " << block.id << '\n';
    }
}

const std::vector<Block>& Grid::getBlocks() const {
    return blocks;
}

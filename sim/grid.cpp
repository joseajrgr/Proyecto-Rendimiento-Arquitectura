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
                blocks.emplace_back(blockId++, blockBmin, blockBmax);
            }
        }
    }
}

const std::vector<Block>& Grid::getBlocks() const {
    return blocks;
}

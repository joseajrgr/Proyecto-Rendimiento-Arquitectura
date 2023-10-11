//
// Created by quinc on 03/10/2023.
//

#include <cmath>
#include "grid.hpp"

Grid::Grid(const Punto& bmin, const Punto& bmax) : bmin(bmin), bmax(bmax) {}

void Grid::dividirEnBloques(double smoothingLength) {
    numberblocksx = floor((bmax.x - bmin.x) / smoothingLength);
    numberblocksy = floor((bmax.y - bmin.y) / smoothingLength);
    numberblocksz = floor((bmax.z - bmin.z) / smoothingLength);
    numBlocks = numberblocksx * numberblocksy * numberblocksz;
    meshx = (bmax.x - bmin.x) / numberblocksx;
    meshy = (bmax.y - bmin.y) / numberblocksy;
    meshz = (bmax.z - bmin.z) / numberblocksz;
}

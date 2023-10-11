//
// Created by quinc on 03/10/2023.
//

#include "block.hpp"

Block::Block(const Punto& minPoint, const Punto& maxPoint) : minPoint(minPoint), maxPoint(maxPoint) {}

bool Block::particula_en_bloque(const Punto& point) const {
    return (point.x >= minPoint.x && point.x <= maxPoint.x &&
            point.y >= minPoint.y && point.y <= maxPoint.y &&
            point.z >= minPoint.z && point.z <= maxPoint.z);
}

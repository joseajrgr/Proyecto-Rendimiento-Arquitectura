#include "block.hpp"

Block::Block(int id, const Punto& minPoint, const Punto& maxPoint, int cx , int cy , int cz) : id(id), minPoint(minPoint), maxPoint(maxPoint) , cx(cx) ,cy(cy), cz(cz) {}

bool Block::particula_en_bloque(const Punto& point) const {
    return (point.x >= minPoint.x && point.x <= maxPoint.x &&
            point.y >= minPoint.y && point.y <= maxPoint.y &&
            point.z >= minPoint.z && point.z <= maxPoint.z);
}

void Block::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

const std::vector<Particle>& Block::getParticles() const {
    return particles;
}

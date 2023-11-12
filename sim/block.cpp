#include "block.hpp"
#include "sim/constantes.hpp"



Block::Block(int id, const Punto& minPoint, const Punto& maxPoint, int cx , int cy , int cz) : id(id), minPoint(minPoint), maxPoint(maxPoint) , cx(cx) ,cy(cy), cz(cz) {}

void Block::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

const std::vector<Particle>& Block::getParticles() const {
    return particles;
}

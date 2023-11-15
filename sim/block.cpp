#include "block.hpp"



Block::Block(int id, int cx , int cy , int cz) : id(id), cx(cx) ,cy(cy), cz(cz) {}

void Block::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

const std::vector<Particle>& Block::getParticles() const {
    return particles;
}

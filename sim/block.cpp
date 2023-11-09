#include "block.hpp"
#include "sim/constantes.hpp"

Particle::Particle() noexcept{
    id = 0;
    px = py = pz = 0.0;
    hvx = hvy = hvz = 0.0;
    vx = vy = vz = 0.0;
    ax = Constantes::gravedad.x;
    ay = Constantes::gravedad.y;
    az = Constantes::gravedad.z;
    density = 0.0;
}

Block::Block(int id, const Punto& minPoint, const Punto& maxPoint, int cx , int cy , int cz) : id(id), minPoint(minPoint), maxPoint(maxPoint) , cx(cx) ,cy(cy), cz(cz) {}

bool Block::particula_en_bloque(const Particle& particle) const {
    return (particle.px >= minPoint.x && particle.px <= maxPoint.x &&
            particle.py >= minPoint.y && particle.py <= maxPoint.y &&
            particle.pz >= minPoint.z && particle.pz <= maxPoint.z);
}

void Block::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

const std::vector<Particle>& Block::getParticles() const {
    return particles;
}

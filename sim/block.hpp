#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP

#include <vector>

struct Punto {
    double x, y, z;
};

struct Particle {
    int id;
    float px, py, pz; // Coordenadas de su posicion
    float hvx, hvy, hvz; // Coordenadas del gradiente de velocidad
    float vx, vy, vz; // Coordenadas de la velocidad
    float ax, ay, az; // Coordenadas de la aceleración
    float density;
    Particle()noexcept;
};

class Block {
    public:
        int id;
        Punto minPoint;
        Punto maxPoint;
        int cx, cy, cz; // Indice del bloque en cada coordenada
        std::vector<Particle> particles;

        Block(int id, const Punto& minPoint, const Punto& maxPoint, int cx, int cy, int cz);
        void addParticle(const Particle& particle);
        const std::vector<Particle>& getParticles() const;
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP

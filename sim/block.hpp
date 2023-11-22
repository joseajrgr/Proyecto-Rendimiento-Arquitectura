#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP

#include <vector>


struct Punto {
    double x, y, z;
};

struct Particle {
    int id;
    int idBloque;
    double px, py, pz; // Coordenadas de su posicion
    double hvx, hvy, hvz; // Coordenadas del gradiente de velocidad
    double vx, vy, vz; // Coordenadas de la velocidad
    double ax, ay, az; // Coordenadas de la aceleración
    double density;

    // Metodo para comparar que dos particulas son iguales. Este metodo se usara en los tests
    bool operator==(const Particle &other) const {
        return id == other.id &&
               idBloque == other.idBloque &&
               px == other.px && py == other.py && pz == other.pz &&
               hvx == other.hvx && hvy == other.hvy && hvz == other.hvz &&
               vx == other.vx && vy == other.vy && vz == other.vz &&
               ax == other.ax && ay == other.ay && az == other.az &&
               density == other.density;
    }
};

class Block {
public:
    Block();

    int id;
    int cx, cy, cz; // Indice del bloque en cada coordenada
    std::vector<Particle> particles;

    Block(int id, int cx, int cy, int cz);

    void addParticle(const Particle &particle);
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP

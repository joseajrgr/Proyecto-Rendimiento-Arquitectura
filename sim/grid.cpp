#include <iostream>
#include <cmath>
#include "constantes.hpp"
#include "grid.hpp"


// Inicializar los valores para el constructor
Grid::Grid(const Punto &bmin, const Punto &bmax) : bmin(bmin),
                                                   bmax(bmax) {}


// Funcion quue divide la malla en bloques
void Grid::dividirEnBloques(double smoothingLength) {
    // Se comprueba si la longitud de suavizado es positiva
    if (smoothingLength <= 0.0) {
        throw std::invalid_argument("Smoothing length must be a positive value.");
    }

    // Variables con el numero de bloques (por coordenadas y en total)
    numberblocksx = floor(((bmax.x - bmin.x)) / smoothingLength);
    numberblocksy = floor(((bmax.y - bmin.y)) / smoothingLength);
    numberblocksz = floor(((bmax.z - bmin.z)) / smoothingLength);
    numBlocks = numberblocksx * numberblocksy * numberblocksz;

    // Variables con la longitud en cada coordenada de los bloques
    meshx = (bmax.x - bmin.x) / numberblocksx;
    meshy = (bmax.y - bmin.y) / numberblocksy;
    meshz = (bmax.z - bmin.z) / numberblocksz;

    // Variables con el valor inverso de las longitudes (muy utilizado al reposicionar)
    invmeshx = 1 / meshx;
    invmeshy = 1 / meshy;
    invmeshz = 1 / meshz;

    dividirVectorBloques(blocks);
}


// Funcion que simula la malla
std::pair<double, double> Grid::simular_malla(const Fluid &fluid) {
    // Calcula la longitud de suavizado y la masa de las particulas
    const double smoothingLength = Constantes::multRadio / fluid.particlespermeter;
    const double particleMass = std::pow(10.0, 3.0) / std::pow(fluid.particlespermeter, 3.0);

    dividirEnBloques(smoothingLength);

    // Muestra los valores por pantalla en el formato requerido
    std::cout << "Number of particles: " << fluid.numberparticles << "\n";
    std::cout << "Particles per meter: " << fluid.particlespermeter << "\n";
    std::cout << "Smoothing length: " << smoothingLength << "\n";
    std::cout << "Particle mass: " << particleMass << "\n";
    std::cout << "Grid size: " << numberblocksx << " x " << numberblocksy << " x " << numberblocksz << "\n";
    std::cout << "Number of blocks: " << numBlocks << "\n";
    std::cout << "Block size: " << meshx << " x " << meshy << " x " << meshz << "\n";

    return std::make_pair(smoothingLength, particleMass);
}


// Funcion que reposiciona las particulas en la primera iteracion, a partir del fluido
void Grid::reposicionarParticulasFluid(Fluid &fluid, std::vector<Block> &bloques) const {
    for (int i = 0; i < fluid.numberparticles; ++i) {
        Particle &particula = fluid.particles[i];

        const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * invmeshx),
                                                 static_cast<int>(numberblocksx) - 1));
        const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * invmeshy),
                                                 static_cast<int>(numberblocksy) - 1));
        const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * invmeshz),
                                                 static_cast<int>(numberblocksz) - 1));

        const int blockId = static_cast<int>(indicez + indicey * numberblocksz +
                                             indicex * numberblocksz * numberblocksy);
        Block &block = bloques[blockId];
        particula.idBloque = block.id;
        block.addParticle(particula);
    }
}


// Funcion que reposiciona las particulas almacenadas en los bloques
void Grid::reposicionarParticulasBloque(std::vector<Block> &bloques) const {
    // Crea una copia vacia de los bloques con el mismo tamaño que "bloques"
    std::vector<Block> nuevosBloques(bloques.size());
    dividirVectorBloques(nuevosBloques);

    for (const Block &block: bloques) {
        for (Particle particula: block.particles) {
            const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * invmeshx),
                                                     static_cast<int>(numberblocksx) - 1));
            const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * invmeshy),
                                                     static_cast<int>(numberblocksy) - 1));
            const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * invmeshz),
                                                     static_cast<int>(numberblocksz) - 1));

            // Con los indices de las coordenadas calcula el indice del bloque y la incluye en este
            const int blockId = static_cast<int>(indicez + indicey * numberblocksz +
                                                 indicex * numberblocksz * numberblocksy);
            Block &newBlock = nuevosBloques[blockId];
            particula.idBloque = newBlock.id;
            newBlock.addParticle(particula);
        }
    }
    // Reemplaza los bloques originales con los nuevos bloques
    bloques = std::move(nuevosBloques);
}

// Funcion que genera cada bloque individual en el vector bloques
void Grid::dividirVectorBloques(std::vector<Block> &nuevosBloques) const {
    int bloqueId = -1;
    nuevosBloques.clear();

    for (int k = 0; k < numberblocksx; ++k) {
        for (int j = 0; j < numberblocksy; ++j) {
            for (int i = 0; i < numberblocksz; ++i) {

                // Añade el bloque al final del vector
                bloqueId += 1;
                nuevosBloques.emplace_back(bloqueId, k, j, i);
            }
        }
    }
}





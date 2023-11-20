#include <iostream>
#include <cmath>
#include "constantes.hpp"
#include "grid.hpp"


// Inicializar los valores para el constructor
Grid::Grid(const Punto& bmin, const Punto& bmax) : numberblocksx(0.0), numberblocksy(0.0),
                                                   numberblocksz(0.0), numBlocks(0.0), meshx(0.0), meshy(0.0), meshz(0.0), bmin(bmin), bmax(bmax) {}


// Funcion quue divide la malla en bloques
void Grid::dividirEnBloques(double smoothingLength) {
    if (smoothingLength <= 0.0) {
        //excepción si smoothinglength es menor o igual que 0
        throw std::invalid_argument("Smoothing length must be a positive value.");
    }
    numberblocksx = floor(((bmax.x - bmin.x)) / smoothingLength);
    numberblocksy = floor(((bmax.y - bmin.y)) / smoothingLength);
    numberblocksz = floor(((bmax.z - bmin.z)) / smoothingLength);
    numBlocks = numberblocksx * numberblocksy * numberblocksz;
    meshx = (bmax.x - bmin.x) / numberblocksx;
    meshy = (bmax.y - bmin.y) / numberblocksy;
    meshz = (bmax.z - bmin.z) / numberblocksz;
    invmeshx=1/meshx;
    invmeshy=1/meshy;
    invmeshz=1/meshz;

    // Limpia el vector de blocks
    blocks.clear();
    int blockId = -1;

    for (int k = 0; k < numberblocksx; ++k) {
        for (int j = 0; j < numberblocksy; ++j) {
            for (int i = 0; i < numberblocksz; ++i) {

                // Añade el bloque al final del vector
                blockId += 1;
                blocks.emplace_back(blockId, k ,j, i);
            }
        }
    }
}


// Funcion que simula la malla
std::pair<double, double> Grid::simular_malla(const Fluid &fluid) {
    const double smoothingLength = Constantes::multRadio / fluid.particlespermeter;
    const double particleMass = std::pow(10.0, 3.0) / std::pow(fluid.particlespermeter, 3.0);

    dividirEnBloques(smoothingLength);

    // Mostrar los valores por pantalla en el formato requerido
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

        const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * invmeshx), static_cast<int>(numberblocksx) - 1));
        const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * invmeshy), static_cast<int>(numberblocksy) - 1));
        const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * invmeshz), static_cast<int>(numberblocksz) - 1));

        const int blockId = static_cast<int>(indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy);
        Block &block = bloques[blockId];
        particula.idBloque = block.id;
        block.addParticle(particula);
    }
}


// Funcion que reposiciona las particulas almacenadas en los bloques
void Grid::reposicionarParticulasBloque(std::vector<Block> &bloques) const {
    // Crear una copia vacía de los bloques con el mismo tamaño que bloques
    std::vector<Block> nuevosBloques(bloques.size());
    int bloqueId = -1;
    nuevosBloques.clear();
    for (int k = 0; k < numberblocksx; ++k) {
        for (int j = 0; j < numberblocksy; ++j) {
            for (int i = 0; i < numberblocksz; ++i) {

                // Añade el bloque al final del vector
                bloqueId += 1;
                nuevosBloques.emplace_back(bloqueId, k ,j, i);
            }
        }
    }
    for (const Block &block : bloques) {
        for (Particle particula : block.particles) {
            const int indicex = std::max(0, std::min(static_cast<int>((particula.px - bmin.x) * invmeshx), static_cast<int>(numberblocksx) - 1));
            const int indicey = std::max(0, std::min(static_cast<int>((particula.py - bmin.y) * invmeshy), static_cast<int>(numberblocksy) - 1));
            const int indicez = std::max(0, std::min(static_cast<int>((particula.pz - bmin.z) * invmeshz), static_cast<int>(numberblocksz) - 1));

            const int blockId = static_cast<int>(indicex + indicey * numberblocksx + indicez * numberblocksx * numberblocksy);
            Block &newBlock = nuevosBloques[blockId];
            // std::cout << blockId << " " << newBlock.id << "\n";
            particula.idBloque = newBlock.id;
            newBlock.addParticle(particula);
        }
    }

    // Reemplazar los bloques originales con los nuevos bloques
    bloques = std::move(nuevosBloques);
}


// Funcion que comprueba si el nuevo id del bloque es igual al bloque actual y mueve la particula si es necesario
std::vector<Particle>::iterator Grid::comprobarBloque(Block &block, Particle &particula, Block &newBlock, std::vector<Particle>::iterator iterador) {
    if (newBlock.id != block.id) {
        particula.idBloque = newBlock.id;
        newBlock.addParticle(particula);
        return block.particles.erase(iterador);  // Elimina la partícula del bloque antiguo
    }
    return ++iterador;
}

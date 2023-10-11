#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

struct Particle {
    float px, py, pz;
    //float hvx, hvy, hvz;
    //float vx, vy, vz;
};
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments. Usage: " << argv[0] << " <nts> <inputfile> <outputfile>\n";
        return -1;
    }
    const int iteraciones = std::stoi(argv[1]);
    try {
        if (iteraciones<0){
            std::cerr << "Error: Invalid number of time steps.\n";
            return -2;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: time steps must be numeric.\n";
        return -1;
    }

    const std::string archivoEntrada = argv[2];
    const std::string archivoSalida = argv[3];

    // Leer el archivo de entrada
    std::ifstream input(archivoEntrada, std::ios::binary);
    if (!input) {
        std::cerr << "Error: Cannot open " << archivoEntrada << " for reading\n";
        return -3;
    }

    float particlespermetro =0.0;
    int numberparticles = 0;
    input.read(reinterpret_cast<char*>(&particlespermetro), sizeof(float));
    input.read(reinterpret_cast<char*>(&numberparticles), sizeof(int));

    std::vector<Particle> particles(numberparticles);
    input.read(reinterpret_cast<char*>(particles.data()), numberparticles * sizeof(Particle));
    input.close();


    // Verificar si el número de partículas leídas coincide con numberparticles OPCIONAL
    const int numParticulasLeidas = particles.size();
    if (numParticulasLeidas != numberparticles) {
        std::cerr << "Error: El número de partículas leídas (" << numParticulasLeidas << ") no coincide con numberparticles (" << numberparticles << ").\n";
        return -5;
    }

    // Calcular otros valores requeridos
    const double smoothingLength = 1.695 / particlespermetro;
    const double particleMass = std::pow(10, 3) / std::pow(particlespermetro, 3);

    // Calcular el tamaño de la malla (grid size) y el número de bloques
    const float xmax = 0.065;
    const float ymax = 0.1;
    const float zmax = 0.065;
    const float xmin = -0.065;
    const float ymin = -0.08 ;
    const float zmin = -0.065;
    const float numberblocksx = std::floor((xmax - xmin) / smoothingLength);
    const float numberblocksy = std::floor((ymax - ymin) / smoothingLength);
    const float numberblocksz = std::floor((zmax - zmin) / smoothingLength);

    const float numBlocks = numberblocksx * numberblocksy * numberblocksz;
    const float meshx = (xmax - xmin) / numberblocksx;
    const float meshy = (ymax - ymin) / numberblocksy;

    const float meshz = (zmax - zmin) / numberblocksz;

    // Mostrar los valores por pantalla en el formato requerido
    std::cout << "Number of particles: " << numberparticles << "\n";
    std::cout << "Particles per meter: " << particlespermetro << "\n";
    std::cout << "Smoothing length: " << smoothingLength << "\n";
    std::cout << "Particle mass: " << particleMass << "\n";
    std::cout << "Grid size: " << numberblocksx << " x " << numberblocksy << " x " << numberblocksz << "\n";
    std::cout << "Number of blocks: " << numBlocks << "\n";
    std::cout << "Block size: " << meshx << " x " << meshy << " x " << meshz << "\n";

    // Realizar la simulación
    for (int iter = 0; iter < iteraciones; ++iter) {
        std::cout << "Iteración " << iter + 1 << ":\n";
        for (int i = 0; i < numberparticles; ++i) {
            const Particle& particula = particles[i];
            // Aquí se puede hacer la simulación para la partícula actual
            std::cout << "Partícula " << i + 1 << ": px=" << particula.px << " py=" << particula.py << " pz=" << particula.pz << "\n";

        }
        std::cout << "\n";
    }

    // Guardar el estado final del fluido en un archivo de salida
    std::ofstream output(archivoSalida, std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot open " << archivoSalida << " for writing\n";
        return -4;
    }

    output.write(reinterpret_cast<const char*>(&particlespermetro), sizeof(float));
    output.write(reinterpret_cast<const char*>(&numberparticles), sizeof(int));
    output.write(reinterpret_cast<const char*>(particles.data()), numberparticles * sizeof(Particle));


    output.close();
    std::cout << "Simulación completada. Estado final del fluido guardado en: " << archivoSalida << "\n";

    return 0;
}


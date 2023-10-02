#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Particle {
    float px, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <iteraciones> <archivo_entrada> <archivo_salida>\n";
        return 1;
    }

    int iteraciones = std::stoi(argv[1]);
    std::string archivoEntrada = argv[2];
    std::string archivoSalida = argv[3];

    // Leer el archivo de entrada
    std::ifstream input(archivoEntrada, std::ios::binary);
    if (!input) {
        std::cerr << "Error al abrir el archivo de entrada: " << archivoEntrada << "\n";
        return 1;
    }

    float ppm;
    int np;
    input.read(reinterpret_cast<char*>(&ppm), sizeof(float));
    input.read(reinterpret_cast<char*>(&np), sizeof(int));

    std::vector<Particle> particles(np);
    input.read(reinterpret_cast<char*>(particles.data()), np * sizeof(Particle));
    input.close();

    // Verificar si el número de partículas leídas coincide con np OPCIONAL
    int numParticulasLeidas = particles.size();
    if (numParticulasLeidas != np) {
        std::cerr << "Error: El número de partículas leídas (" << numParticulasLeidas << ") no coincide con np (" << np << ").\n";
        return 1;
    }

    // Realizar la simulación
    for (int iter = 0; iter < iteraciones; ++iter) {
        std::cout << "Iteración " << iter + 1 << ":\n";
        for (int i = 0; i < np; ++i) {
            Particle& particula = particles[i];
            // Aquí se puede hacer la simulación para la partícula actual
            std::cout << "Partícula " << i + 1 << ": px=" << particula.px << " py=" << particula.py << " pz=" << particula.pz << "\n";

        }
        std::cout << "\n";
    }

    // Guardar el estado final del fluido en un archivo de salida
    std::ofstream output(archivoSalida, std::ios::binary);
    if (!output) {
        std::cerr << "Error al abrir el archivo de salida: " << archivoSalida << "\n";
        return 1;
    }

    output.write(reinterpret_cast<char*>(&ppm), sizeof(float));
    output.write(reinterpret_cast<char*>(&np), sizeof(int));
    output.write(reinterpret_cast<char*>(particles.data()), np * sizeof(Particle));
    output.close();

    std::cout << "Simulación completada. Estado final del fluido guardado en: " << archivoSalida << "\n";

    return 0;
}


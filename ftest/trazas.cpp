//
// Created by Raul on 11/11/2023.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

struct Particle {
    int64_t id;
    double posx, posy, posz;
    double hvx, hvy, hvz;
    double velx, vely, vlez;
    double density;
    double accx, accy, accz;
};
struct ParticleInfo {
    Particle particle;
    int blockIndex;
};

void readTraceFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    int numBlocks;
    file.read(reinterpret_cast<char *>(&numBlocks), sizeof(int));
    std::vector<ParticleInfo> particles;
    for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
        int64_t numParticles;
        file.read(reinterpret_cast<char *>(&numParticles), sizeof(int64_t));

        for (int64_t particleIndex = 0; particleIndex < numParticles; ++particleIndex) {
            Particle particle;
            file.read(reinterpret_cast<char *>(&particle.id), sizeof(int64_t));
            file.read(reinterpret_cast<char *>(&particle.posx), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.posy), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.posz), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.hvx), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.hvy), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.hvz), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.velx), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.vely), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.vlez), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.density), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.accx), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.accy), sizeof(double));
            file.read(reinterpret_cast<char *>(&particle.accz), sizeof(double));

            ParticleInfo info;
            info.particle = particle;
            info.blockIndex = blockIndex;
            particles.push_back(info);
        }
    }

    file.close();
    std::sort(particles.begin(), particles.end(), [](const auto &a, const auto &b) {
        return a.particle.id < b.particle.id;
    });


    std::ofstream outFile("salida.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open output file salida.txt" << std::endl;
        return;
    }
    int precision = 30;
    outFile << std::fixed << std::setprecision(precision);
    for (const ParticleInfo &info: particles) {
        const Particle &particle = info.particle;
        outFile << "La partícula " << particle.id << " " << particle.density << " está en el bloque "
                << info.blockIndex << " x: " << particle.posx << " y: " << particle.posy
                << " z: " << particle.posz << " "
                << "     Aceleración: (" << particle.accx << ", " << particle.accy << ", "
                << particle.accz << ") Velocidad: (" << particle.velx << ", " << particle.vely
                << ", " << particle.vlez << ")        Gradiente: (" << particle.hvx << ", "
                << particle.hvy << ", " << particle.hvz << ")" << std::endl;
    }

    outFile.close();
}

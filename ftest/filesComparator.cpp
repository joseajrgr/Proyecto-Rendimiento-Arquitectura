//
// Created by Raul on 11/11/2023.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>



struct Particle {
    int64_t id;
    double px, py, pz;
    double hvx, hvy, hvz;
    double vx, vy, vz;
    double density;
    double ax, ay, az;
};
struct ParticleInfo {
    Particle particle;
    int blockIndex;
};

struct Fluid{
    int particlespermeter;
    int numberparticles;
    std::vector<Particle> particles;
};

void readFldFile(std::ifstream& file, Fluid& fluid){
  file.read(reinterpret_cast<char*>(&fluid.particlespermeter), sizeof(float));
  file.read(reinterpret_cast<char*>(&fluid.numberparticles), sizeof(int));
  fluid.particles.resize(fluid.numberparticles);
  for (int i = 0; i < fluid.numberparticles; ++i) {
    fluid.particles[i].id = i;
    for (double* attr : {&fluid.particles[i].px, &fluid.particles[i].py, &fluid.particles[i].pz,
                          &fluid.particles[i].hvx, &fluid.particles[i].hvy, &fluid.particles[i].hvz,
                          &fluid.particles[i].vx, &fluid.particles[i].vy, &fluid.particles[i].vz}) {
      float temp = 0;
      file.read(reinterpret_cast<char*>(&temp), sizeof(float));
      *attr = static_cast<double>(temp);
    }
  }
}

int compareFiles(const std::string& datafilename, const std::string& outfilename, double tolerance) {
  //leo archivo de trazas y guardo los datos en un vector de struct de ParticleInfo
  Fluid fluidData;
  std::ifstream file(datafilename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << datafilename << std::endl;
    return -1;
  }
  readFldFile(file, fluidData);


  //leo archivo de salida de fluid y lo guardo en struct Fluid
  Fluid fluidOut;
  std::ifstream outF(outfilename, std::ios::binary);
  if (!outF.is_open()) {
    std::cerr << "Error: Unable to open file " << outfilename << std::endl;
    return -1;
  }
  readFldFile(outF, fluidOut);


  //comparo resultados de leer cada archivo con un cierto margen de error
  for(int i = 0; i<fluidOut.numberparticles; i++){
    if ((fluidOut.particles[i].px - tolerance > fluidData.particles[i].px or fluidOut.particles[i].px + tolerance < fluidData.particles[i].px)
        or (fluidOut.particles[i].py - tolerance > fluidData.particles[i].py or fluidOut.particles[i].py + tolerance < fluidData.particles[i].py)
        or (fluidOut.particles[i].pz - tolerance > fluidData.particles[i].pz or fluidOut.particles[i].pz + tolerance < fluidData.particles[i].pz)
        or (fluidOut.particles[i].hvx - tolerance > fluidData.particles[i].hvx or fluidOut.particles[i].hvx + tolerance < fluidData.particles[i].hvx)
        or (fluidOut.particles[i].hvy - tolerance > fluidData.particles[i].hvy or fluidOut.particles[i].hvy + tolerance < fluidData.particles[i].hvy)
        or (fluidOut.particles[i].hvz - tolerance > fluidData.particles[i].hvz or fluidOut.particles[i].hvz + tolerance < fluidData.particles[i].hvz)
        or (fluidOut.particles[i].vx - tolerance > fluidData.particles[i].vx or fluidOut.particles[i].vx + tolerance < fluidData.particles[i].vx)
        or (fluidOut.particles[i].vy - tolerance > fluidData.particles[i].vy or fluidOut.particles[i].vy + tolerance < fluidData.particles[i].vy)
        or (fluidOut.particles[i].vz - tolerance > fluidData.particles[i].vz or fluidOut.particles[i].vz + tolerance < fluidData.particles[i].vz)
        or (fluidOut.particles[i].density - tolerance > fluidData.particles[i].density or fluidOut.particles[i].density + tolerance < fluidData.particles[i].density)
        or (fluidOut.particles[i].ax - tolerance > fluidData.particles[i].ax or fluidOut.particles[i].ax + tolerance < fluidData.particles[i].ax)
        or (fluidOut.particles[i].ay - tolerance > fluidData.particles[i].ay or fluidOut.particles[i].ay + tolerance < fluidData.particles[i].ay)
        or (fluidOut.particles[i].az - tolerance > fluidData.particles[i].az or fluidOut.particles[i].az + tolerance < fluidData.particles[i].az)){
        std::cout <<  "La partícula de fluidOut " << fluidOut.particles[i].id << " Densidad: " << fluidOut.particles[i].density
                   << " x: " << fluidOut.particles[i].px << " y: " << fluidOut.particles[i].py
                  << " z: " << fluidOut.particles[i].pz << " Velocidad: (" << fluidOut.particles[i].vx << ", " << fluidOut.particles[i].vy << ", "
                  << fluidOut.particles[i].vz << ")"
                  << "     Aceleración: (" << fluidOut.particles[i].ax << ", " << fluidOut.particles[i].ay << ", "
                  << fluidOut.particles[i].az << ")" << '\n';
        std::cout <<  "La partícula de fluidData " << fluidData.particles[i].id << " Densidad: " << fluidData.particles[i].density
                  << " x: " << fluidData.particles[i].px << " y: " << fluidData.particles[i].py
                  << " z: " << fluidData.particles[i].pz << " Velocidad: (" << fluidData.particles[i].vx << ", " << fluidData.particles[i].vy << ", "
                  << fluidData.particles[i].vz << ")"
                  << "     Aceleración: (" << fluidData.particles[i].ax << ", " << fluidData.particles[i].ay << ", "
                  << fluidData.particles[i].az << ")" << '\n';
      }
          return -1;
    }

  return 0;
}

//
// Created by Raul on 11/11/2023.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <span>

struct Particle {
    int64_t id;
    double px, py, pz;
    double hvx, hvy, hvz;
    double vx, vy, vz;
    double density;
    double ax, ay, az;
};

struct Fluid{
    int particlespermeter = 0;
    int numberparticles = 0;
    std::vector<Particle> particles;
};

//NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
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
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

int compareFiles(const std::string& datafilename, const std::string& outfilename, double tolerance) {
  //leo archivo de trazas y guardo los datos en un vector de struct de ParticleInfo, y luego hago lo mismo con el archivo de salida de fluid
  Fluid fluidData;
  std::ifstream file(datafilename, std::ios::binary);
  if (!file.is_open()) {
    return -1;
  }
  readFldFile(file, fluidData);
  Fluid fluidOut;
  std::ifstream outF(outfilename, std::ios::binary);
  if (!outF.is_open()) {
    return -1;
  }
  readFldFile(outF, fluidOut);
  //comparo resultados de leer cada archivo con un cierto margen de error
  for(int i = 0; i<fluidOut.numberparticles; i++){
    if ((fluidOut.particles[i].px - tolerance > fluidData.particles[i].px or fluidOut.particles[i].px + tolerance < fluidData.particles[i].px) or (fluidOut.particles[i].py - tolerance > fluidData.particles[i].py or fluidOut.particles[i].py + tolerance < fluidData.particles[i].py) or (fluidOut.particles[i].pz - tolerance > fluidData.particles[i].pz or fluidOut.particles[i].pz + tolerance < fluidData.particles[i].pz)
        or (fluidOut.particles[i].hvx - tolerance > fluidData.particles[i].hvx or fluidOut.particles[i].hvx + tolerance < fluidData.particles[i].hvx) or (fluidOut.particles[i].hvy - tolerance > fluidData.particles[i].hvy or fluidOut.particles[i].hvy + tolerance < fluidData.particles[i].hvy) or (fluidOut.particles[i].hvz - tolerance > fluidData.particles[i].hvz or fluidOut.particles[i].hvz + tolerance < fluidData.particles[i].hvz)
        or (fluidOut.particles[i].vx - tolerance > fluidData.particles[i].vx or fluidOut.particles[i].vx + tolerance < fluidData.particles[i].vx) or (fluidOut.particles[i].vy - tolerance > fluidData.particles[i].vy or fluidOut.particles[i].vy + tolerance < fluidData.particles[i].vy) or (fluidOut.particles[i].vz - tolerance > fluidData.particles[i].vz or fluidOut.particles[i].vz + tolerance < fluidData.particles[i].vz)
        or (fluidOut.particles[i].density - tolerance > fluidData.particles[i].density or fluidOut.particles[i].density + tolerance < fluidData.particles[i].density)
        or (fluidOut.particles[i].ax - tolerance > fluidData.particles[i].ax or fluidOut.particles[i].ax + tolerance < fluidData.particles[i].ax) or (fluidOut.particles[i].ay - tolerance > fluidData.particles[i].ay or fluidOut.particles[i].ay + tolerance < fluidData.particles[i].ay) or (fluidOut.particles[i].az - tolerance > fluidData.particles[i].az or fluidOut.particles[i].az + tolerance < fluidData.particles[i].az)){
      return -1;
    }
  }
  return 0;
}

int main(int argc, char** argv){
  if (argc != 4){
      std::cerr<<"Número inválido de argumentos para filesComparator.cpp. \n";
      return -2;
  }

  std::span const args_view{argv, static_cast<std::size_t>(argc)};
  std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};
  const std::string& fileData = arguments[0];
  const std::string& fileOutput = arguments[1];
  const double tolerance = std::stod(arguments[2]);

  const int code = compareFiles(fileData, fileOutput, tolerance);
  return code;

}
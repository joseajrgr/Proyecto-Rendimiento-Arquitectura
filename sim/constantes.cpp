#include "constantes.hpp"

namespace Constantes {
    const double multRadio = 1.695;
    const double densFluido = 1000;
    const double presRigidez = 3.0;
    const double colisRigidez = 30000;
    const double amortiguamiento = 128.0;
    const double viscosidad = 0.4;
    const double tamParticula = 0.0002;
    const double pasoTiempo = 0.001;

    const Gravedad gravedad = {0.0, -9.8, 0.0};

    const Punto limInferior = {-0.065, -0.08, -0.065};
    const Punto limSuperior = {0.065, 0.1, 0.065};

    // Distintos numeros a los que accedemos al operar
    const double factor05 = 0.5;
    const double factor1e10 = 1e-10;
    const double seis = 6;
    const double cuarentaycinco = 45;
    const double quince = 15;

    // Para la transferencia de la aceleracion
    const double smallQ = 10e-12;
}

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

    // Definir constantes para los códigos de error
    const int ERROR_INVALID_PARTICLE_COUNT = -5;

    const double factor05 = 0.5;
    const double factor1e10 = 1e-10;

    // Para la transferencia de la aceleracion
    const double smallQ = 10e-12;
}

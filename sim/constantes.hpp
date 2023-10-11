#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP

#include "block.hpp"  // Incluimos block.hpp para tener acceso a la estructura "Punto"

namespace Constantes {
    extern const double multRadio;
    extern const double densFluido;
    extern const double presRigidez;
    extern const double colisRigidez;
    extern const double amortiguamiento;
    extern const double viscosidad;
    extern const double tamParticula;
    extern const double pasoTiempo;

    // Definición de la estructura del vector de gravedad
    struct Gravedad {
        double x;
        double y;
        double z;
    };
    // Declaración de la constante de gravedad como variable externa
    extern const Gravedad gravedad;

    extern const Punto limInferior;
    extern const Punto limSuperior;
}

#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP

#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP

#include <cstdint>
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

    // Definir constantes para los códigos de error
    enum ErrorCode : std::int8_t {
        NO_ERROR = 0,
        INVALID_ARGUMENTS = -1,
        INVALID_TIME_STEPS = -2,
        INVALID_NUMERIC_FORMAT = -1,
        CANNOT_OPEN_FILE_READING = -3,
        CANNOT_OPEN_FILE_WRITING = -4,
        INVALID_PARTICLE_COUNT = -5
    };

    // Distintos numeros a los que accedemos al operar
    extern const double factor05;
    extern const double factor1e10;
    extern const double seis;
    extern const double cuarentaycinco;
    extern const double quince;

    // Para la transferencia de la aceleracion
    struct ConstAccTransf {
        double h;
        double hSquared;
        double factor2;
        double commonFactor;
    };
    extern const double smallQ;
}

#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_CONSTANTES_HPP

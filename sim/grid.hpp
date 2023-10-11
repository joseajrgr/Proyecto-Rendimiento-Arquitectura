//
// Created by quinc on 03/10/2023.
//

#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

#include "block.hpp"  // Incluimos block.hpp para tener acceso a la estructura "Punto"

class Grid {
    public:
        double numberblocksx;
        double numberblocksy;
        double numberblocksz;
        double numBlocks;
        double meshx;
        double meshy;
        double meshz;

        Grid(const Punto& bmin, const Punto& bmax);
        void dividirEnBloques(double smoothingLength);

    private:
        Punto bmin; // Limite inferior del recinto
        Punto bmax; // Limite superior del recinto
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_GRID_HPP

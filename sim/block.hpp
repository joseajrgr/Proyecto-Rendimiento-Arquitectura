//
// Created by quinc on 03/10/2023.
//

#ifndef PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP
#define PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP


struct Punto {
    double x, y, z;
};

class Block {
    public:
        Block(const Punto& minPoint, const Punto& maxPoint);
        bool particula_en_bloque(const Punto& point) const;

    private:
        Punto minPoint;
        Punto maxPoint;
};


#endif //PROYECTO_RENDIMIENTO_ARQUITECTURA_BLOCK_HPP

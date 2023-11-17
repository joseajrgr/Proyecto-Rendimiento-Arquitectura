#include <gtest/gtest.h>
#include "sim/grid.cpp"

//Test para comprobar un funcionamiento correcto del metodo dividirEnBloques
TEST(GridTests, dividirEnBloques_correcto) {
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{3.0,3.0,3.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;

    grid.dividirEnBloques(smoothingLength);

    ASSERT_EQ(3.0, grid.getNumberblocksx());
    ASSERT_EQ(3.0, grid.getNumberblocksy());
    ASSERT_EQ(3.0, grid.getNumberblocksz());
    ASSERT_EQ(27.0, grid.getNumBlocks());
    ASSERT_EQ(1.0, grid.getMeshx());
    ASSERT_EQ(1.0, grid.getMeshy());
    ASSERT_EQ(1.0, grid.getMeshz());
    ASSERT_EQ(27.0, grid.getBlocks().size());
}

//test para comprobar que se lanza una excepción si el smoothinglength es 0
TEST(GridTests, dividirEnBloques_smoothinglength0) {
    const Punto bmin{0.0, 0.0, 0.0};
    const Punto bmax{10.0, 10.0, 10.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 0.0;

    ASSERT_THROW(grid.dividirEnBloques(smoothingLength), std::invalid_argument);
}

//test para comprobar que se lanza una excepción si el smoothinglength es negativo
TEST(GridTests, dividirEnBloques_smoothinglength_negative) {
    const Punto bmin{0.0, 0.0, 0.0};
    const Punto bmax{10.0, 10.0, 10.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = -10.0;

    ASSERT_THROW(grid.dividirEnBloques(smoothingLength), std::invalid_argument);
}

/*Test para comprobar un funcionamiento correcto tras invocar de forma sucesiva a smoothinglength
con smoothinglengths diferentes */
TEST(GridTests, dividirEnBloques_sucesivo) {
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{10.0,10.0,10.0};
    Grid grid(bmin, bmax);
    double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    ASSERT_EQ(10.0, grid.getNumberblocksx());
    ASSERT_EQ(10.0, grid.getNumberblocksy());
    ASSERT_EQ(10.0, grid.getNumberblocksz());
    ASSERT_EQ(1000.0, grid.getNumBlocks());
    ASSERT_EQ(1.0, grid.getMeshx());
    ASSERT_EQ(1.0, grid.getMeshy());
    ASSERT_EQ(1.0, grid.getMeshz());
    ASSERT_EQ(1000.0, grid.getBlocks().size());
    smoothingLength = 2.0;
    grid.dividirEnBloques(smoothingLength);
    ASSERT_EQ(5.0, grid.getNumberblocksx());
    ASSERT_EQ(5.0, grid.getNumberblocksy());
    ASSERT_EQ(5.0, grid.getNumberblocksz());
    ASSERT_EQ(125.0, grid.getNumBlocks());
    ASSERT_EQ(2.0, grid.getMeshx());
    ASSERT_EQ(2.0, grid.getMeshy());
    ASSERT_EQ(2.0, grid.getMeshz());
    ASSERT_EQ(125.0, grid.getBlocks().size());
}
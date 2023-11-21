#include <gtest/gtest.h>
#include "sim/grid.cpp"
//constantes para resolver errores clang-tidy
const double decimal5_value = 0.5;
const double double_2_value = 2.0;
const double double_1_decimal_5value = 1.5;
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
    ASSERT_EQ(27.0, grid.getNumberblocksx()*grid.getNumberblocksy()*grid.getNumberblocksz());
    ASSERT_EQ(1.0, grid.getMeshx());
    ASSERT_EQ(1.0, grid.getMeshy());
    ASSERT_EQ(1.0, grid.getMeshz());
    ASSERT_EQ(27.0, grid.getBlocks().size());
}

//Test para comprobar un funcionamiento correcto del metodo dividirEnBloques con puntos negativos
TEST(GridTests, dividirEnBloques_bneg) {
    const Punto bmin{-3.0,-3.0,-3.0};
    const Punto bmax{0.0,0.0,0.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;

    grid.dividirEnBloques(smoothingLength);

    ASSERT_EQ(3.0, grid.getNumberblocksx());
    ASSERT_EQ(3.0, grid.getNumberblocksy());
    ASSERT_EQ(3.0, grid.getNumberblocksz());
    ASSERT_EQ(27.0, grid.getNumberblocksx()*grid.getNumberblocksy()*grid.getNumberblocksz());
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
    ASSERT_EQ(1000.0, grid.getNumberblocksx()*grid.getNumberblocksy()*grid.getNumberblocksz());
    ASSERT_EQ(1.0, grid.getMeshx());
    ASSERT_EQ(1.0, grid.getMeshy());
    ASSERT_EQ(1.0, grid.getMeshz());
    ASSERT_EQ(1000.0, grid.getBlocks().size());
    smoothingLength = double_2_value;
    grid.dividirEnBloques(smoothingLength);
    ASSERT_EQ(5.0, grid.getNumberblocksx());
    ASSERT_EQ(5.0, grid.getNumberblocksy());
    ASSERT_EQ(5.0, grid.getNumberblocksz());
    ASSERT_EQ(125.0,grid.getNumberblocksx()*grid.getNumberblocksy()*grid.getNumberblocksz());
    ASSERT_EQ(2.0, grid.getMeshx());
    ASSERT_EQ(2.0, grid.getMeshy());
    ASSERT_EQ(2.0, grid.getMeshz());
    ASSERT_EQ(125.0, grid.getBlocks().size());
}

//Test para comprobar el correcto funcionamiento de reposicionarParticulasFluid()
TEST(GridTests, repos_fluid_correcto) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,1.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 1.5, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    Fluid fluid{1.0,2,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    //comprobamos que cada bloque tiene una partícula, y que esa partícula tiene el id correspondiente
    for(int i=0; i<static_cast<int>(blocks.size());i++){
        ASSERT_EQ(1,blocks[i].particles.size());
        ASSERT_EQ(i,blocks[i].particles[0].id);
    }
}

//test para comprobar reposicionarParticulasFluid() con una particula fuera de la malla
TEST(GridTests, repos_fluid_particula_ext) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{1.0,1.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 está fuera de la malla
    const Particle particle1={0, 0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    particulas.push_back(particle1);
    Fluid fluid{1.0,1,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    //comprobamos que la partícula aunque este fuera del grid al reposicionar se integra en la malla
    for(int i=0; i<static_cast<int>(blocks.size());i++){
        ASSERT_EQ(1,blocks[i].particles.size());
        ASSERT_EQ(i,blocks[i].particles[0].id);
    }
}

//test para comprobar reposicionarParticulasFluid() con una particula con px negativa
TEST(GridTests, repos_fluid_particula_neg) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{-2.0,-1.0,-1.0};
    const Punto bmax{0.0,0.0,0.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 está fuera de la malla
    const Particle particle1={0, 0, -0.5, -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    particulas.push_back(particle1);
    Fluid fluid{1.0,1,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    //comprobamos que la partícula está en su bloque correspondiente
    ASSERT_EQ(0,blocks[0].particles.size());
    ASSERT_EQ(1,blocks[1].particles.size());
    ASSERT_EQ(0,blocks[1].particles[0].id);
}

/*test para comprobar reposicionarParticulaBloque de forma sucesiva alteranod el px de las partículas
entre las llamadas de reposicionarParticulasBloque*/
TEST(GridTests, repos_block_correcto) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,1.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    Fluid fluid{1.0,2,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    for(auto &block :blocks){
        ASSERT_EQ(1,block.particles.size());
    }
    ASSERT_EQ(1,blocks[1].particles[0].id);
    ASSERT_EQ(0,blocks[0].particles[0].id);
    //con estos valores la particula con id 0 deberá estar en el bloque 1 y la particula con id 1 en el bloque con id 0
    blocks[0].particles[0].px=double_2_value;
    blocks[1].particles[0].px=0.0;
    grid.reposicionarParticulasBloque(blocks);
    //comprobamos que cada bloque tiene una partícula, y que esa partícula tiene el id correspondiente
    for(auto &block :blocks){
        ASSERT_EQ(1,block.particles.size());
    }
    ASSERT_EQ(1,blocks[0].particles[0].id);
    ASSERT_EQ(0,blocks[1].particles[0].id);
}

//test para comprobar reposicionarParticulas() con una particula fuera de la malla
TEST(GridTests, repos_block_ext) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{1.0,1.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 está fuera de la malla
    const Particle particle1={0, 0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    particulas.push_back(particle1);
    Fluid fluid{1.0,1,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    //cambiamos las cordenadas de las particulas para que esté fuera del grid
    blocks[0].particles[0].px=blocks[0].particles[0].py=blocks[0].particles[0].pz=double_2_value;
    grid.reposicionarParticulasBloque(blocks);
    //comprobamos que la partícula aunque este fuera del grid al reposicionar se integra en la malla
    ASSERT_EQ(1,blocks[0].particles.size());
    ASSERT_EQ(0,blocks[0].particles[0].id);
}

//test para comprobar reposicionarParticulasBloque() con una particula con px negativa
TEST(GridTests, repos_block_neg) {
    //creamos una malla que tenga 2 bloques
    const Punto bmin{-2.0,-1.0,-1.0};
    const Punto bmax{0.0,0.0,0.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 está fuera de la malla
    const Particle particle1={0, 0, -decimal5_value, -decimal5_value, -decimal5_value, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    particulas.push_back(particle1);
    Fluid fluid{1.0,1,particulas};
    std::vector<Block> blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,blocks);
    ASSERT_EQ(0,blocks[0].particles.size());
    ASSERT_EQ(1,blocks[1].particles.size());
    ASSERT_EQ(0,blocks[1].particles[0].id);
    blocks[1].particles[0].px=blocks[1].particles[0].py=blocks[1].particles[0].pz=-double_1_decimal_5value;
    grid.reposicionarParticulasBloque(blocks);
    //comprobamos que la partícula se cambia de bloque al cambiar los valores del vector p
    ASSERT_EQ(1,blocks[0].particles.size());
    ASSERT_EQ(0,blocks[1].particles.size());
    ASSERT_EQ(0,blocks[0].particles[0].id);
}

TEST(GridTests, simular_malla){
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{1.0,1.0,1.0};
    Grid grid(bmin, bmax);
    std::vector<Particle> particulas;
    const Particle particle1={0, 0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    particulas.push_back(particle1);
    const Fluid fluid{1.0,1,particulas};
    auto result=grid.simular_malla(fluid);
    ASSERT_EQ(1.695,result.first);
    ASSERT_EQ(1000,result.second);

}
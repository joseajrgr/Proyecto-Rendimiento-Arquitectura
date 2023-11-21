#include <gtest/gtest.h>
#include "sim/simulacion.hpp"
//constantes para evitar avisos clang-tidy por magic number
const double double_4_value = 4.0;
const double double_2_value = 2.0;
const double double_3_value = 3.0;
const double double_10_value = 10.0;
const double decimal01_value = 0.01;
const double decimal1_value = 0.1;
//test para comprobar que se inicializan las aceleraciones de forma correcta
TEST(SimulationTests, InitAccelerations)
{
    Block block(0,0,0,0);
    const Particle particle{1, 1, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.1, 0.3, 2.0, 3.0, 3.0, 3.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    initAccelerations(blocks_vector);
    ASSERT_EQ(blocks_vector[0].particles[0].ax,0.0);
    ASSERT_EQ(blocks_vector[0].particles[0].ay,-9.8);
    ASSERT_EQ(blocks_vector[0].particles[0].az,0.0);
    ASSERT_EQ(blocks_vector[0].particles[0].density,0.0);
}

/*Test para comprobar el funcionamiento de particles movement en un bloque con una partícula
  Para comprobar los resultados comprobamos con una tolerancia no muy restrictiva, de 1e-5
 */
TEST(SimulationTests, ParticlesMovement) {
    Block block(0,0,0,0);
    const Particle particle{1, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particlesMovement(blocks_vector);
    const double tolerance = 1e-5;
    ASSERT_NEAR(0.003003,blocks_vector[0].particles[0].px,tolerance);
    ASSERT_NEAR(1.003,blocks_vector[0].particles[0].py,tolerance);
    ASSERT_NEAR(2.002,blocks_vector[0].particles[0].pz,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[0].particles[0].vx,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[0].particles[0].vy,tolerance);
    ASSERT_NEAR(2.0015,blocks_vector[0].particles[0].vz,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[0].particles[0].hvx,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[0].particles[0].hvy,tolerance);
    ASSERT_NEAR(2.003,blocks_vector[0].particles[0].hvz,tolerance);
}

/*test para comprobar particlesmovement con un vector de bloques de tamaño mayor a 1
cada uno con su propia partícula*/
TEST(SimulationTests, ParticlesMovement2) {
    Block block(0,0,0,0);
    Block block2(1,0,0,0);
    const Particle particle{0, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, double_4_value};
    const Particle particle2{1, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    block2.addParticle(particle2);
    blocks_vector.push_back(block);
    blocks_vector.push_back(block2);
    particlesMovement(blocks_vector);
    const double tolerance = 1e-5;
    //comprobación para la partícula del primer bloque
    ASSERT_EQ(0,blocks_vector[0].particles[0].id);
    ASSERT_NEAR(0.003003,blocks_vector[0].particles[0].px,tolerance);
    ASSERT_NEAR(1.003,blocks_vector[0].particles[0].py,tolerance);
    ASSERT_NEAR(2.002,blocks_vector[0].particles[0].pz,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[0].particles[0].vx,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[0].particles[0].vy,tolerance);
    ASSERT_NEAR(2.0015,blocks_vector[0].particles[0].vz,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[0].particles[0].hvx,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[0].particles[0].hvy,tolerance);
    ASSERT_NEAR(2.003,blocks_vector[0].particles[0].hvz,tolerance);
    //comprobación para la partícula del segundo bloque
    ASSERT_EQ(1,blocks_vector[1].particles[0].id);
    ASSERT_NEAR(0.003003,blocks_vector[1].particles[0].px,tolerance);
    ASSERT_NEAR(1.003,blocks_vector[1].particles[0].py,tolerance);
    ASSERT_NEAR(2.002,blocks_vector[1].particles[0].pz,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[1].particles[0].vx,tolerance);
    ASSERT_NEAR(3.0015,blocks_vector[1].particles[0].vy,tolerance);
    ASSERT_NEAR(2.0015,blocks_vector[1].particles[0].vz,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[1].particles[0].hvx,tolerance);
    ASSERT_NEAR(3.003,blocks_vector[1].particles[0].hvy,tolerance);
    ASSERT_NEAR(2.003,blocks_vector[1].particles[0].hvz,tolerance);
}

//Test para combrobar un bloque con cx=0, pero con deltax < 1e-10
TEST(SimulationTests, ParticlesCollisions) {
    Block block(0,0,2,2);
    const Particle particle{0, 1, 1.0, 1.0, 1.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(1.0,blocks_vector[0].particles[0].ax);
}

//Test para combrobar un bloque con cx=0, con deltax > 1e-10
TEST(SimulationTests, ParticlesCollisionsx) {
    Block block(0,0,2,2);
    const Particle particle{0, 1, -0.0652, 1.0, 1.0, -0.1, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(1,blocks_vector[0].particles[0].az);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ay);
    ASSERT_NEAR(-112.0,blocks_vector[0].particles[0].ax,1e-5);
}

//Test para combrobar un bloque con cy=0, con deltay > 1e-10
TEST(SimulationTests, ParticlesCollisionsy) {
    Block block(0,2,0,2);
    const Particle particle{0, 1, 1.0, -0.08, 1.0, 1.0, -0.2, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(1,blocks_vector[0].particles[0].az);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ax);
    ASSERT_NEAR(-242.99999,blocks_vector[0].particles[0].ay,1e-5);
}

//Test para combrobar un bloque con cz=0, con deltaz > 1e-10
TEST(SimulationTests, ParticlesCollisionsz) {
    Block block(0,2,2,0);
    const Particle particle{0, 1, 1.0, 1.0, -0.0652, 1.0, 1.0, -0.1, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ay);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ax);
    ASSERT_NEAR(-240,blocks_vector[0].particles[0].az,1e-5);
}

//Test para combrobar un bloque con cx=0,cy=0 y cz=0 con deltax,deltay,deltaz > 1e-10
TEST(SimulationTests, ParticlesCollisionsxyzliminf) {
    Block block(0,0,0,0);
    const Particle particle{0, 1, -0.0652, -0.08, -0.0652, -0.1, -0.2, -0.1, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_NEAR(-112.0,blocks_vector[0].particles[0].ax,1e-5);
    ASSERT_NEAR(-242.99999,blocks_vector[0].particles[0].ay,1e-5);
    ASSERT_NEAR(-240,blocks_vector[0].particles[0].az,1e-5);
}

//test para comprobar un bloque con cx,cy,cz=numblocs-1 y deltaz,deltax,deltay >1e-10
TEST(SimulationTests, ParticlesCollisionsxyzlimsup) {
    Block block(0,3,3,3);
    const Particle particle{0, 1, 0.065, 0.12, 0.065, -0.1, -0.2, -0.1, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_NEAR(-130,blocks_vector[0].particles[0].ax,1e-5);
    ASSERT_NEAR(-855,blocks_vector[0].particles[0].ay,1e-5);
    ASSERT_NEAR(-258,blocks_vector[0].particles[0].az,1e-5);
}

//test para comprobar que particlecollisions funciona pasando un vector de bloques de tamaño mayor que uno
TEST(SimulationTests, ParticlesCollisionsxyzDifBlocks) {
    Block block1(0,0,2,2);
    Block block2(0,2,0,2);
    Block block3(0,2,2,0);
    const Particle particle1{0, 1, -0.0652, 1.0, 1.0, -0.1, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    const Particle particle2{0, 1, 1.0, -0.08, 1.0, 1.0, -0.2, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    const Particle particle3{0, 1, 1.0, 1.0, -0.0652, 1.0, 1.0, -0.1, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block1.addParticle(particle1);
    block2.addParticle(particle2);
    block3.addParticle(particle3);
    blocks_vector.push_back(block1);
    blocks_vector.push_back(block2);
    blocks_vector.push_back(block3);
    particleColissions(blocks_vector,double_4_value,double_4_value,double_4_value);
    //comprobamos la particula del primer bloque
    ASSERT_EQ(1,blocks_vector[0].particles[0].ay);
    ASSERT_EQ(1,blocks_vector[0].particles[0].az);
    ASSERT_NEAR(-112.0,blocks_vector[0].particles[0].ax,1e-5);
    //comprobamos la particula del segundo bloque
    ASSERT_EQ(1,blocks_vector[1].particles[0].az);
    ASSERT_EQ(1,blocks_vector[1].particles[0].ax);
    ASSERT_NEAR(-242.99999,blocks_vector[1].particles[0].ay,1e-5);
    //comprobamos la particula del tercer bloque
    ASSERT_EQ(1,blocks_vector[2].particles[0].ay);
    ASSERT_EQ(1,blocks_vector[2].particles[0].ax);
    ASSERT_NEAR(-240,blocks_vector[2].particles[0].az,1e-5);
}

//Test para combrobar un bloque con cx=0, y una particula con posicion inferior al liminferior.x
TEST(SimulationTests, LimitInteractionsx) {
    Block block(0,0,2,2);
    const Particle particle{0, 1, -0.066, 1.0, 1.0, -0.1, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(-0.064,blocks_vector[0].particles[0].px);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvx);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vx,1e-5);
}

//Test para combrobar un bloque con cy=0, y una particula con posicion inferior al liminferior.y
TEST(SimulationTests, LimitInteractionsy) {
    Block block(0,2,0,2);
    const Particle particle{0, 1, 1.0, -0.09, 1.0, 1.0, -0.1, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(-0.07,blocks_vector[0].particles[0].py);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvy);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vy,1e-5);
}

//Test para combrobar un bloque con cz=0, y una particula con posicion inferior al liminferior.z
TEST(SimulationTests, LimitInteractionsz) {
    Block block(0,2,2,0);
    const Particle particle{0, 1, 1.0, 1.0, -0.066, 1.0, 3.0, -0.1, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(-0.064,blocks_vector[0].particles[0].pz);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvz);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vz,1e-5);
}

/*Test para comprobar que limitinteractions funciona correctamente en todas las cordenadas simultaneamente, en un bloque
 con ci=0 */
TEST(SimulationTests, LimitInteractionsxyzliminf) {
    Block block(0,0,0,0);
    const Particle particle{0, 1, -0.066, -0.09, -0.066, -0.1, -0.1, -0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(-0.064,blocks_vector[0].particles[0].px);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvx);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vx,1e-5);
    ASSERT_EQ(-0.07,blocks_vector[0].particles[0].py);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvy);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vy,1e-5);
    ASSERT_EQ(-0.064,blocks_vector[0].particles[0].pz);
    ASSERT_EQ(+0.1,blocks_vector[0].particles[0].hvz);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vz,1e-5);
}

/*test para comprobar que limitinteractions funciona correctamente en todas las cordenadas simultaneamente, en un bloque
con ci = numblocksi-1*/
TEST(SimulationTests, LimitInteractionsxyzlimsup) {
    Block block(0,3,3,3);
    const Particle particle{0, 1, +0.066, +0.11, +0.066, +0.1, +0.1, +0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(+0.064,blocks_vector[0].particles[0].px);
    ASSERT_EQ(-0.1,blocks_vector[0].particles[0].hvx);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vx,1e-5);
    ASSERT_NEAR(0.09,blocks_vector[0].particles[0].py,1e-5);
    ASSERT_EQ(-0.1,blocks_vector[0].particles[0].hvy);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vy,1e-5);
    ASSERT_EQ(+0.064,blocks_vector[0].particles[0].pz);
    ASSERT_EQ(-0.1,blocks_vector[0].particles[0].hvz);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vz,1e-5);
}

//test para comprobar que limitinteractions funciona pasando un vector de bloques de tamaño mayor que uno
TEST(SimulationTests, LimitInteractionsxyzdifbloklimsup) {
    Block block1(0,3,0,0);
    Block block2(0,0,3,0);
    Block block3(0,0,0,3);
    const Particle particle1{0, 1, +0.066, +1.0, +1.0, +0.1, +0.2, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    const Particle particle2{0, 1, 1.0, +0.11, +1.0, +0.2, +0.1, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    const Particle particle3{0, 1, 1.0, 1.0, +0.066, +0.2, +0.2, +0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, double_4_value};
    std::vector<Block> blocks_vector;
    block1.addParticle(particle1);
    block2.addParticle(particle2);
    block3.addParticle(particle3);
    blocks_vector.push_back(block1);
    blocks_vector.push_back(block2);
    blocks_vector.push_back(block3);
    limitInteractions(blocks_vector,double_4_value,double_4_value,double_4_value);
    ASSERT_EQ(+0.064,blocks_vector[0].particles[0].px);
    ASSERT_EQ(-0.1,blocks_vector[0].particles[0].hvx);
    ASSERT_NEAR(-1.0,blocks_vector[0].particles[0].vx,1e-5);
    ASSERT_NEAR(0.09,blocks_vector[1].particles[0].py,1e-5);
    ASSERT_EQ(-0.1,blocks_vector[1].particles[0].hvy);
    ASSERT_NEAR(-1.0,blocks_vector[1].particles[0].vy,1e-5);
    ASSERT_EQ(+0.064,blocks_vector[2].particles[0].pz);
    ASSERT_EQ(-0.1,blocks_vector[2].particles[0].hvz);
    ASSERT_NEAR(-1.0,blocks_vector[2].particles[0].vz,1e-5);
}

//por razones de simplicidad para probar transformdesnsities utilizamos un h y un factordenstransf sencillos
TEST(SimulationTests, TransformDensities) {
    Block block1(0,3,0,0);
    const Particle particle1{0, 1, +0.066, +1.0, +1.0, +0.1, +0.2, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    std::vector<Block> blocks_vector;
    block1.addParticle(particle1);
    blocks_vector.push_back(block1);
    transformDensities(blocks_vector,double_2_value,decimal1_value);
    ASSERT_NEAR(blocks_vector[0].particles[0].density,6.5,1e-5);
}

//test para comprobar el funcionamiento de transformdensities con varios bloques
TEST(SimulationTests, TransformDensities2) {
    Block block1(0,3,0,0);
    Block block2(1,3,1,0);
    Block block3(2,3,0,1);
    const Particle particle1{0, 1, +0.066, +1.0, +1.0, +0.1, +0.2, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    const Particle particle2{0, 1, +0.066, +1.0, +1.0, +0.1, +0.2, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0};
    const Particle particle3{0, 1, +0.066, +1.0, +1.0, +0.1, +0.2, +0.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 3.0};
    std::vector<Block> blocks_vector;
    block1.addParticle(particle1);
    block2.addParticle(particle2);
    block3.addParticle(particle3);
    blocks_vector.push_back(block1);
    blocks_vector.push_back(block2);
    blocks_vector.push_back(block3);
    transformDensities(blocks_vector,double_2_value,decimal1_value);
    ASSERT_NEAR(blocks_vector[0].particles[0].density,6.5,1e-5);
    ASSERT_NEAR(blocks_vector[1].particles[0].density,6.6,1e-5);
    ASSERT_NEAR(blocks_vector[2].particles[0].density,6.7,1e-5);
}

//test para comprobar que no se incrementa las densidades, imponiendo un h pequeño
TEST(SimulationTests, incrementDensities){
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,2.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    //La partícula 3 deberá ir al segundo bloque
    const Particle particle3{2, 0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    particulas.push_back(particle3);
    Fluid fluid{1.0,3,particulas};
    std::vector<Block> grid_blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,grid_blocks);
    incrementDensities(grid_blocks,decimal1_value,grid);
    ASSERT_EQ(0.0,grid_blocks[0].particles[0].density);
    ASSERT_EQ(2.0,grid_blocks[1].particles[0].density);
    ASSERT_EQ(1.0,grid_blocks[2].particles[0].density);
}

//test para comprobar que se actualizan las densidades al invocar a incrementDensities, imponiendo un h grande
TEST(SimulationTests, incrementDensities2){
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,2.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    //La partícula 3 deberá ir al segundo bloque
    const Particle particle3{2, 0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    particulas.push_back(particle3);
    Fluid fluid{1.0,3,particulas};
    std::vector<Block> grid_blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,grid_blocks);
    incrementDensities(grid_blocks,double_3_value*double_3_value,grid);
    ASSERT_EQ(189.0,grid_blocks[0].particles[0].density);
    ASSERT_EQ(66.0,grid_blocks[1].particles[0].density);
    ASSERT_EQ(126.0,grid_blocks[2].particles[0].density);
}

/*test para comprobar que si distSquared >= constAccTransf.hSquared
  no se actualizan las aceleraciones al invocar a transferAcceleration*/
TEST(SimulationTests, transferAcceleration){
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,2.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    //La partícula 3 deberá ir al segundo bloque
    const Particle particle3{2, 0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    particulas.push_back(particle3);
    Fluid fluid{1.0,3,particulas};
    std::vector<Block> grid_blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,grid_blocks);
    Constantes::ConstAccTransf constAccTransf={decimal01_value,decimal01_value,double_4_value,double_2_value};
    transferAcceleration(grid_blocks,constAccTransf,grid);
    ASSERT_EQ(0.0,grid_blocks[0].particles[0].ax);
    ASSERT_EQ(0.0,grid_blocks[0].particles[0].ay);
    ASSERT_EQ(0.0,grid_blocks[0].particles[0].az);
    ASSERT_EQ(2.0,grid_blocks[1].particles[0].ax);
    ASSERT_EQ(2.0,grid_blocks[1].particles[0].ay);
    ASSERT_EQ(2.0,grid_blocks[1].particles[0].az);
    ASSERT_EQ(1.0,grid_blocks[2].particles[0].ax);
    ASSERT_EQ(1.0,grid_blocks[2].particles[0].ay);
    ASSERT_EQ(1.0,grid_blocks[2].particles[0].az);
}

//test para comprobar que se actulizan las aceleraciones al llamar a transefercceleration
TEST(SimulationTests, transferAcceleration2){
    const Punto bmin{0.0,0.0,0.0};
    const Punto bmax{2.0,2.0,1.0};
    Grid grid(bmin, bmax);
    const double smoothingLength = 1.0;
    grid.dividirEnBloques(smoothingLength);
    //definimos 2 particulas y un fluid
    std::vector<Particle> particulas;
    //La partícula 1 deberá ir al primer bloque
    const Particle particle1={0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0};
    //La partícula 2 deberá ir al segundo bloque
    const Particle particle2{1, 0, 2.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 10.0};
    //La partícula 3 deberá ir al segundo bloque
    const Particle particle3{2, 0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 10.0};
    particulas.push_back(particle1);
    particulas.push_back(particle2);
    particulas.push_back(particle3);
    Fluid fluid{1.0,3,particulas};
    std::vector<Block> grid_blocks = grid.getBlocks();
    grid.reposicionarParticulasFluid(fluid,grid_blocks);
    Constantes::ConstAccTransf constAccTransf={decimal01_value,double_10_value,double_4_value,double_2_value};
    transferAcceleration(grid_blocks,constAccTransf,grid);
    ASSERT_NEAR(156.899959,grid_blocks[0].particles[0].ax,1e-5);
    ASSERT_NEAR(175.596125,grid_blocks[0].particles[0].ay,1e-6);
    ASSERT_NEAR(87.838062,grid_blocks[0].particles[0].az,1e-6);
    ASSERT_NEAR(237.97864,grid_blocks[1].particles[0].ax,1e-5);
    ASSERT_NEAR(-409.574765,grid_blocks[1].particles[0].ay,1e-5);
    ASSERT_NEAR(-203.8073828,grid_blocks[1].particles[0].az,1e-5);
    ASSERT_NEAR(-391.8786,grid_blocks[2].particles[0].ax,1e-5);
    ASSERT_NEAR(236.97864,grid_blocks[2].particles[0].ay,1e-5);
    ASSERT_NEAR(118.96932,grid_blocks[2].particles[0].az,1e-5);
}

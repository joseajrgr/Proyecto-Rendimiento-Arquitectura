#include <gtest/gtest.h>
#include "sim/simulacion.hpp"

//test para comprobar que se inicializan las aceleraciones de forma correcta
TEST(SimulationTests, InitAccelerations) {
    Block block(0,0,0,0);
    const Particle particle{1, 1, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.1, 0.3, 2.0, 3.0, 3.0, 3.0, 4.0};
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
    const Particle particle{1, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particlesMovement(blocks_vector);
    double tolerance = 1e-5;
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
TEST(SimulationTests, ParticlesMovement2) {
    Block block(0,0,0,0);
    Block block2(1,0,0,0);
    const Particle particle{0, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0};
    const Particle particle2{1, 1, 0.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    block2.addParticle(particle2);
    blocks_vector.push_back(block);
    blocks_vector.push_back(block2);
    particlesMovement(blocks_vector);
    double tolerance = 1e-5;
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
TEST(SimulationTests, ParticlesCollisions) {
    Block block(0,2,2,0);
    const Particle particle{0, 1, 1.0, 1.0, 1.0, 3.0, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,4.0,4.0,4.0);
    ASSERT_EQ(1.0,blocks_vector[0].particles[0].ax);
}

TEST(SimulationTests, ParticlesCollisionsx) {
    Block block(0,2,2,0);
    const Particle particle{0, 1, -0.0652, 1.0, 1.0, -0.1, 3.0, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,4.0,4.0,4.0);
    ASSERT_EQ(1,blocks_vector[0].particles[0].az);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ay);
    ASSERT_NEAR(-112.0,blocks_vector[0].particles[0].ax,1e-5);
}

TEST(SimulationTests, ParticlesCollisionsy) {
    Block block(0,2,0,2);
    const Particle particle{0, 1, 1.0, -0.08, 1.0, 1.0, -0.2, 2.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,4.0,4.0,4.0);
    ASSERT_EQ(1,blocks_vector[0].particles[0].az);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ax);
    ASSERT_NEAR(-242.99999,blocks_vector[0].particles[0].ay,1e-5);
}
TEST(SimulationTests, ParticlesCollisionsz) {
    Block block(0,0,2,2);
    const Particle particle{0, 1, 1.0, 1.0, -0.0652, 1.0, 1.0, -0.1, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 4.0};
    std::vector<Block> blocks_vector;
    block.addParticle(particle);
    blocks_vector.push_back(block);
    particleColissions(blocks_vector,4.0,4.0,4.0);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ay);
    ASSERT_EQ(1,blocks_vector[0].particles[0].ax);
    ASSERT_NEAR(-240,blocks_vector[0].particles[0].az,1e-5);
}
#include <gtest/gtest.h>
#include "sim/block.cpp"
#include "sim/block.hpp"

//Test para comprobr que se añade bien una particula
TEST(BlockTests, AddSingleParticle) {
    Block block(0,0,0,0);
    const Particle particle{1, 1, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.1, 0.3, 2.0, 3.0, 3.0, 3.0, 4.0};

    block.addParticle(particle);
    ASSERT_EQ(1, block.particles.size());
    ASSERT_EQ(particle, block.particles.at(0));
}

//Test para comprobar que se añaden multiples partículas y se guardan en el orden correcto
TEST(BlockTests, AddMultipleParticles) {
    Block block(0,0,0,0);
    const Particle particle1{1, 1, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0};
    const Particle particle2{2, 1, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 5.0};
    const Particle particle3{2, 1, 1.0, 1.0, 1.0, 2.0, 3.3, 3.3, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 5.0};

    block.addParticle(particle1);
    block.addParticle(particle2);
    block.addParticle(particle3);

    ASSERT_EQ(3, block.particles.size());
    ASSERT_EQ(particle1, block.particles.at(0));
    ASSERT_EQ(particle2, block.particles.at(1));
    ASSERT_EQ(particle3, block.particles.at(2));
}

/*test para probar que no deja añadir una particula incorrecta, pero el clang-tidy no deja compilar para probar el test
TEST(BlockTest, AddInvalidParticle) {
    Block block(0, 0, 0, 0);
    Particle particle(1,1,1,1);

    EXPECT_THROW(block.addParticle(particle), std::exception);
}
*/
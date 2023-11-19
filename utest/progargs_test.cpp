#include <gtest/gtest.h>
#include "sim/progargs.hpp"
#include "sim/constantes.hpp"

//test para comprobar que al pasar 3 argumentos, salta un error
TEST(Propargs_Tests, NumArguments3) {
    // Arrange
    const std::vector<std::string> arguments = {"small.fld", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode result = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);

    // Assert
    ASSERT_EQ(result, -1);
}

//test para comprobar que al pasar 5 argumentos, salta un error
TEST(Propargs_Tests, NumArguments5) {
    // Arrange
    const std::vector<std::string> arguments = {"small.fld", "out.fld", "aaaa", "33"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode result = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);
    // Assert
    ASSERT_EQ(result, -1);
}

//test para comprobar que al pasar como argumento un valor incorrecto de nº de iteraciones, obtenemos un error
TEST(Propargs_Tests, NumIterationsInvalidValue) {
    // Arrange
    const std::vector<std::string> arguments = {"-2", "small.fld", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode result = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);

    // Assert
    ASSERT_EQ(result, -2);
}

//test para comprobar que al pasar el nº de iteraciones como argumento y que no sea un número este, se obtiene un error
TEST(Propargs_Tests, NumIterationsInvalidFormat) {
    // Arrange
    const std::vector<std::string> arguments = {"treintaytres", "small.fld", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode result = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);

    // Assert
    ASSERT_EQ(result, -1);
}

//test para comprobar que al pasar en el inputfile un archivo que no se puede leer o no existe, se obtiene un error
TEST(Propargs_Tests, OpenFileWrongFormat) {
    // Arrange
    const std::vector<std::string> arguments = {"10", "mesifrutero", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode result = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);
    // Assert
    ASSERT_EQ(result, -3);
}

//test para comprobar que si el argumento outputfile no se puede abrir, da un error
/*TEST(Propargs_Tests, OutFileWrongFormat) {
    // Arrange
    const std::vector<std::string> arguments = {"10", "small.fld", "mesifrutero"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode resultin = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);
    Constantes::ErrorCode resultout = comprobarArgsSalida(arguments, argumentos);
    // Assert
    ASSERT_EQ(resultin, 0);
    ASSERT_EQ(resultout, -4);
}*/

//test con argumentos correctos, y el inputfile es small.fld
TEST(Propargs_Tests, AllCorrectSmall) {
    // Arrange
    const std::vector<std::string> arguments = {"10", "small.fld", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode resultin = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);
    Constantes::ErrorCode resultout = comprobarArgsSalida(arguments, argumentos);
    // Assert
    ASSERT_EQ(resultin, 0);
    ASSERT_EQ(resultout, 0);
}

//test para comprobar argumentos correctos, siendo inputfile large.fld
TEST(Propargs_Tests, AllCorrectLarge) {
    // Arrange
    const std::vector<std::string> arguments = {"10", "large.fld", "out.fld"};
    Argumentos argumentos;
    const size_t argc = arguments.size() + 1;
    // Act
    Constantes::ErrorCode resultin = comprobarArgsEntrada(static_cast<int>(argc), arguments, argumentos);
    Constantes::ErrorCode resultout = comprobarArgsSalida(arguments, argumentos);
    // Assert
    ASSERT_EQ(resultin, 0);
    ASSERT_EQ(resultout, 0);
}
#include "./filesComparator.cpp"
#include <gtest/gtest.h>

TEST(Fluid_Tests, NumberParticlesZero){
  // Arrange
    const char* script = "./ftest/scripts/test1-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 64256);
  ASSERT_EQ(codError, 251);
}

TEST(Fluid_Tests, BadParticlesNumberLess){
  // Arrange
    const char* script = "./ftest/scripts/test2-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 64256);
  ASSERT_EQ(codError, 251);
}

TEST(Fluid_Tests, BadParticlesNumberMore){
  // Arrange
    const char* script = "./ftest/scripts/test3-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 64256);
  ASSERT_EQ(codError, 251);
}

TEST(Fluid_Tests, IterationsValueLessZero){
  // Arrange
    const char* script = "./ftest/scripts/test4-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 65024);
  ASSERT_EQ(codError, 254);
}

TEST(Fluid_Tests, WrongIterationsValue){
  // Arrange
    const char* script = "./ftest/scripts/test5-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 65280);
  ASSERT_EQ(codError, 255);
}

TEST(Fluid_Tests, InputfileWrong){
  // Arrange
    const char* script = "./ftest/scripts/test6-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 64768);
  ASSERT_EQ(codError, 253);
}

TEST(Fluid_Tests, OutputFileWrong){
  // Arrange
    const char* script = "./ftest/scripts/test7-error.sh";
  // Act
  int result = std::system(script);
  int codError = WEXITSTATUS(result);
  // Assert
  ASSERT_EQ(result, 64512);
  ASSERT_EQ(codError, 252);
}

TEST(Fluid_Tests, ComparationSmall1) {
    // Arrange
    const char* script = "./ftest/scripts/test1S.sh";
    const std::string outfileName = "./ftest/reference_files/small-1.fld";
    const std::string fileData = "./ftest/outfiles_fld/outS.fld";
    const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationSmall2) {
    // Arrange
    const char* script = "./ftest/scripts/test2S.sh";
  const std::string outfileName = "./ftest/reference_files/small-2.fld";
  const std::string fileData = "./ftest/outfiles_fld/outS.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationSmall3) {
    // Arrange
    const char* script = "./ftest/scripts/test3S.sh";
  const std::string outfileName = "./ftest/reference_files/small-3.fld";
  const std::string fileData = "./ftest/outfiles_fld/outS.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationSmall4) {
    // Arrange
    const char* script = "./ftest/scripts/test4S.sh";
  const std::string outfileName = "./ftest/reference_files/small-4.fld";
  const std::string fileData = "./ftest/outfiles_fld/outS.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationSmall5) {
    // Arrange
    const char* script = "./ftest/scripts/test5S.sh";
  const std::string outfileName = "./ftest/reference_files/small-5.fld";
  const std::string fileData = "./ftest/outfiles_fld/outS.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationLarge1) {
    // Arrange
    const char* script = "./ftest/scripts/test1L.sh";
  const std::string outfileName = "./ftest/reference_files/large-1.fld";
  const std::string fileData = "./ftest/outfiles_fld/outL.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationLarge2) {
    // Arrange
    const char* script = "./ftest/scripts/test2L.sh";
  const std::string outfileName = "./ftest/reference_files/large-2.fld";
  const std::string fileData = "./ftest/outfiles_fld/outL.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationLarge3) {
    // Arrange
    const char* script = "./ftest/scripts/test3L.sh";
  const std::string outfileName = "./ftest/reference_files/large-3.fld";
  const std::string fileData = "./ftest/outfiles_fld/outL.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationLarge4) {
    // Arrange
    const char* script = "./ftest/scripts/test4L.sh";
  const std::string outfileName = "./ftest/reference_files/large-4.fld";
  const std::string fileData = "./ftest/outfiles_fld/outL.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}

TEST(Fluid_Tests, ComparationLarge5) {
    // Arrange
    const char* script = "./ftest/scripts/test5L.sh";
  const std::string outfileName = "./ftest/reference_files/large-5.fld";
  const std::string fileData = "./ftest/outfiles_fld/outL.fld";
  const double tolerance = 1e-1;
    // Act
    int result = std::system(script);
    int equal = compareFiles(outfileName, fileData, tolerance);
    // Assert
    ASSERT_EQ(result, 0);
    ASSERT_EQ(equal, 0);
}


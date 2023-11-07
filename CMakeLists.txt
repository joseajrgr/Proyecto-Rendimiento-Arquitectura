cmake_minimum_required(VERSION 3.21)
project( fluid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler options
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)
set(CMAKE_CXX_FLAGS_RELEASE )
# Enable GoogleTest Library
include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable GSL Library
FetchContent_Declare(GSL
                    GIT_REPOSITORY "https://github.com/microsoft/GSL"
                    GIT_TAG v4.0.0
                    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

# Run clang−tidy on the whole source tree
# Note this will slow down compilation.
# You may temporarily disable but do not forget to enable again.
set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=.*)
# All includes relative to source tree root.
include_directories(PUBLIC .)

# Process cmake from sim and fluid directories
add_subdirectory(sim)
add_subdirectory(fluid)

# Unit tests and functional tests
enable_testing()
add_subdirectory(utest)
add_subdirectory(ftest)

add_executable(fluid  fluid.cpp)
add_library(fluid_h fluid.cpp)
target_link_libraries( fluid sim)
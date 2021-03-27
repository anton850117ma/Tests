# cmake_minimum_required(VERSION 3.0.0)
# project(CMakelists.txt VERSION 0.1.0)

# include(CTest)
# enable_testing()

# add_executable(myprogram main.cpp main.h)
# add_library(mylibrary main.h)

cmake_minimum_required(VERSION 3.0.0)
project(TestProgram)

include(CTest)
enable_testing()

add_executable(testprogram main.cpp)


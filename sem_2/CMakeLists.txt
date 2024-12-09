cmake_minimum_required(VERSION 3.10)
project(PARPROGA_SEM_2)


set(BASE_OPENMP_LIB ${CMAKE_SOURCE_DIR}/BaseOpenMP)


add_executable(BASE_1 ${BASE_OPENMP_LIB}/1.cpp)
add_executable(BASE_2 ${BASE_OPENMP_LIB}/2.cpp)
add_executable(BASE_3 ${BASE_OPENMP_LIB}/3.cpp)

target_compile_options(BASE_1 PUBLIC -fopenmp)
target_link_libraries(BASE_1 omp)
target_compile_options(BASE_2 PUBLIC -fopenmp)
target_link_libraries(BASE_2 omp)
target_compile_options(BASE_3 PUBLIC -fopenmp)
target_link_libraries(BASE_3 omp)
cmake_minimum_required(VERSION 3.10)
project(PARPROGA_SEM_2)


set(BASE_OPENMP_LIB ${CMAKE_SOURCE_DIR}/BaseOpenMP)
set(BORDERED_EQUATION_LIB ${CMAKE_SOURCE_DIR}/BorderedEquation)
set(LABA_2_LIB ${CMAKE_SOURCE_DIR}/laba_2)
set(OUTPUT_FILE ${CMAKE_BINARY_DIR}/out_eq.out)

find_package(MPI)

add_executable(BASE_1 ${BASE_OPENMP_LIB}/1.cpp)
add_executable(BASE_2 ${BASE_OPENMP_LIB}/2.cpp)
add_executable(BASE_3 ${BASE_OPENMP_LIB}/3.cpp)

target_compile_options(BASE_1 PUBLIC -fopenmp)
target_link_libraries(BASE_1 omp)
target_compile_options(BASE_2 PUBLIC -fopenmp)
target_link_libraries(BASE_2 omp)
target_compile_options(BASE_3 PUBLIC -fopenmp)
target_link_libraries(BASE_3 omp)

add_executable(BORDERED_EQUATION ${BORDERED_EQUATION_LIB}/border_eq.cpp)
target_link_libraries(BORDERED_EQUATION omp)
target_compile_options(BORDERED_EQUATION PUBLIC -fopenmp)

add_custom_target(RUN_EQUATION_TO_FILE
COMMAND ${CMAKE_BINARY_DIR}/BORDERED_EQUATION 9 > ${OUTPUT_FILE}
)


add_custom_target(SHOW_EQUATION_GRATH
COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}" python3 ${BORDERED_EQUATION_LIB}/answer_grath.py ${OUTPUT_FILE}
)

add_custom_target(SHOW_EQUATION_BOOST_TIME
COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}" python3 ${BORDERED_EQUATION_LIB}/working_time_graph.py ${CMAKE_BINARY_DIR}/BORDERED_EQUATION
)

add_dependencies(SHOW_EQUATION_GRATH BORDERED_EQUATION RUN_EQUATION_TO_FILE)
add_dependencies(SHOW_EQUATION_BOOST_TIME BORDERED_EQUATION)

add_executable(LABA_2_ORIGIN ${LABA_2_LIB}/origin.cpp)
add_executable(LABA_2_1_ORIGIN ${LABA_2_LIB}/1z.cpp)
add_executable(LABA_2_1_PARALELLED ${LABA_2_LIB}/1z-paralelled.cpp)
target_compile_options(LABA_2_1_PARALELLED PUBLIC -pthread -g)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(LABA_2_1_PARALELLED PUBLIC ${MPI_CXX_LIBRARIES} ${MPI_C_LIBRARIES})
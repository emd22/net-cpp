cmake_minimum_required(VERSION 3.5)

file (GLOB_RECURSE SOURCES "*.cpp")
file (GLOB_RECURSE HEADERS "../include/lib/*.hpp")

set(GCC_COVERAGE_COMPILE_FLAGS "-g")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../include/lib
)
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
  
add_executable(lib ${SOURCES} ${HEADERS})
target_link_libraries(lib Threads::Threads)
#----------------------------------------------
# cmake_minimum_required(VERSION 3.5)

# set(LIB_NAME netcpp)

# project(${LIB_NAME})

# file (GLOB_RECURSE SOURCES "*.cpp")
# file (GLOB_RECURSE HEADERS "../include/${LIB_NAME}/*.hpp")

# set(GCC_COVERAGE_COMPILE_FLAGS "-g")

# include_directories(
#   ${CMAKE_CURRENT_LIST_DIR}/../include/${LIB_NAME}
# )

# add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
# add_library(${LIB_NAME} SHARED ${SOURCES})
# install(TARGETS ${LIB_NAME} DESTINATION ${LIB_NAME}/${LIB_NAME})
# install(FILES ${HEADERS} DESTINATION include/${LIB_NAME})
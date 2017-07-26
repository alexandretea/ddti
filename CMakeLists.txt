cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "ddti")

project(${PROJECT_NAME})

include(ExternalProject)
include(CheckIncludeFileCXX)

# MPI
find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)

include_directories(
    srcs/
    srcs/exceptions
)

set(LIBS
    mlpack
    boost_program_options
    ${MPI_LIBRARIES}
)

file(GLOB sources
	srcs/*.cpp
    srcs/utils/*.cpp
	srcs/exceptions/*.cpp
)
set(SOURCES ${sources})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
                      "${CMAKE_CXX_FLAGS} -std=c++14 -W -Wall -Wextra")
target_link_libraries(${PROJECT_NAME} ${LIBS})

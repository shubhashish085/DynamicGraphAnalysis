cmake_minimum_required(VERSION 2.8)
project(DynamicGraphAnalysis)

include_directories(
        ${PROJECT_SOURCE_DIR}/
)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O3 -g -Wall -march=native -pthread")

set(ANALYSIS_FILE_LIST
        CommandParser.h
        CommandParser.cpp
        MatchingCommand.h
        MatchingCommand.cpp
        Graph.h
        GraphAnalysis.cpp
        types.h)


add_executable(dga.out ${ANALYSIS_FILE_LIST})
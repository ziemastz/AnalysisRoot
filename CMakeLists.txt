cmake_minimum_required(VERSION 3.0...3.29)
project(AnalysisRoot VERSION 0.1.0)

find_package(ROOT 6.30 CONFIG REQUIRED)

include("${ROOT_USE_FILE}")

separate_arguments(ROOT_EXE_LINKER_FLAGS)
add_executable(AnalysisRoot main.cpp)
target_link_libraries(AnalysisRoot PUBLIC ROOT::ROOTDataFrame ROOT::RIO ROOT::Net)

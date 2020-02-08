cmake_minimum_required(VERSION 3.10)
project(bve_koatc)

set(CMAKE_CXX_STANDARD 17)

include_directories(include src subprojects/spdlog-1.5.0/include build/src)
add_definitions(-DATS_EXPORTS -DUNICODE -D_UNICODE)

file(GLOB_RECURSE src_files src/*.h src/*.hxx src/*.cxx)
add_library(Ats ${src_files})

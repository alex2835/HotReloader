cmake_minimum_required(VERSION 3.0.0)
project(HotReloaderMain VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/import)
add_subdirectory(src/export)
add_subdirectory(test)

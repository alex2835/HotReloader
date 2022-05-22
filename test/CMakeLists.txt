cmake_minimum_required(VERSION 3.0.0)
project(HotReloaderTest VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(exe)

set(CPACK_PROJECT_NAME HotReloader)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(HotReloaderMain VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)

set(CPACK_PROJECT_NAME HotReloader)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

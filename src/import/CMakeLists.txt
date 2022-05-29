cmake_minimum_required(VERSION 3.0.0)
project(HotReloaderImport VERSION 0.1.0)

SET(SOURCE
   "import.cpp"
)

add_library(HotReloaderImport STATIC ${SOURCE})

target_include_directories( HotReloaderImport PUBLIC
   ${CMAKE_SOURCE_DIR}/dependencies
   ${CMAKE_SOURCE_DIR}/include
)
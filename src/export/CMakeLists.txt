cmake_minimum_required(VERSION 3.0.0)
project(HotReloaderExport VERSION 0.1.0)

SET(SOURCE
   "function_registry.cpp"
   "export.cpp"
)

add_library(HotReloaderExport STATIC ${SOURCE})

target_include_directories( HotReloaderExport PUBLIC
   ${CMAKE_SOURCE_DIR}/dependencies
   ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(HotReloaderExport PROPERTIES UNITY_BUILD ON)

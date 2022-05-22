cmake_minimum_required(VERSION 3.0.0)
project(HotReloader VERSION 0.1.0)

SET(SOURCE
   "hot_reloader.cpp"
   "exporter.cpp"
)

add_library(HotReloader ${SOURCE})

target_include_directories( HotReloader PUBLIC
   ${CMAKE_SOURCE_DIR}/dependencies
   ${CMAKE_SOURCE_DIR}/include
)

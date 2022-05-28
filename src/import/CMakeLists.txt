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

function( HOT_RELOAD_LIB EXE LIB )
add_custom_command(
   TARGET ${EXE} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy
   $<TARGET_FILE:${LIB}>
   $<TARGET_FILE_DIR:${EXE}>)
endfunction()


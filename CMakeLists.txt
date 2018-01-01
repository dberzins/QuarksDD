cmake_minimum_required(VERSION 3.6)
project("QuarksDD" VERSION 0.0.1)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /DQUARKSDD_SLOW /wd4805 /wd4996")
endif()

# Dependency Includes/Lib directorys
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
link_directories(${CMAKE_SOURCE_DIR}/deps/lib)

# Source and Headers
file(GLOB_RECURSE sources code/*.cpp code/*.h)

set(lib_sources ${sources})
list(REMOVE_ITEM lib_sources ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp)

file(GLOB_RECURSE source_headers code/*.h)
file(GLOB_RECURSE tests tests/*.cpp tests/*.h)

# Project EXE build
add_executable(${PROJECT_NAME} ${sources})

# Shred/Static Lib build
#add_library(${PROJECT_NAME}_shared SHARED ${lib_sources})
add_library(${PROJECT_NAME}_static STATIC ${lib_sources})
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${source_headers})
#set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# INSTALL build
#install (TARGETS ${PROJECT_NAME}_shared DESTINATION  ${CMAKE_SOURCE_DIR}/build/Result/Lib)
install (TARGETS ${PROJECT_NAME}_static DESTINATION  ${CMAKE_SOURCE_DIR}/build/Result/Lib)

install(DIRECTORY  ${CMAKE_SOURCE_DIR}/code/
   DESTINATION  ${CMAKE_SOURCE_DIR}/build/Result/Include/
   FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Tests build
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/Result/Include)
link_directories(${CMAKE_SOURCE_DIR}/build/Result/Lib)

add_executable(Tests ${tests})
target_link_libraries(Tests ${PROJECT_NAME}.lib)

cmake_minimum_required(VERSION 3.2)
project(ginga)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENTDIR ON)

find_package (SDL2 REQUIRED)
find_package (SDL2_image REQUIRED)
find_package (OpenGLES2 REQUIRED)

macro (add_example name file)
  add_executable (${name} ${file})

  target_link_libraries (${name}
    OpenGLES2::OpenGLES2
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
  )

  target_include_directories(${name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}

    ${OPENGLES_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
  )
endmacro ()

add_example (example-01 example-01.cpp)
add_example (example-02 example-02.cpp)


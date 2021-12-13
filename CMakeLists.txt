cmake_minimum_required(VERSION 3.15)

project(Pacman)

find_package(OpenGL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(tinyobjloader)

add_executable(Pacman
	main.cpp
	shaders/playerShad.h
<<<<<<< HEAD
	"shaders/gridShad.h"
    shaders/ghostShad.h
=======
	shaders/gridShad.h
        shaders/ghostShad.h
>>>>>>> 5145f7ac8b64e9242dff3723f7554b3f639c6251
	shaders/modelShader.h
	
	include/glutil.h
	"character.h"
	"character.cpp"
<<<<<<< HEAD
	"pellet.h"
	"pellet.cpp"
=======
>>>>>>> 5145f7ac8b64e9242dff3723f7554b3f639c6251
	"grid.h" 
	"grid.cpp"
	"include.h"
	"globFunc.h"
	"globFunc.cpp"
	"initialize.h"
	"initialize.cpp"
	"ghost.h"
	"ghost.cpp"
	"blockSpawner.h"
	"blockSpawner.cpp"
	"camera.h"
	"camera.cpp" )

target_link_libraries(Pacman
	PRIVATE
	glad
	glfw
	glm
	tinyobjloader
	OpenGL::GL)

  target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/stb/include)
  
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
  STB_IMAGE_IMPLEMENTATION)

  add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/assets/pacman.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/pacman.png)

    add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/assets/wallTexture.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/wallTexture.png)

      add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/assets/ghostModelShader.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/ghostModelShader.png)
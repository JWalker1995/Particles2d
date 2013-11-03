project(particles2d)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
#    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")


set(Particles2d_sources
	src/window.cpp
	src/state/startstate.cpp
	src/state/gamestate.cpp
	src/terrain/simplexnoise.cpp
	src/terrain/terrain.cpp
	src/world/chunk.cpp
	src/world/particletype.cpp
	src/world/world.cpp
	src/world/view.cpp
)

set(Particles2d_headers
	src/defs.h
	src/version.h
	src/gl.h
	src/window.h
	src/state/state.h
	src/state/startstate.h
	src/state/gamestate.h
	src/terrain/simplexnoise.h
	src/terrain/terrain.h
	src/util/logger.h
	src/util/random.h
	src/util/offsetvector.h
	src/util/weakset.h
	src/world/cell.h
	src/world/chunk.h
	src/world/particle.h
	src/world/particletype.h
	src/world/solid.h
	src/world/world.h
	src/world/view.h
)

include_directories(src)
add_executable(${PROJECT_NAME} src/main.cpp ${Particles2d_sources} ${Particles2d_headers})


# OpenGL dependency
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(particles2d ${OPENGL_LIBRARIES} )

# GLEW dependency
find_package(GLEW REQUIRED)
include_directories( ${GLEW_INCLUDE_DIRS} )
target_link_libraries(particles2d ${GLEW_LIBRARIES} )

# GLFW dependency
add_subdirectory(glfw)
include_directories(glfw/include)
target_link_libraries(particles2d glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

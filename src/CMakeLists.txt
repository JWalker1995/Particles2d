set(Particles2d_sources
	window.cpp
	state/startstate.cpp
	state/gamestate.cpp
	terrain/simplexnoise.cpp
	world/world.cpp
)

set(Particles2d_headers
	defs.h
	version.h
	gl.h
	window.h
	state/state.h
	state/startstate.h
	state/gamestate.h
	terrain/simplexnoise.h
	util/logger.h
	util/random.h
	util/offsetvector.h
	world/world.h
)

include_directories(.)
add_executable(${PROJECT_NAME} main.cpp ${Particles2d_sources} ${Particles2d_headers})


# OpenGL dependency
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(particles2d ${OPENGL_LIBRARIES} )

# GLEW dependency
add_library(GLEW SHARED IMPORTED)
include_directories( ${GLEW_INCLUDE_DIRS} )
target_link_libraries(particles2d ${GLEW_LIBRARIES} )

# GLFW dependency
add_library(GLFW SHARED IMPORTED)
include_directories( ${GLFW_INCLUDE_DIRS} )
target_link_libraries(particles2d ${GLFW_LIBRARIES} )

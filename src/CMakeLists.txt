AUX_SOURCE_DIRECTORY(. SRC_LIST)

SET(Particles2d_sources
)

SET(Particles2d_headers
)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${Particles2d_sources} ${Particles2d_headers})

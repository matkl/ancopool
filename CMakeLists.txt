cmake_minimum_required (VERSION 2.6)

project (ancopool)

file(GLOB ancopool_SRC
    "*.h"
    "*.cpp"
)

add_executable(ancopool ${ancopool_SRC})

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL REQUIRED sdl)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} ${SDL_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${PNG_LIBRARIES} ${SDL_LIBRARIES})


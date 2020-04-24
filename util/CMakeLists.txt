file(GLOB ROOT_SOURCE *.cc)
file(GLOB ROOT_HEADER *.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(util STATIC ${ROOT_SOURCE} ${ROOT_HEADER})

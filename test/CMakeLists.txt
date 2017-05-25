include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(util)
add_subdirectory(event)
add_subdirectory(json)
add_subdirectory(parser)
add_subdirectory(engine)
#add_subdirectory(gfx)
add_subdirectory(math)

add_subdirectory(mocks)

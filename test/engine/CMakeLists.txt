CXXTEST_ADD_TEST(
    engine
    engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
)
target_link_libraries(engine
                      lab-engine
                      lab-json
                      lab-parser
                      lab-event
                      mock-gl
                      mock-glfw
                      pthread)

CXXTEST_ADD_TEST(
    screen-manager
    screen-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/screen-manager.hpp
)
target_link_libraries(screen-manager
                      lab-engine
                      lab-json
                      lab-parser
                      lab-event
                      mock-gl
                      mock-glfw
                      pthread)


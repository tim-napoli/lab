CXXTEST_ADD_TEST(
    event
    event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event.hpp
)
target_link_libraries(event lab-event)

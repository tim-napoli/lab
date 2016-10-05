CXXTEST_ADD_TEST(
    event
    event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event.hpp
)
target_link_libraries(event lab-event)

CXXTEST_ADD_TEST(
    source
    source.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source.hpp
)
target_link_libraries(source lab-event)

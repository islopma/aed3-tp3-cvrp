project(CVRP_SRC)

add_library(cvrp_lib STATIC cvrp.cpp)
target_include_directories(cvrp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cvrp main.cpp)
target_link_libraries(cvrp cvrp_lib)

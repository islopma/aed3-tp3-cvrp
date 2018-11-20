project(CVRP_SRC)

add_library(cvrp_lib STATIC cvrp.cpp)
target_include_directories(cvrp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cfrs_lib STATIC cfrs.cpp)
target_include_directories(cfrs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cfrs2_lib STATIC cfrs2.cpp)
target_include_directories(cfrs2_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(union_find_lib STATIC union_find.cpp)
target_include_directories(union_find_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cvrp main.cpp)
target_link_libraries(cvrp cvrp_lib)

add_executable(savings savings/savings.cpp savings/savings_main.cpp)
target_link_libraries(savings cvrp_lib)

add_executable(cfrs cfrs_main.cpp)
target_link_libraries(cfrs cvrp_lib cfrs_lib)

add_executable(cfrs2 cfrs2_main.cpp)
target_link_libraries(cfrs2 cvrp_lib cfrs_lib cfrs2_lib union_find_lib)
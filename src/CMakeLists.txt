project(CVRP_SRC)

add_library(cvrp_lib STATIC cvrp.cpp)
target_include_directories(cvrp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(savings_lib STATIC savings/savings.cpp)
target_include_directories(savings_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(simanneal_lib STATIC simanneal/simanneal.cpp)
target_include_directories(simanneal_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cfrs_lib STATIC cfrs.cpp)
target_include_directories(cfrs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cfrs2_lib STATIC cfrs2.cpp)
target_include_directories(cfrs2_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(union_find_lib STATIC union_find.cpp)
target_include_directories(union_find_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(closest_lib STATIC closest/closest.cpp)
target_include_directories(closest_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cvrp main.cpp)
target_link_libraries(cvrp cvrp_lib)

add_executable(savings savings/savings_main.cpp)
target_link_libraries(savings cvrp_lib savings_lib)

add_executable(savings_time savings/savings_time.cpp)
target_link_libraries(savings_time cvrp_lib savings_lib)

add_executable(simanneal simanneal/simanneal_main.cpp)
target_link_libraries(simanneal cvrp_lib simanneal_lib savings_lib closest_lib)

add_executable(simanneal_time simanneal/simanneal_time.cpp)
target_link_libraries(simanneal_time cvrp_lib simanneal_lib savings_lib closest_lib)

add_executable(cfrs cfrs_main.cpp)
target_link_libraries(cfrs cvrp_lib cfrs_lib)

add_executable(cfrs_time cfrs_time.cpp)
target_link_libraries(cfrs_time cvrp_lib cfrs_lib)

add_executable(cfrs2 cfrs2_main.cpp)
target_link_libraries(cfrs2 cvrp_lib cfrs_lib cfrs2_lib union_find_lib)

add_executable(cfrs2_time cfrs2_time.cpp)
target_link_libraries(cfrs2_time cvrp_lib cfrs_lib cfrs2_lib union_find_lib)

add_executable(closest closest/closest_main.cpp)
target_link_libraries(closest cvrp_lib closest_lib)

add_executable(closest_time closest/closest_time.cpp)
target_link_libraries(closest_time cvrp_lib closest_lib)
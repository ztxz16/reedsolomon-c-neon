project(reedsolomon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread --std=c++11 -O2 -march=native")

if (USENEON)
    add_compile_definitions(USENEON)
endif()

add_executable(main test_rs.cpp)

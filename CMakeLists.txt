cmake_minimum_required(VERSION 3.3)
project(assignment2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES assignment2/main.c assignment2/queue.c assignment2/queue.h assignment2/base.c assignment2/base.h assignment2/util.c assignment2/util.h assignment2/graph.c assignment2/graph.h cTest/demo1.c cTest/demo1.h)

add_executable(assignment2 ${SOURCE_FILES})


project(cTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES cTest/demo8.c cTest/demo8.h)

add_executable(cTest ${SOURCE_FILES})


project(assignment3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES cTest/assignment4/libsat-demo1.c cTest/assignment4/libsat-demo2.c cTest/assignment4/SAT.h )
add_executable(assignment3 ${SOURCE_FILES})

project(assignment4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES zchaff64/mengdongqi.cpp zchaff64/mengdongqi.h zchaff64/SAT.h
        zchaff64/zchaff_utils.cpp
        zchaff64/zchaff_solver.cpp
        zchaff64/zchaff_base.cpp
        zchaff64/zchaff_dbase.cpp
        zchaff64/zchaff_c_wrapper.cpp
        zchaff64/zchaff_cpp_wrapper.cpp)
add_executable(assignment4 ${SOURCE_FILES})

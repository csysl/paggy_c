cmake_minimum_required(VERSION 3.15)
project(paggy_c)

set(CMAKE_CXX_STANDARD 17)

#add_executable(paggy_c test.cpp func.h func.cpp)

add_executable(paggy_c main.cpp init.h
        func.h func.cpp
        key.h key.cpp  #ket agent
        user.h user.cpp #user
        cs.h cs.cpp #server
        #multithread
        user2.cpp
        )

# configure gmp++
target_link_libraries(paggy_c gmp gmpxx)

# configure threads
find_package(Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
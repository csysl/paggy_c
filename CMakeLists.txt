cmake_minimum_required(VERSION 3.15)
project(paggy_c)

set(CMAKE_CXX_STANDARD 17)

#add_executable(paggy_c test.cpp func.h func.cpp)

add_executable(paggy_c src/main.cpp src/init.h
        src/func/func.h src/func/func.cpp
        src/key/key.h src/key/key.cpp  #ket agent
#        user.h user.cpp #user
        src/cs.h src/cs.cpp #server
        #multithread
        src/user2.cpp #cs2.cpp
        )

# configure gmp++
target_link_libraries(paggy_c gmp gmpxx)

# configure threads
find_package(Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
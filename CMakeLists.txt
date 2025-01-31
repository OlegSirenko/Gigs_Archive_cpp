cmake_minimum_required(VERSION 3.28)
project(Gigs_Archive)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system log log_setup REQUIRED)
find_package(CURL)
include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

include_directories(resources modules)

add_executable(Gigs_Archive main.cpp
        modules/CommandHandler.cpp
        modules/CommandHandler.h
        resources/messages.hpp
        resources/config.hpp
        modules/Logger.h)

target_link_libraries(Gigs_Archive /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
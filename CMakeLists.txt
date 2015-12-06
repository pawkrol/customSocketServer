cmake_minimum_required(VERSION 3.3)
project(TCPServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SocketConnector.cpp SocketConnector.h Packet.cpp Packet.h)
add_executable(TCPServer ${SOURCE_FILES})
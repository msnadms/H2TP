cmake_minimum_required(VERSION 3.20)
project(H2TP)

set(CMAKE_CXX_STANDARD 20)

add_executable(H2TP main.cpp HTTPFactory.cpp HTTPFactory.h Server.cpp Websockets/Websocket.cpp Websockets/Websocket.h)

find_library(ws NAMES wsock32 PATHS ${PROJECT_SOURCE_DIR} NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH)

target_link_libraries(${PROJECT_NAME} wsock32)

install (TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/archive )
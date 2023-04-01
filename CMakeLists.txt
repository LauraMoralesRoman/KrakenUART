cmake_minimum_required(VERSION 3.10)

project(ProtocolV3)

# Specify standatd
# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

# Libraries
add_subdirectory(shared)

set (SERVER_FILES serial_port.hpp serial_port.cpp)

add_compile_options(-Wall)
add_executable(ProtocolV3 main.cpp)
target_link_libraries(ProtocolV3 protocol -lpthread)

# Custom targets
add_custom_target(run
    COMMAND ./ProtocolV3
    DEPENDS ./ProtocolV3)

add_executable(ProtocolV3_TX main_tx.cpp ${SERVER_FILES})
add_executable(ProtocolV3_RX main_rx.cpp ${SERVER_FILES})
add_executable(Server server.cpp ${SERVER_FILES})

target_link_libraries(ProtocolV3_TX protocol -lpthread)
target_link_libraries(ProtocolV3_RX protocol -lpthread)
target_link_libraries(Server protocol -lpthread)

set (modem_rx /dev/pts/2)
set (modem_tx /dev/pts/3)

add_custom_target(test
    COMMAND "mprocs" "--names" "'TX','RX'" "--" './ProtocolV3_TX ${modem_tx}' './ProtocolV3_RX ${modem_rx}'
    DEPENDS ProtocolV3_TX ProtocolV3_RX)

add_custom_target(check
    COMMAND cppcheck --std=c++17 --enable=all --inconclusive --inline-suppr ../shared
    DEPENDS)



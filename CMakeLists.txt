cmake_minimum_required(VERSION 3.16)

project(SystemUpdater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SystemUpdater main.cpp)

include(GNUInstallDirs)
install(TARGETS SystemUpdater
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

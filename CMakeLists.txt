
PROJECT(SkOP CXX C)

cmake_minimum_required(VERSION 2.4.0)

#find and setup Qt4 for this project
#find_package(Qt4 REQUIRED)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#include(${QT_USE_FILE})
#add definitions, compiler switches, etc.
#ADD_DEFINITIONS(-Wall -O3 -pg)
SET( CMAKE_CXX_FLAGS "-Wall -O3")

ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(src)
#SET(skop_SRCS
#    main.cpp
#    )

#qt4_automoc(${skop_SRCS})
#list all source files here
#ADD_EXECUTABLE(grev ${grev_SRCS})

#need to link to some other libraries ? just add them here
#TARGET_LINK_LIBRARIES(grev ${QT_LIBRARIES})

# Install
SET(CMAKE_INSTALL_PREFIX "/usr")
INSTALL ( PROGRAMS ${CMAKE_BINARY_DIR}/src/skop
          DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

FILE ( GLOB_RECURSE shaders . src/*.vert src/*.frag)
INSTALL ( FILES ${shaders}
          DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


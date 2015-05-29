# qt-gui project
# original CMakeLists.txt, generated from [src\qt-gui.pro]

cmake_minimum_required( VERSION 2.8.8 )

# CMakeScripts or use the ones that come by default with CMake.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

project( qt-gui )

option(ADD_TABDIALOG "Set ON to build original tabdialog"   OFF)

# Add LINUX or WINDOWS definitions
if(UNIX)
   add_definitions( -DLINUX )
else(UNIX)
   add_definitions( -DWINDOWS )
endif(UNIX)
if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings - none needed in this compile
    set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    #     set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    # endforeach(warning)
    set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS" )
    # if (${MSVC_VERSION} EQUAL 1600)
    #    set( MSVC_LD_FLAGS "/FORCE:MULTIPLE" )
    # endif (${MSVC_VERSION} EQUAL 1600)
    #set( NOMINMAX 1 )
    # to distinguish between debug and release libs
    set( CMAKE_DEBUG_POSTFIX "d" )
else()
    # items for unix
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

#  TIDY_FOUND          - True if tidy found.
#  TIDY_INCLUDE_DIRS   - where to find tidy.h, etc.
#  TIDY_LIBRARIES      - List of libraries when using tidy.
find_package( Tidy )
if (TIDY_FOUND)
    message(STATUS "*** Tidy found. lib ${TIDY_LIBRARIES} inc ${TIDY_INCLUDE_DIRS}")
    list(APPEND add_LIBS ${TIDY_LIBRARIES})
    include_directories( ${TIDY_INCLUDE_DIRS} )
    add_definitions( -DUSE_TIDY_EXTERN )
    message(STATUS "*** Set -DUSE_TIDY_EXTERN")
else ()
    message(STATUS "*** Tidy NOT found! No Tidy GUI2 built")
endif ()    
# configuration file, if needed
# configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h )
# add_definitions( -DHAVE_CONFIG_H )
# include_directories( ${CMAKE_BINARY_DIR} )

# QT = core gui
message(STATUS "*** Finding Qt4 components COMPONENTS QtCore QtGui QtXml QtXmlPatterns")
find_package ( Qt4 COMPONENTS QtCore QtGui QtXml QtXmlPatterns REQUIRED )
include ( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# debug messages
message(STATUS "*** include ${QT_USE_FILE}")
message(STATUS "*** defs  ${QT_DEFINITIONS}")
message(STATUS "*** libs ${QT_LIBRARIES}")

set(name qt-gui)
set(dir src)
set( ${name}_SRCS
    ${dir}/main.cpp
    ${dir}/mainwindow.cpp
    ${dir}/mydialog.cpp
    )
set( ${name}_HDRS
    ${dir}/mainwindow.h
    ${dir}/mydialog.h
    )
set( ${name}_OTHERS
    ${dir}/app_config.h
    )

# Added for DEBUG only
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "*** inc_dirs = ${inc_dirs}")

QT4_WRAP_CPP( ${name}_HDRS_MOC ${${name}_HDRS} )

add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS_MOC} ${${name}_OTHERS} )
target_link_libraries( ${name} ${QT_LIBRARIES} )
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install 
# install(TARGETS qt-gui DESTINATION bin )

##############################################
### Tidy GUI 2
if (TIDY_FOUND)
    set(name tidy-gui2)
    set(dir src/tidy-gui)
    set( ${name}_SRCS
        ${dir}/tg-main.cpp
        ${dir}/tg-dialog.cpp
        ${dir}/tg-config.cpp
        ${dir}/tg-scraps.cpp
        )
    set( ${name}_HDRS
        ${dir}/tg-dialog.h
        ${dir}/tg-config.h
        )
    QT4_WRAP_CPP( ${name}_HDRS_MOC ${${name}_HDRS} )
    add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS_MOC} ${${name}_HDRS} )
    target_link_libraries( ${name} ${QT_LIBRARIES} ${add_LIBS} )
    if (MSVC)
        set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
    endif ()
    # deal with install 
    # install(TARGETS qt-gui DESTINATION bin )
endif ()

##############################################################################
if (ADD_TABDIALOG)
    ### TABDIALOG SAMPLES - originally used for some tests, but does nothing really
    set(name tabdialog)
    set(dir src/tabdialog)
    set( ${name}_SRCS
        ${dir}/tabmain.cpp
        ${dir}/tabdialog.cpp
        ${dir}/tabconfig.cpp
        )
    set( ${name}_HDRS
        ${dir}/tabdialog.h
        ${dir}/tabconfig.h
        )
    QT4_WRAP_CPP( ${name}_HDRS_MOC ${${name}_HDRS} )
    add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS_MOC} ${${name}_HDRS} )
    target_link_libraries( ${name} ${QT_LIBRARIES} )
    if (MSVC)
        set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
    endif ()
    # deal with install 
    # install(TARGETS qt-gui DESTINATION bin )

    # another test app, no longer needed
    set(name qtxml-test)
    set(dir src)
    set( ${name}_SRCS
        ${dir}/${name}.cpp
        )
    add_executable( ${name} ${${name}_SRCS} )
    target_link_libraries( ${name} ${QT_LIBRARIES} )
    if (MSVC)
        set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
    endif ()
    # deal with install 
    # install(TARGETS qt-gui DESTINATION bin )
endif ()

# eof - original generated by Qt2cmake.pl, on 2015/05/24 16:45:49

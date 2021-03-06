############################################################
#                                                          #
#     Part of the Creator Live Music Production Suite.     #
# Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012 #
#                                                          #
#                   All rights reserved.                   #
#                                                          #
############################################################

########################## COMMON ##########################

cmake_minimum_required(VERSION 2.8)

list (APPEND mode-dj_cpp
    collectioncontext.cpp
    djcontext.cpp
    effectstab.cpp
    livebar.cpp
    sampletab.cpp
    samplerdj.cpp
    tracktab.cpp
)

list (APPEND mode-dj_h
)

list (APPEND mode-dj_h_qobject
    collectioncontext.h
    djcontext.h
    effectstab.h
    livebar.h
    sampletab.h
    samplerdj.h
    tracktab.h
)

list (APPEND mode-dj_ui
    collectioncontext.ui
    djcontext.ui
    effectstab.ui
    sampletab.ui
    livebar.ui
    tracktab.ui
)

list (APPEND mode-dj_qrc
)

################## PLATFORM-SPECIFIC CODE ##################

if (QNX)
else (QNX)
endif (QNX)

########################## BUILD! ##########################

add_definitions(-Wall)

set(CMAKE_AUTOMOC ON)
find_package (Qt5Widgets REQUIRED)

include_directories (${cmake_binary_dir}
    ${CMAKE_BINARY_DIR}/mode-dj
    ../liblivecore/
    ../liblivewidgets/
)

qt5_wrap_ui (mode-dj_moc ${mode-dj_ui}) 
qt5_add_resources (livevst_rcc ${livevst_qrc})

add_library (mode-dj SHARED
    ${mode-dj_cpp}
    ${mode-dj_moc}
    ${livevst_qrc}
)

if (NOT WIN32)
set_target_properties(mode-dj PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
else (NOT WIN32)
set_target_properties(mode-dj PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
endif (NOT WIN32)

if (WIN32)
target_link_libraries (mode-dj
    livecore
    livewidgets
    ${QT_LIBRARIES}
)
endif (WIN32)

qt5_use_modules(mode-dj Widgets)

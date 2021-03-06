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

list (APPEND app-sampler_cpp
    samplerapp.cpp
    samplerframe.cpp
    sampler_dotlive.cpp
)

list (APPEND app-sampler_h
)

list (APPEND app-sampler_h_qobject
    samplerapp.h
    samplerframe.h
)

list (APPEND app-sampler_ui
    samplerframe.ui
)

list (APPEND app-sampler_qrc
    sampler_icons.qrc
)

################## PLATFORM-SPECIFIC CODE ##################

if (QNX)
else (QNX)
endif (QNX)

file (INSTALL app_sampler.png DESTINATION ${CMAKE_BINARY_DIR}/plugins/icons)  

########################## BUILD! ##########################

add_definitions(-Wall)

set(CMAKE_AUTOMOC ON)
find_package (Qt5Widgets REQUIRED)

include_directories (${cmake_binary_dir}
    ${CMAKE_BINARY_DIR}/app-sampler
    ../liblivecore/
    ../liblivewidgets/
)

qt5_wrap_ui (app-sampler_moc ${app-sampler_ui}) 
qt5_add_resources (app-sampler_rcc ${app-sampler_qrc})

add_library (app-sampler SHARED
    ${app-sampler_cpp}
    ${app-sampler_moc}
    ${app-sampler_rcc}
)

if (NOT WIN32)
set_target_properties(app-sampler PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
else (NOT WIN32)
set_target_properties(app-sampler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
endif (NOT WIN32)

if (WIN32)
target_link_libraries (app-sampler
    livecore
    livewidgets
    ${QT_LIBRARIES}
)
endif (WIN32)
qt5_use_modules(app-sampler Widgets)

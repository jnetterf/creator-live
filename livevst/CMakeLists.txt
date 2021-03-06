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

list (APPEND livevst_cpp
    vstWin.cpp
    vstlinux.cpp
    vstinterface.cpp
    vstselectionwidget.cpp
    vstsettingswidget.cpp
    vst_dotlive.cpp
    vstwizard.cpp
)

list (APPEND livevst_h
)

list (APPEND livevst_h_qobject
    vstlinux_p.h
    vstinterface.h
    vstselectionwidget.h
    vstsettingswidget.h
    vstwizard.h
    hackwidget.h
    vst.h
    vst_p.h
    vstsidekick.h
)

list (APPEND livevst_ui
    vstselectionwidget.ui 
    vstsettingswidget.ui
    vstwizard.ui
)

list (APPEND livevst_qrc
    icons.qrc
)

################## PLATFORM-SPECIFIC CODE ##################

if (QNX)
else (QNX)
endif (QNX)

########################## BUILD! ##########################

add_definitions(-Wall)

set(CMAKE_AUTOMOC ON)
find_package (Qt5Widgets REQUIRED)

include_directories (
    .
    ${CMAKE_BINARY_DIR}/livevst
    ../liblivecore/
    ../liblivewidgets/
)

qt5_wrap_ui (livevst_moc ${livevst_ui}) 
qt5_add_resources (livevst_rcc ${livevst_qrc})

add_library (livevst SHARED
    ${livevst_cpp}
    ${livevst_moc}
    ${livevst_qrc}
)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIVEVST_LIBRARY=1")
target_link_libraries (livevst
    livecore
    ${QT_LIBRARIES}
)
endif (WIN32)

set_target_properties(livevst PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

qt5_use_modules(livevst Widgets)

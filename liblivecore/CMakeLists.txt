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

list (APPEND liblivecore_cpp
    anabeat.cpp
    audiosecond.cpp
    mapping.cpp 
    songsystem.cpp 
    metronome.cpp 
    timesignature.cpp 
    variantbinding.cpp 
    keysignature.cpp 
    pitch.cpp 
    midifilter.cpp 
    midibinding.cpp 
    asyncconnect.cpp 
    midievent.cpp 
    core_dotlive.cpp 
    chord.cpp 
    time.cpp 
    object.cpp 
    audiograph.cpp
    audiotrack.cpp 
    miditrack.cpp 
    midieventcounter.cpp 
    extern/midifile/src/Options_private.cpp 
    extern/midifile/src/MidiFile.cpp 
    extern/midifile/src/FileIO.cpp 
    extern/midifile/src/Options.cpp 
    appinterface.cpp 
    audiointerface.cpp 
    instrumentinterface.cpp 
    app.cpp 
    instrument.cpp 
    midibridge.cpp
    ambition.cpp
    modeinterface.cpp
)

list (APPEND liblivecore
    live/audio 
    live/audiograph
    live/liblivecore_global 
    live/songsystem 
    live/timesignature 
    live/mapping 
    live/keysignature 
    live/midifilter 
    live/pitch 
    live/midievent 
    live/asyncconnect 
    live/chord 
    live/time 
    live/core 
    live/audiointerface 
    live/appinterface 
    live/instrumentinterface 
    live/midi 
    live/app 
    live/instrument 
    live/midieventcounter 
    live/modeinterface
)

list (APPEND liblivecore_qobject
    live/anabeat 
    live/audiosecond
    live/ambition
    live/metronome 
    live/variantbinding 
    live/midibinding 
    asyncconnect_p.h 
    live/object 
    live/audiotrack 
    live/miditrack 
    midibridge.h
)

################## PLATFORM-SPECIFIC CODE ##################

if (QNX)
    list (APPEND liblivecore_cpp
        audio_qnx.cpp
        midiqnx.cpp 
    )
    list (APPEND liblivecore
    )
    list (APPEND liblivecore_qobject
        audiosystem_qnx_p.h 
        midisystem_p.h 
    )
else (QNX)
    list (APPEND liblivecore_cpp
        audio.cpp
        midi.cpp 
    )
    list (APPEND liblivecore_qobject
        audiosystem_p.h 
        midisystem_p.h 
    )
endif (QNX)

########################## BUILD! ##########################

add_definitions(-Wall)

set(CMAKE_AUTOMOC OFF)
find_package (Qt5Core)
find_package (Qt5Network)
find_package (Qt5Concurrent)

include_directories (${cmake_binary_dir}
    .
)

qt5_wrap_cpp (liblivecore_cpp ${liblivecore_qobject})

add_library (livecore SHARED
    ${liblivecore_cpp}
    ${liblivecore_moc}
)

if (WIN32)
target_link_libraries (livecore
    #${creator-live_libs}
    ${QT_LIBRARIES}
    "C:/Program Files (x86)/Jack/lib/libjack.lib"
    C:/QtSDK/Desktop/Qt/4.8.1/msvc2010/lib/portmidi.lib
)
else (WIN32)
if (QNX)
else (QNX)
target_link_libraries (livecore
        portmidi
        jack
        fluidsynth
)
endif (QNX)
endif (WIN32)

if (NOT WIN32)
set_target_properties(livecore PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
else (NOT WIN32)
set_target_properties(livecore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
endif (NOT WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBLIVECORE_LIBRARY=1")

if (!WIN32)
if (!QNX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Weffc++ -pedantic -pedantic-errors -Wextra  -Wall -Wcast-align -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion -Wdisabled-optimization -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security  -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wtrigraphs  -Wuninitialized -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings")
endif (!QNX)
endif (!WIN32)

qt5_use_modules(livecore Core Concurrent Network)

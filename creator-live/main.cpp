/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "liveapplication.h"

#include <live/audio>

int main(int argc,char** argv)
{
    std::cout<<"Creator Live is copyright Joshua Netterfield 2010-2012. All rights are reserved."<<std::endl;
    LiveApplication* liveApp=new LiveApplication(argc,argv);
    int ret = liveApp->exec();
    live::audio::stop();
    return ret;
}

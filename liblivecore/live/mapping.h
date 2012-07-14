/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "live/object.h"

#ifndef MAPPING_H
#define MAPPING_H

namespace live {

/**
 * A mapping is an interface to an object, just with a different name.
 */
class LIBLIVECORESHARED_EXPORT Mapping : public live::Object
{
    ObjectPtr s_other;
public:
    /**
     * Create a Object which is just another name for a different object.
     */
    Mapping(QString cname,ObjectPtr other);

    void aIn (const float*data,int chan,ObjectChain&p);
    virtual bool isAudioObject();

    virtual void mIn(const Event*data,ObjectChain&p);
    virtual bool isMidiObject();

    virtual bool isInput();
    virtual bool isOutput();
};

}

#endif // MAPPING_H
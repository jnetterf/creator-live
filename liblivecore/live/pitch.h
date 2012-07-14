/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef PITCH_H_CON
#define PITCH_H_CON

#include <QString>

#include "liblivecore_global.h"

namespace live {

class KeySignature;

class LIBLIVECORESHARED_EXPORT Pitch
{
public:
    enum Accidental
    {
        DoubleSharp=2,
        Sharp=1,
        Natural=0,
        Flat=-1,
        DoubleFlat=-2
    };
    enum
    {
        AUTOOCTAVE=-12,
        INVALID=-66
    };

    char s_root;                /*003*/
    Accidental s_accidental;    /*004*/
    int s_octave;               /*005*/
    bool valid;

public:
    Pitch(char croot,Accidental caccidental,int coctave=AUTOOCTAVE);
    Pitch(char croot,char caccidental,int coctave=AUTOOCTAVE);
    Pitch(QString cpitch,int coctave=AUTOOCTAVE);
    int midiNote();
    QByteArray save();
    static Pitch* load(const QByteArray&str);
protected:
    Pitch() {}
};

}

#endif // PITCH_H_CON
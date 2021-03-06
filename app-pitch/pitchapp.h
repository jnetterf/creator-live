/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef PITCH_H
#define PITCH_H

#include <live/object>

class PitchAppAudioR;

class PitchApp : public QObject, public live::Object
{
    Q_OBJECT
    PitchAppAudioR* m_audioR;
    live::Connection m_connections;
public:
    LIVE_HYBRID
    LIVE_EFFECT
    friend class AppSys;
    int m_stShift;  /*003*/
    int m_id;       /*004*/
    static int m_lastId;

public:
    RELOADABLE(PitchApp)
    PitchApp();
    ~PitchApp();

    const int& shiftAmount();

public slots:
    void shiftUp();
    void shiftDown();
    void setShift(const int& s);
    void aIn(const float *data, int chan, Object *p);
    void mIn(const live::Event *data, live::ObjectChain*p);
};

#endif // PITCH_H

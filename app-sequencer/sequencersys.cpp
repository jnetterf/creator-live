/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "sequencersys.h"

using namespace live;
//using namespace live_widgets;

#define SEQSYS_CLIPF_BEGIN \
    if (ignoreLock) return; \
    Object::beginAsyncAction();\
    ignoreLock=1; \
    SequencerApp*send=(qobject_cast<SequencerApp*>(sender())); \
    Object::endAsyncAction(); \
    Q_ASSERT(send); \
    Q_ASSERT(send->clipped())

#define SEQSYS_CLIPF_END \
    ignoreLock=0

void SequencerSys::registerClippedSeq(SequencerApp* that)
{
    (self?self:new SequencerSys)->registerSeq_real(that);
}
void SequencerSys::deregisterClippedSeq(SequencerApp *that)
{
    (self?self:new SequencerSys)->deregisterSeq_real(that);
}
int SequencerSys::newIdForTrack()
{
    return ++(self?self:new SequencerSys)->_lastId;
}

void SequencerSys::registerSeq_real(SequencerApp* that)
{
    Object::beginAsyncAction();

    Q_ASSERT(!_u.contains(that));
    // states __must be__ identical
    if (_u.size())
    {
        if (_u[0]->isPlaying()&&!that->isPlaying())
        {
            that->startPlayback();
        }
        else if (!_u[0]->isPlaying()&&that->isPlaying())
        {
            that->stopPlayback();
        }

        if (that->pos()!=_u[0]->pos())
        {
            that->setPos(_u[0]->pos());
        }
    }

    connect(that,SIGNAL(playbackStarted()),this,SLOT(startPlayback()));
    connect(that,SIGNAL(playbackStopped()),this,SLOT(stopPlayback()));
    connect(that,SIGNAL(posSet(int)),this,SLOT(setPos(int)));
    _u.push_back(that);
    Object::endAsyncAction();
}

void SequencerSys::deregisterSeq_real(SequencerApp* that)
{
    Object::beginAsyncAction();

    Q_ASSERT(_u.contains(that));
    that->disconnect(this);
    _u.removeOne(that);

    Object::endAsyncAction();
}

SequencerSys::~SequencerSys()
{
    self=0;
}

void SequencerSys::startPlayback()
{
    SEQSYS_CLIPF_BEGIN;

    for (int i=0;i<_u.size();i++)
    {
        if (_u[i]!=send&&_u[i]->clipped())
        {
            _u[i]->startPlayback();
        }
    }

    SEQSYS_CLIPF_END;
}

void SequencerSys::stopPlayback()
{
    SEQSYS_CLIPF_BEGIN;

    for (int i=0;i<_u.size();i++)
    {
        if (_u[i]!=send&&_u[i]->clipped())
        {
            _u[i]->stopPlayback();
        }
    }

    SEQSYS_CLIPF_END;
}

void SequencerSys::setPos(int pos)
{
    SEQSYS_CLIPF_BEGIN;

    for (int i=0;i<_u.size();i++)
    {
        if (_u[i]!=send&&_u[i]->clipped())
        {
            _u[i]->setPos(pos);
        }
    }

    SEQSYS_CLIPF_END;
}

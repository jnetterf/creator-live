/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef PIANO_KEY_H
#define PIANO_KEY_H

#include <QtGlobal>
#include <QGraphicsRectItem>
#include "live/object"
#include "liblivewidgets_global.h"

class LiveInputMIDI;
class LiveOutputMIDI;

namespace live_widgets {

class LIBLIVEWIDGETSSHARED_EXPORT PianoKey : public QObject, public live::Object, public QGraphicsRectItem {
    Q_OBJECT
private:
    static int m_shiftTwinID;
    static live_widgets::PianoKey* m_universe[300];
    bool m_virtual;
    qint16 m_id;
    bool m_enabled;

public:
    LIVE_MIDI
    LIVE_EFFECT //fixme==> hidden
    bool mOn() const{ return 0; } bool aOn() const { return 1; }
    explicit PianoKey( qreal x, qreal y, qreal width, qreal height, qint16 id, QGraphicsItem * parent );

    void enableKey(bool enabled);
    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
    void setID(qint16 id );
    void setVirtual( bool isVirtual ) {
        m_virtual = isVirtual;
    }
    void mIn(const live::Event *data, live::ObjectChain*p);

    int id() const {
        return m_id;
    }
    int isEnabled() const {
        return m_enabled;
    }
    bool isVirtual() const {
        return m_virtual;
    }

signals:
    void updated();
};

}

#endif//PIANO_KEY_H

// Yay!

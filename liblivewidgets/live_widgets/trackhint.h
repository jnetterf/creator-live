/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef TRACKHINT_H
#define TRACKHINT_H

#include <QWidget>
#include "liblivewidgets_global.h"

#include <live/audio>
#include <live/object>

namespace Ui {
    class TrackHint;
}

namespace live_widgets {

class LIBLIVEWIDGETSSHARED_EXPORT TrackHint : public QWidget {
    Q_OBJECT

public:
    explicit TrackHint(QWidget *parent = 0);
    ~TrackHint();

private:
    Ui::TrackHint* ui;

    TrackHint(const TrackHint&)
      : QWidget()
      , ui(0)
      { TCRASH();
    }

    TrackHint& operator=(const TrackHint&) {
        TCRASH();
        return *this;
    }
};

}

#endif // TRACKHINT_H

/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef TRACKTAB_H
#define TRACKTAB_H

#include <QWidget>
#include <live_widgets/bindableparent.h>

namespace Ui {
class tracktab;
}

class TrackTab : public QWidget, public live_widgets::BindableParent
{
    Q_OBJECT
    
public:
    explicit TrackTab(QWidget *parent = 0);
    ~TrackTab();
    
private:
    Ui::tracktab *ui;
};

#endif // TRACKTAB_H

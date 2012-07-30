/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "collectioncontext.h"
#include "ui_collectioncontext.h"

#include "live/audio.h"
#include "live/object.h"

#include <QTimer>

using namespace live;

CollectionContext::CollectionContext(DJContext *parent) :
    QWidget(parent)
  , s_parent(parent)
  , ui(new Ui::CollectionContext)
{
    ui->setupUi(this);

    connect(live::object::singleton(), SIGNAL(stockChanged()), this, SLOT(stockChanged_logic()));
    connect_changeEvents();
    QTimer::singleShot(0, this, SLOT(stockChanged_logic()));
}

CollectionContext::~CollectionContext()
{
    delete ui;
}

void CollectionContext::connect_changeEvents()
{
    connect(ui->comboBox_audio, SIGNAL(currentIndexChanged(QString)), s_parent, SLOT(audioChanged_logic(QString)));
    connect(ui->comboBox_monitor, SIGNAL(currentIndexChanged(QString)), s_parent, SLOT(monitorChanged_logic(QString)));
}

void CollectionContext::disconnect_changeEvents()
{
    disconnect(ui->comboBox_audio, SIGNAL(currentIndexChanged(QString)), s_parent, SLOT(audioChanged_logic(QString)));
    disconnect(ui->comboBox_monitor, SIGNAL(currentIndexChanged(QString)), s_parent, SLOT(monitorChanged_logic(QString)));
}

void CollectionContext::stockChanged_logic()
{
    QList< ObjectPtr > ptrs = object::get(AudioOnly | OutputOnly);
    QStringList names;
    for(int i = 0; i < ptrs.size(); i++)
    {
        names.push_back(ptrs[i]->name());
    }

    disconnect_changeEvents();
    QString sa = ui->comboBox_audio->currentText();
    QString sm = ui->comboBox_monitor->currentText();
    ui->comboBox_audio->clear();
    ui->comboBox_monitor->clear();
    ui->comboBox_audio->addItems(names);
    ui->comboBox_monitor->addItems(names);
    for (int i = 0; i < 2; i++)
    {
        if (names.indexOf(i ? sa : sm) == -1)
        {
            if (!i) s_parent->audioChanged_logic(ui->comboBox_audio->currentText());
            else s_parent->monitorChanged_logic(ui->comboBox_monitor->currentText());
        }
        else
        {
            (i ? ui->comboBox_audio : ui->comboBox_monitor)->setCurrentIndex
                    (names.indexOf(i ? sa : sm));
        }
    }
    connect_changeEvents();
}
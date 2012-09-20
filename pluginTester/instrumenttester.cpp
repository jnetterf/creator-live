/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "instrumenttester.h"
#include "ui_instrumenttester.h"

using namespace live;

InstrumentTester::InstrumentTester(live::ObjectPtr in, live::InstrumentInterface *inst, QWidget *parent) :
    QWidget(parent),
    s_inst(inst),
    s_in(in),
    s_sett(0),
    ui(new Ui::InstrumentTester)
{
    ui->setupUi(this);
    connect(ui->radioButton_select,SIGNAL(toggled(bool)),this,SLOT(configAction(bool)));
    connect(ui->radioButton_settings,SIGNAL(toggled(bool)),this,SLOT(setupAction(bool)));
    ui->widget_configuration->setLayout(new QVBoxLayout);
}

InstrumentTester::~InstrumentTester()
{
    delete ui;
}

void InstrumentTester::configAction(bool a)
{
    if(!a) return;

    if(s_sett) {
        s_sett->deleteLater();
        s_sett=0;
    }

    (s_sett=qobject_cast<QWidget*>(s_inst->selectionWidget(s_instObj,s_out)))->show();
    ui->widget_configuration->layout()->addWidget(s_sett);
    connect(s_sett,SIGNAL(instrumentUpdated(live::ObjectPtr, live::ObjectPtr)),this,SLOT(changeInst(live::ObjectPtr, live::ObjectPtr)));
}

void InstrumentTester::setupAction(bool a)
{
    if(!a) return;
    if(s_sett) {
        s_sett->deleteLater();
        s_sett=0;
    }
    (s_sett=qobject_cast<QWidget*>(s_inst->settingsWidget()))->show();
    ui->widget_configuration->layout()->addWidget(s_sett);
}

void InstrumentTester::changeInst(ObjectPtr inst, ObjectPtr loop)
{
    qDebug() << "CHANGE_INST";
    for (int i = 0; i < s_connections.size(); ++i) {
        if (s_connections[i].a==s_in && s_connections[i].b == s_instObj)
            s_connections.removeAt(i--);
        if (s_connections[i].a==s_instObj && s_connections[i].b == s_out)
            s_connections.removeAt(i--);
    }
    s_instObj=inst;
    s_out=loop;
    if(s_in.valid()&&s_instObj.valid()) s_connections.push_back(Connection(s_in,s_instObj,live::MidiConnection));
    if(s_instObj.valid()&&s_out.valid()) s_connections.push_back(Connection(s_instObj, s_out,live::HybridConnection));
}

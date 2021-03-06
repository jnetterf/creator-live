/*****************************************************
SamplerFrame.cpp                           rev. 110731

  Part of the Creator Live Music Workstation Project
  Copyright Joshua Netterfield  (trintith@gmail.com)

                 All rights reserved.
*****************************************************/

#include "samplerframe.h"
#include "ui_samplerframe.h"
#include <QFileDialog>

#include <QtPlugin>

#include <live/midibinding>
#include <live/midi>

using namespace live;

SamplerFrame::SamplerFrame(SamplerApp* app, AbstractTrack *parent) :
    AppFrame(parent), m_app(*app), currentCM(0), m_activeButton(-1), ui(new Ui::Sampler)
{
    for (int i=0;i<16;i++)
    {
        m_times[i]=0;
    }
    ui->setupUi(this);
    connect(ui->modeMulti,SIGNAL(toggled(bool)),&m_app,SLOT(setMultiMode(bool)));
    connect(ui->modeRec,SIGNAL(toggled(bool)),&m_app,SLOT(setRecordMode(bool)));
    QButtonGroup* bg=new QButtonGroup(this);
    bg->setExclusive(1);
    bg->addButton(ui->modeRec);
    bg->addButton(ui->modePlay);

    connect(ui->play1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play2,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play3,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play4,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play5,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play6,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play7,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play8,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play9,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play10,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play11,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play12,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play13,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play14,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play15,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play16,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextLogic(QPoint)));
    connect(ui->play1,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play2,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play3,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play4,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play5,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play6,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play7,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play8,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play9,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play10,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play11,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play12,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play13,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play14,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play15,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play16,SIGNAL(pressed()),this,SLOT(buttonLogic()));
    connect(ui->play1,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play2,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play3,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play4,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play5,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play6,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play7,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play8,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play9,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play10,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play11,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play12,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play13,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play14,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play15,SIGNAL(released()),this,SLOT(buttonLogic()));
    connect(ui->play16,SIGNAL(released()),this,SLOT(buttonLogic())); //ROTFLOL
}

static void removeBackground(QString& style)
{
    if (style.contains("background-color"))
    {
        int start=style.indexOf("background-color");
        int end=style.indexOf(";",start);
        if (start!=end&&start!=-1) style.remove(start,end-start);
    }
}

void SamplerFrame::buttonLogic()
{
    Q_ASSERT(sender());
    QToolButton* button=qobject_cast<QToolButton*>(sender());
    Q_ASSERT(button);

    QString name=sender()->objectName();
    name.remove("play");
    bool ok=1;
    int num=name.toInt(&ok);
    Q_ASSERT(ok);
    if (qobject_cast<QToolButton*>(sender())->isDown())
    {
        QString style=button->styleSheet();

        removeBackground(style);
        style=style+"background-color:"+(ui->modeRec->isChecked()?"red":"green")+";";
        button->setStyleSheet(style);

        m_app.hit(num-1);
        m_times[num-1]=midi::getTime_msec();
    }
    else if (m_app.isPlayMode()&&midi::getTime_msec()-m_times[num-1]<100)
    {
        // play it until completion
        // FIXME : add timer
//        m_app.m_audioTracks[0]->length();
    }
    else
    {
        QString style=button->styleSheet();
        removeBackground(style);
        button->setStyleSheet(style);
        m_times[num-1]=0;
        m_app.release(num-1);
    }
}

void SamplerFrame::contextLogic(QPoint)
{
    Q_ASSERT(sender());
    Q_ASSERT(qobject_cast<QToolButton*>(sender()));

    if ( currentCM != 0 ) {
        delete currentCM;
        currentCM = 0;
    }

    QString name=sender()->objectName();
    name.remove("play");
    bool ok=1;
    int num=name.toInt(&ok)-1;
    Q_ASSERT(ok);
    if (!ok)
    {
        return;
    }

    currentCM = new QMenu;
    currentCM->addAction("Load MIDI/Audio",this,SLOT(importLogic()));
    currentCM->addAction("Export MIDI/Audio",this,SLOT(exportLogic()));

    ok=0;
    m_activeButton=num;
    for (int h=0;h<m_app.m_customBindings.keys();h++)
    {
        for (int i=0;i<200;i++)
        {
            if (m_app.m_customBindings.value(m_app.m_customBindings.keys()[h])[i]==num)
            {
                currentCM->addAction("Remove binding",this,SLOT(bindingLogic()));
                ok=1;
                break;
            }
        }
    }
    if (!ok)
    {
        currentCM->addAction("Bind next MIDI button to hold",this,SLOT(bindingLogic()));
    }

    currentCM->exec( QCursor::pos() );

}

void SamplerFrame::importLogic()
{
    Q_ASSERT(m_activeButton!=-1&&m_activeButton<16&&m_activeButton>=0);

    QString file=QFileDialog::getOpenFileName(this,QString("Import file"),QString(),"Audio Files (*.wav *.flac *.ogg);;MIDI Files (*.MIDI *.midi *.MID *.mid)");
    if (QFile::exists(file)&&(file.endsWith("flac",Qt::CaseInsensitive)||file.endsWith("ogg",Qt::CaseInsensitive)||file.endsWith("wav",Qt::CaseInsensitive)))
    {
        m_app.m_audioTracks[m_activeButton]->importFile(file);
    }
    else if (QFile::exists(file)&&(file.endsWith("midi")||file.endsWith("MIDI")||file.endsWith("mid")||file.endsWith("MID")))
    {
        m_app.m_midiTracks[m_activeButton]->importFile(file);
    }
}

void SamplerFrame::exportLogic()
{
    Q_ASSERT(m_activeButton!=-1&&m_activeButton<16&&m_activeButton>=0);

    QString file=QFileDialog::getSaveFileName(this,QString("Export file"),QString(),"Audio Files (*.wav *.flac *.ogg);;MIDI Files (*.MIDI *.midi *.MID *.mid)");
    if (file.endsWith("flac",Qt::CaseInsensitive)||file.endsWith("ogg",Qt::CaseInsensitive)||file.endsWith("wav",Qt::CaseInsensitive))
    {
        m_app.m_audioTracks[m_activeButton]->exportFile(file);
    }
    else if (file.endsWith("midi")||file.endsWith("MIDI")||file.endsWith("mid")||file.endsWith("MID"))
    {
        m_app.m_midiTracks[m_activeButton]->exportFile(file);
    }
}

void SamplerFrame::bindingLogic()
{
    Q_ASSERT(m_activeButton!=-1&&m_activeButton<16&&m_activeButton>=0);

    int oid=-1,ohd=-1;
    for (int h=0;h<m_app.m_customBindings.keys();h++)
    {
        for (int i=0;i<200;i++)
        {
            if (m_app.m_customBindings.value(m_app.m_customBindings.keys()[h])[i]==m_activeButton)
            {
                oid=i;
                ohd=h;
                break;
            }
        }
        if (oid!=-1) {
            break;
        }
    }
    if (oid==-1) //new binding
    {
        m_app.setBindingMode(m_activeButton);
    }
    else    //remove binding
    {
        m_app.m_customBindings.value(m_app.m_customBindings.keys()[ohd])[oid]=-1;
        MidiBinding::customKey->value(m_app.m_customBindings.keys()[ohd])[oid]=0;
    }
}

SamplerFrame::~SamplerFrame()
{
    m_app.deleteLater();
    delete ui;
}


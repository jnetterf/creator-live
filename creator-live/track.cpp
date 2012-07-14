/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "track.h"
#include "live/app.h"
#include "live_widgets/trackhint.h"

int Track::s_lastId=-1;
using namespace live;
using namespace live_widgets;

Track::Track(live::ObjectPtr cinput, live::ObjectPtr coutput) :
    BindableParent(this),
    s_th(new TrackHint),
    s_ambition(*(new Ambition(cinput,ObjectChain(),coutput))),
    s_appUi_(),
    s_id(++s_lastId),

    ui_outputName(new RotatedLabel(this)),
    ui_mainLayout(new QHBoxLayout(this)),
    ui_spacerItem(0),
    ui_chainWidget(new ChainTypeWidget(this))
{
    /*MAKE CHANGES IN BELOW CONSTRUCTOR TOO!!!!*/
    ui_chainWidget->setGeometry(0,0,width(),3);
    ui_chainWidget->setFixedHeight(2);
    setAcceptDrops(1);
    setLayout(ui_mainLayout);
    ui_mainLayout->setAlignment(Qt::AlignLeft);
    ui_outputName->setMinimumHeight(200);
    s_th->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui_mainLayout->addWidget(s_th);
//    ui_mainLayout->addWidget(ui_midiSelect=new OutputSelectionWidget);
    ui_mainLayout->addWidget(ui_outputName,0,Qt::AlignRight);
//    ui_mainLayout->addSpacerItem(ui_spacerItem=new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));
//    ui_midiSelect->hide();
    Q_ASSERT(ui_mainLayout->setAlignment(ui_outputName,Qt::AlignRight));
    ui_mainLayout->setSpacing(0);

    ui_outputName->setMaximumWidth(20);
    this->setLayout(ui_mainLayout);

    ui_outputName->setText(s_ambition.b_output.ref());
    ui_outputName->setAlignment(Qt::AlignRight);

    connect(&s_ambition.b_output,
            SIGNAL(changeObserved(QString,QString)),
            ui_outputName,
            SLOT(setText(QString)));

    connect(ui_outputName,SIGNAL(clicked()),this,SLOT(outputSelection()));
    setMinimumHeight(250);
    setMaximumHeight(250);
//    connect(ui_midiSelect,SIGNAL(indexSet(int)),this,SLOT(selectOutput(int)));
    MidiBindingQtSys::addWidget(this);

    ui_outputName->setObjectName("ui_outputName");
    setObjectName("Track_"+QString::number(s_id));
    setGeometry(geometry());
}

Track::Track(Ambition* bp) :
    BindableParent(this),
    s_th(0),
    s_ambition(*bp),
    s_appUi_(),
    s_id(-1),

    ui_outputName(new RotatedLabel(this)),
    ui_mainLayout(new QHBoxLayout(this)),
    ui_spacerItem(0),
    ui_chainWidget(new ChainTypeWidget(this))
{
    ui_chainWidget->setGeometry(0,0,width(),3);
    ui_chainWidget->setFixedHeight(2);
    setAcceptDrops(1);
    setLayout(ui_mainLayout);
    ui_mainLayout->setAlignment(Qt::AlignLeft);
    ui_outputName->setMinimumHeight(200);
//    ui_mainLayout->addWidget(ui_midiSelect=new OutputSelectionWidget);
    ui_mainLayout->addWidget(ui_outputName,50,Qt::AlignRight);
//    ui_mainLayout->addSpacerItem(ui_spacerItem=new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));
//    ui_midiSelect->hide();
    Q_ASSERT(ui_mainLayout->setAlignment(ui_outputName,Qt::AlignRight));
    ui_mainLayout->setSpacing(0);

    ui_outputName->setMaximumWidth(20);
    this->setLayout(ui_mainLayout);

    ui_outputName->setText(s_ambition.b_output.ref());
    ui_outputName->setAlignment(Qt::AlignRight);

    connect(&s_ambition.b_output,
            SIGNAL(changeObserved(QString,QString)),
            ui_outputName,
            SLOT(setText(QString)));

    connect(ui_outputName,SIGNAL(clicked()),this,SLOT(outputSelection()));
    setMinimumHeight(250);
    setMaximumHeight(250);
//    connect(ui_midiSelect,SIGNAL(indexSet(int)),this,SLOT(selectOutput(int)));
    MidiBindingQtSys::addWidget(this);
    ui_outputName->setObjectName("ui_outputName");
    setGeometry(geometry());
}

Track::~Track()
{
    qDebug()<<"DESTROY TRACK";
    delete ui_outputName;
    delete ui_mainLayout;
    foreach(QWidget* a,s_appUi_)
    {
        delete a;
    }

    delete &s_ambition;
}

void Track::resizeEvent(QResizeEvent *e)
{
    ui_chainWidget->setGeometry(0,0,width(),5);
    QWidget::resizeEvent(e);
}

void Track::remakeChainWidget()
{
    ui_chainWidget->reset();
    int i=0;
    foreach(QWidget* ui,s_appUi_)
    {
        ui_chainWidget->setBack(ui->x()+ui->width(),s_ambition.at(i)->processingMidi(),s_ambition.at(i)->processingAudio());
        i++;
    }
}

void Track::clearUiPipeline()
{
    ui_mainLayout->removeWidget(ui_outputName);
    foreach(QWidget* ui,s_appUi_)
    {
        ui_mainLayout->removeWidget(ui);
    }
    ui_chainWidget->reset();
}

void Track::makeUiPipeline()
{
    foreach(QWidget* ui,s_appUi_)
    {
        ui_mainLayout->addWidget(ui);
        ui->show();
        ui->adjustSize();
    }
    adjustSize();
    ui_mainLayout->addWidget(ui_outputName);
    setGeometry(geometry());
    remakeChainWidget();
}

void Track::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasFormat("text/plain")&&app::appNames().contains(e->mimeData()->text()))
    {
        if(e->mimeData()->text()!="FILTER"||s_ambition.inputIsMidiObject())
        {
            e->acceptProposedAction();
        }
    }
    if(e->mimeData()->hasFormat("application/x-qt-windows-mime;value=\"FileName\""))
    {
        e->acceptProposedAction();
    }
}

void Track::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasFormat("application/x-qt-windows-mime;value=\"FileName\""))
    {
        live::ObjectPtr backend = app::newBackend("SEQUENCER");
        AppFrame* frontend = qobject_cast<AppFrame*>(app::newFrontend("SEQUENCER",backend));
        int x=e->pos().x();
        int pos=0;
        if(s_appUi_.size())
        {
            int lastHalfX=s_appUi_[0]->geometry().x()+0.5*s_appUi_[0]->geometry().width();
            for(int i=1;i<s_ambition.chainSize();i++)
            {
                int nextHalfX=s_appUi_[i]->geometry().x()+0.5*s_appUi_[i]->geometry().width();
                if(lastHalfX<=x&&x<=nextHalfX)
                {
                    pos=i;
                    break;
                }
                lastHalfX=nextHalfX;
                if(x>=lastHalfX)
                {
                    pos=i+1;
                }
            }
            if(s_appUi_.size()==1&&x>=lastHalfX)
            {
                pos=s_appUi_.size();
            }
        }
        addApp(pos,frontend,backend);

        QString file=e->mimeData()->data("application/x-qt-windows-mime;value=\"FileName\"");
        if(QFile::exists(file)&&(file.endsWith("flac",Qt::CaseInsensitive)||file.endsWith("ogg",Qt::CaseInsensitive)||file.endsWith("wav",Qt::CaseInsensitive)))
        {
            QMessageBox::warning(0,"Error","This functionality has not yet been reimplemented.","Complain to Josh!");
//            dynamic_cast<SequencerApp*>(backend.data())->s_audioTrack->importFile(file);
        }
        else if(QFile::exists(file)&&(file.endsWith("midi")||file.endsWith("MIDI")||file.endsWith("mid")||file.endsWith("MID")))
        {
            QMessageBox::warning(0,"Error","This functionality has not yet been reimplemented.","Complain to Josh!");
//            dynamic_cast<SequencerApp*>(backend.data())->s_midiTrack->importFile(file);
        }
        else
        {
            QMessageBox::critical(this,"Not supported","flac,ogg,wav,midi,MIDI,mid,and MID files are the only types of files currently supported.");
        }
    }
    else if(app::appNames().contains(e->mimeData()->text()))
    {
        live::ObjectPtr backend = app::newBackend(e->mimeData()->text());
        AppFrame* frontend = qobject_cast<AppFrame*>(app::newFrontend(e->mimeData()->text(),backend));
        int x=e->pos().x();
        int pos=0;
        if(s_appUi_.size())
        {
            int lastHalfX=s_appUi_[0]->geometry().x()+0.5*s_appUi_[0]->geometry().width();
            for(int i=1;i<s_ambition.chainSize();i++)
            {
                int nextHalfX=s_appUi_[i]->geometry().x()+0.5*s_appUi_[i]->geometry().width();
                if(lastHalfX<=x&&x<=nextHalfX)
                {
                    pos=i;
                    break;
                }
                lastHalfX=nextHalfX;
                if(x>=lastHalfX)
                {
                    pos=i+1;
                }
            }
            if(s_appUi_.size()==1&&x>=lastHalfX)
            {
                pos=s_appUi_.size();
            }
        }
        addApp(pos,frontend,backend);
    }
    else
    {
        qDebug()<<"Rejected "<<e->mimeData()->text()<<"(it is not a valid app name!!!)";
    }
}

void Track::setOutput(live::ObjectPtr output,live::ObjectPtr loopback)
{
    s_ambition.setOutput(output);
    s_ambition.setLoopbackOut(loopback);
}

void Track::setInput(live::ObjectPtr input)
{
    s_ambition.setInput(input);
}

void Track::addApp(int i,AppFrame* appUi,live::ObjectPtr app)
{
    if(s_th)
    {
        ui_mainLayout->removeWidget(s_th);
        s_th->deleteLater();
        s_th=0;
    }
    if(app.valid())
    {
        s_ambition.insert(i,app);
    }

    if(ui_spacerItem)
    {
        ui_mainLayout->removeItem(ui_spacerItem);
        delete ui_spacerItem;
        ui_spacerItem=0;
    }

    connect(appUi->_tbBack,SIGNAL(clicked()),this,SLOT(logic_appBack()));
    connect(appUi->_tbClose,SIGNAL(clicked()),this,SLOT(logic_appDel()));
    connect(appUi->_tbNext,SIGNAL(clicked()),this,SLOT(logic_appNext()));

    clearUiPipeline();
    appUi->setParent(this);
    s_appUi_.insert(i,appUi);
    bool ok=1;
    makeUiPipeline();
    for(int i=0;i<s_appUi_.size();i++)
    {
        s_appUi_[i]->adjustSize();
        if(s_appUi_[i]->maximumWidth()>1500)
        {
            ok=0;
            break;
        }
    }
    if(ok)
    {
        ui_mainLayout->insertSpacerItem(s_appUi_.size(),ui_spacerItem=new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));
    }
    adjustSize();
    update();
}

void Track::delApp(int i)
{
    //ONLY CALLED FROM logic_delApp. Do not actually delete app.

    s_ambition.removeFromChain(i);
//    if(!s_ambition.chainSize())
//    {
//        ui_mainLayout->addSpacerItem(ui_spacerItem=new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));
//    }

    clearUiPipeline();
    s_appUi_.takeAt(i);
    makeUiPipeline();


    if(ui_spacerItem)
    {
        ui_mainLayout->removeItem(ui_spacerItem);
        delete ui_spacerItem;
        ui_spacerItem=0;
    }
    bool ok=1;
    for(int i=0;i<s_appUi_.size();i++)
    {
        s_appUi_[i]->adjustSize();
        if(s_appUi_[i]->maximumWidth()>1500)
        {
            ok=0;
            break;
        }
    }
    if(ok)
    {
        ui_mainLayout->insertSpacerItem(s_appUi_.size(),ui_spacerItem=new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));
    }
}

void Track::outputSelection()
{

    if(s_ambition.inputIsAudioObject())
    {
        QStringList a;
        a<<object::get(AudioOnly|OutputOnly);
        bool ok;
        QString ix=QInputDialog::getItem(this,"Select an output","Output:",a,a.indexOf(s_ambition.b_output.ref()),0,&ok);
        int i=a.indexOf(ix);
        if(ok)
        {
            QList<live::ObjectPtr> x=object::get(AudioOnly|OutputOnly);
            if(i!=-1&&i<x.size()&&x[i]->name()==ix)
            {
                setOutput(x[i]);
            }
            else
            {
                QMessageBox::warning(this,"Oops.","Something went wrong in trying to set the output. Sorry about that. Maybe try again?");
            }
        }
    }
    else if(s_ambition.inputIsMidiObject())
    {
        QStringList a;
        a<<object::get(MidiOnly|OutputOnly|Instrument);
        QStringList out=a;
        bool ok;
        QString ix=QInputDialog::getItem(this,"Select an output","Output:",out,out.indexOf(s_ambition.b_output),0,&ok);
        int i=out.indexOf(ix);
        if(ok)
        {
            live::ObjectPtr out=object::get(MidiOnly|OutputOnly|Instrument|NoRefresh)[i];
            if(out->isAudioObject()&&out->isMidiObject())
            {
                //Now for output
                QStringList a;
                a<<object::get(AudioOnly|OutputOnly);
                bool ok;
                QString ix=QInputDialog::getItem(this,"Select an output","This device supports audio out. Select one:",a,a.indexOf(s_ambition.b_output),0,&ok);
                int i=a.indexOf(ix);
                if(ok)
                {
                    if(i!=-1&&i<object::get(AudioOnly|OutputOnly|NoRefresh).size()&&
                            object::get(AudioOnly|OutputOnly|NoRefresh)[i]->name()==ix)
                    {
                        setOutput(out,object::get(AudioOnly|OutputOnly|NoRefresh)[i]);
                    }
                    else
                    {
                        QMessageBox::warning(this,"Oops.","Something went wrong in trying to set the output. Sorry about that. Maybe try again?");
                    }
                }
            }
            else if(out->name()==a[i])
            {
                setOutput(out);
            }
            else
            {
                QMessageBox::warning(this,"Oops.","Something went wrong in trying to set the output. Sorry about that. Maybe try again?");
            }
        }
    }
    clearUiPipeline();
    makeUiPipeline();
}

void Track::logic_appBack()
{
    AppFrame*x=qobject_cast<AppFrame*>(sender()->parent());
    Q_ASSERT(x);
    for(int i=0;i<s_appUi_.size();i++)
    {
        if(x==s_appUi_[i])
        {
            if(!i)
            {
                return;
            }
            else
            {
                live::ObjectPtr oa=s_ambition.at(i);
                AppFrame* of=s_appUi_[i];
                delApp(i);
                addApp(i-1,of,oa);
            }
        }
    }
}

void Track::logic_appDel()
{
    AppFrame*x=qobject_cast<AppFrame*>(sender()->parent());
    Q_ASSERT(x);
    for(int i=0;i<s_appUi_.size();i++)
    {
        if(x==s_appUi_[i])
        {
            AppFrame* of=s_appUi_[i];
            delApp(i);
            of->deleteLater();  //will delete of
        }
    }
}

void Track::logic_appNext()
{
    AppFrame*x=qobject_cast<AppFrame*>(sender()->parent());
    Q_ASSERT(x);
    for(int i=0;i<s_appUi_.size();i++)
    {
        if(x==s_appUi_[i])
        {
            if(i+1==s_appUi_.size())
            {
                return;
            }
            else
            {
                live::ObjectPtr oa=s_ambition.at(i);
                AppFrame* of=s_appUi_[i];
                delApp(i);
                addApp(i+1,of,oa);
            }
        }
    }
}
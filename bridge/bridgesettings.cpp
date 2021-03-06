/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "bridgesettings.h"
#include "ui_bridgesettings.h"

#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

#include <live/object>
#include <live/midievent>
#include <live/midi>
#include "live/../audiosystem_p.h"
#include "live/../midisystem_p.h"

BridgeSettings::BridgeSettings(QWidget *parent)
  : QWidget(parent)
  , live::Object("Creator Live Bridge",0,0,2)
  , live_widgets::BindableParent(this)
  , ui(new Ui::BridgeSettings)
{

    live::audio::registerInterface(new live_private::SecretAudio);
    // FIXME:: should be null.
    live_private::SecretMidi::me=new live_private::SecretMidi;


    ui->setupUi(this);

    QList<QHostAddress> ha = QNetworkInterface::allAddresses();
    for (int i=0;i<ha.size();i++) {
        if (ha[i].toString().startsWith("127.0")) {
            ha.removeAt(i--);
            continue;
        }
    }
    QList<QNetworkInterface> ai = QNetworkInterface::allInterfaces();
    QStringList options;
    for (int i=0;i<ai.size();i++) {
        if (ai[i].flags()&QNetworkInterface::IsLoopBack) {
            ai.removeAt(i);
            i--;
            continue;
        }
        options.push_back(ai[i].name());
    }
    if (!ha.size()||!ai.size()) {
        QMessageBox::question(this,"Could not find the internet.","The internet seems to be missing. If you find it, start Bridge again.",QMessageBox::Ok);
        hide();
        QTimer::singleShot(0,qApp,SLOT(quit()));
        return;
    } else if (ha.size()>1) {
        bool ok;
        QString net=QInputDialog::getItem(this,"Choose a Network","What network is your PlayBook on?",options,0,0,&ok);
        if (!ok) {
            hide();
            QTimer::singleShot(0,qApp,SLOT(quit()));
            return;
        }
        int idx=options.indexOf(net);
        QList<QNetworkAddressEntry> nae = ai[idx].addressEntries();
        ha.clear();
        for (int i=0;i<nae.size();i++) ha.push_back(nae[i].ip());
        for (int i=0;i<ha.size();i++) {
            if (ha[i].toString().startsWith("127.0")) {
                ha.removeAt(i--);
                continue;
            }
        }
        if (!ha.size()||!ai.size()) {
            QMessageBox::question(this,"Could not find the internet.","The internet seems to be missing. If you find it, start Bridge again.",QMessageBox::Ok);
            hide();
            QTimer::singleShot(0,qApp,SLOT(quit()));
            return;
        }
        while (ha.size()>1) ha.removeLast();
    }

    QString code;

    QStringList x = ha[0].toString().split('.');
    for (int i=0;i<x.size();i++) {
        QString codepart=QString::number(x[i].toInt(0,10),16).toUpper();
        while (codepart.size()<=1) codepart="0"+codepart;
        code+=codepart;
    }
    qDebug() << "code:"<<code;
    QString labelText=ui->label_4->text();
    labelText.replace("RRDDZZ",code);
    ui->label_4->setText(labelText);

    QTcpServer* server = new QTcpServer(this);
    server->listen(QHostAddress::Any,1143);
    connect(server, SIGNAL(newConnection()), this, SLOT(startTalking()));
}

void BridgeSettings::startTalking()
{
    QTcpSocket* sock=dynamic_cast<QTcpServer*>(sender())->nextPendingConnection();
    if (!m_in.size()) {
        m_in=live::object::get(live::MidiOnly|live::InputOnly);
        for (int i=0;i<m_in.size();i++) {
            if (!m_in[i]) continue;
            m_connections.push_back(live::Connection(m_in[i], this, live::MidiConnection));
        }
    }
    QByteArray data;
    data+="VERSION 1000\n";
    data+="BEGIN HELLO_WORLD\n";
    data+="BEGIN MIDI_INPUT\n";

    for (int i=0;i<m_in.size();i++) {
        if (!m_in[i]) continue;
        data+=m_in[i]->name()+"\n";
    }
    data+="END MIDI_INPUT\n";
    if (!m_out.size()) m_out=live::object::get(live::MidiOnly|live::OutputOnly);
    data+="BEGIN MIDI_OUTPUT\n";
    for (int i=0;i<m_out.size();i++) {
        if (!m_out[i]) continue;
        data+=m_out[i]->name()+"\n";
    }
    data+="END MIDI_OUTPUT\n";
    data+="END HELLO_WORLD\n";
    sock->write(data);

    m_sockets.push_back(sock);

    connect(sock,SIGNAL(readyRead()),this,SLOT(listen()));
}

void BridgeSettings::mIn(const live::Event *ev, live::ObjectChain* p)
{
    QByteArray data;
    data+="BEGIN EVENT\n";
    data+="FROM "+p->first()->name()+"\n";
    data+="MSG "+QString::number(ev->message)+"\n";
    data+="DATA1 "+QString::number(ev->data1)+"\n";
    data+="DATA2 "+QString::number(ev->data2)+"\n";
    data+="DATA3 "+QString::number(live::midi::getTime_msec()-ev->time.toTime_ms())+"\n";
    data+="END EVENT\n";
    qDebug() << "->"<<data;
    for (int i=0;i<m_sockets.size();i++) {
        m_sockets[i]->write(data);
    }
}

void BridgeSettings::listen()
{
    Q_ASSERT(dynamic_cast<QTcpSocket*>(sender()));


    QList<QByteArray> commands = dynamic_cast<QTcpSocket*>(sender())->readAll().split('\n');
    while (commands.size()) {
        qDebug() << "Yo."<<commands;
        if (!commands.size()) return;
        if (commands.first()!="BEGIN EVENT") return;
        commands.pop_front();

        if (!commands.size()) return;
        if (!commands.first().startsWith("TO ")) return;
        QString from = commands.first();
        from.remove(0,3);
        live::ObjectPtr ptr;
        for (int i=0;i<m_out.size();i++) {
            if (m_out[i]->name()==from) ptr=m_out[i];
        }
        if (!ptr) {
            qDebug() << "No such device "<<from;
            return;
        }
        commands.pop_front();

        bool ok;
        live::Event ev;
        live::ObjectChain p;
        p.push_back(this);

        ///////////////////////////////////////////////////////////
        if (!commands.size()) return;
        if (!commands.first().startsWith("MSG ")) return;
        commands.first().remove(0,4);
        ev.message=commands.first().toInt(&ok);
        if (!ok) return;
        commands.pop_front();

        ///////////////////////////////////////////////////////////
        if (!commands.size()) return;
        if (!commands.first().startsWith("DATA1 ")) return;
        commands.first().remove(0,6);
        ev.data1=commands.first().toInt(&ok);
        if (!ok) return;
        commands.pop_front();

        ///////////////////////////////////////////////////////////
        if (!commands.size()) return;
        if (!commands.first().startsWith("DATA2 ")) return;
        commands.first().remove(0,6);
        ev.data2=commands.first().toInt(&ok);
        if (!ok) return;
        commands.pop_front();

        ///////////////////////////////////////////////////////////
        if (!commands.size()) return;
        if (!commands.first().startsWith("DATA3 ")) return;
        commands.first().remove(0,6);
        ev.time=live::Time(commands.first().toInt(&ok))+live::midi::getTime();
        if (!ok) return;
        commands.pop_front();

        if (!commands.size()) return;
        commands.pop_front();

        qDebug() << "!!"<<ev.time.toTime_ms() << "VS"<<live::midi::getTime_msec();

        ///////////////////////////////////////////////////////////
        live::Event* ev2=new live::Event(ev.message,ev.data1,ev.data2);
        ev2->time=ev.time;
//        ev2->time.sec=ev2->time.nsec=-1;
        ptr->mIn(ev2, &p);
        qDebug()<<ptr->name() << "...";
        live_private::SecretMidi::me->mWithhold(ev2,p,ptr); //now owns ev2
        p.pop_back();
        qDebug() << "SENT!";
    }
}

BridgeSettings::~BridgeSettings()
{
    delete ui;
}

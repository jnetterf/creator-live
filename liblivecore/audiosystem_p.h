/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef AUDIOSYSTEM_P_H
#define AUDIOSYSTEM_P_H

#include <QCoreApplication>
#include "live/object"
#include "live/audio"
#include "live/audiointerface"

#ifndef __QNX__

#include <jack/jack.h>

namespace live_private {

LIBLIVECORESHARED_EXPORT jack_client_t* getJackClient();

// ONLY CALL AUDIO FUNCTIONS FROM THE AUDIO THREAD!
class LIBLIVECORESHARED_EXPORT AudioIn : public live::Object
{
public:
    LIVE_AUDIO
    LIVE_INPUT
    static int lastDeviceInternalID;
    int     chans;
    QList<jack_port_t*> m_port_;
    QStringList m_realnames;
    bool m_map;
    bool m_suspend;

public:
    bool mOn() const{ return 0; } bool aOn() const { return 1; }
    AudioIn(QStringList cnames, QString cname, bool cmap) : live::Object(cname,true,false,2), chans(cnames.size()), m_port_(), m_realnames(cnames),m_map(cmap),m_suspend(0)
    {
        setTemporary(0);

        for(int i=0; i<m_realnames; i++)
        {
            QString& a=m_realnames[i];
            if (a.indexOf(':')==-1)
            {
                if(a.indexOf('\"')!=-1)
                {
                    qDebug() << "Live does not support audio clients with quotes.";
                    Q_ASSERT(a.indexOf('\"')==-1);
                }
                Q_ASSERT(a.indexOf('_')!=-1);
                a[a.indexOf('_',a.contains("vst")?5:0)]=':';
            }
        }
        m_name.replace(":", "_");

        init();
    }

    void init(); //Audio.cpp
    void proc();

    virtual void suspend() { m_suspend=1; }
    virtual void resume() { m_suspend=0; }
    virtual void aIn (const float*,int, live::Object*) {}
};

// ONLY CALL AUDIO FUNCTIONS FROM THE AUDIO THREAD!
class LIBLIVECORESHARED_EXPORT AudioOut : public live::Object
{
public:
    LIVE_AUDIO
    LIVE_OUTPUT
    bool mOn() const{ return 0; } bool aOn() const { return 1; }
    int     chans;
    QList<jack_port_t*> m_port_[32];    //No more than 32 ports

    int     m_i;
    QStringList m_realnames;
    bool    m_map;
    bool    m_processed;

public:
    AudioOut(QStringList cnames, QString name,bool cmap) : live::Object(name,true,true,2), chans(cnames.size()), m_i(-1), m_realnames(cnames), m_map(cmap), m_processed(0)
    {
        setTemporary(0);

        name.replace(":", "_");

        init();
    }
    virtual ~AudioOut() {
    }

    virtual void aIn(const float*data,int chan, live::Object*p); //Audio.cpp

    void init();
};

class LIBLIVECORESHARED_EXPORT AudioNull : public QObject, public live::Object
{
    Q_OBJECT
public:
    LIVE_AUDIO
    LIVE_INPUT
    bool mOn() const{ return 0; } bool aOn() const { return 1; }
    int     chans;
    QMutex p;

    AudioNull(int cchans)
      : live::Object("Null Audio Device",false,false,cchans)
      , chans(cchans)
      , p(QMutex::NonRecursive)
      {
    }

    virtual void aIn(const float*data,int chan, live::Object*)
    {
        aOut(data,chan,this);
    }

    QObject* qoThis() { return this; }
};

class LIBLIVECORESHARED_EXPORT SecretAudio : public QObject, public live::AudioInterface
{
    Q_OBJECT
    Q_INTERFACES(live::AudioInterface)
public:
    static int XRUNS;
    QString m_error;
    static SecretAudio* singleton;
    static QMutex x_sa;

    QList< jack_port_t*> m_availInPorts;
    QList< QString > m_availInPortIds;

    QList< jack_port_t*> m_availOutPorts;
    QList< QString > m_availOutPortIds;

    jack_port_t* getInputPort() {
        Q_ASSERT(m_availInPorts.size());
        if (!m_availInPorts.size())
            return 0;
        return m_availInPorts.takeFirst();
    }
    QString getInputPortId() {
        Q_ASSERT(m_availInPortIds.size());
        if (!m_availInPortIds.size())
            return "NULL";
        return m_availInPortIds.takeFirst();
    }
    jack_port_t* getOutputPort() {
        Q_ASSERT(m_availOutPorts.size());
        if (!m_availOutPorts.size())
            return 0;
        return m_availOutPorts.takeFirst();
    }
    QString getOutputPortId() {
        Q_ASSERT(m_availOutPortIds.size());
        if (!m_availOutPortIds.size())
            return "NULL";
        return m_availOutPortIds.takeFirst();
    }

    quint32 nframes;
    QList< AudioIn* > inputs;
    QList< AudioOut* > outputs;
    QList< AudioNull* > nulls;
    QList< QStringList > inputMappings;
    QStringList inputMappingsName;
    QList< QStringList > outputMappings;
    QStringList outputMappingsName;
    jack_client_t* client;
    QList< int > asioDeviceTypes;

    QObject* qobject() { return this; }

    SecretAudio();

    ~SecretAudio()
    {
        while (inputs.size())
        {
            delete inputs.takeFirst();
        }
        while (outputs.size())
        {
            delete outputs.takeFirst();
        }
        delClient();
    }

    static int jackCallback( jack_nframes_t nframes, void* )
    {
        if (SecretAudio::singleton->nframes != nframes)
            live::lthread::audioInit();

        SecretAudio::singleton->nframes=nframes;
        singleton->process();
        return 0;
    }

    static int xrunCallback( void * )
    {
        ++XRUNS;
        live::Object::XRUN();
        return 0;
    }
    static void QUITCALLBACK(jack_status_t code, const char* reason, void *)
    {
        std::cerr << "CODE: " << code <<", reason: "<<reason<< std::endl;
        TCRASH();
    }

    const quint32& nFrames() { return nframes; }
    qint32 sampleRate();

    bool valid() { return client; }

public slots:
    bool delClient();
    bool makeClient();
    bool refresh();
    void process();
    void jack_disconnect(QString readPort,QString writePort);

    void removeNull(QObject*);

public:
    virtual QString name() { return "Jack Audio"; }
    virtual QString description() { return "Jack is a multi-platform system for handling real-time low latency audio."; }

    virtual QObject* settingsWidget() { return 0; }

    virtual bool shouldDisplaySettingsWidget() { return 0; }

    virtual QString errorString() { QString l=m_error; m_error=""; return l; }

    virtual live::ObjectPtr getNull(int chans);

    virtual bool resetMappings();
    virtual bool addMapping(QStringList mapping, bool input,QString name);
    virtual int mappingCount(bool input);

    virtual QStringList getInputChanStringList();
    virtual QStringList getOutputChanStringList();

private:
    SecretAudio(const SecretAudio&);
    SecretAudio& operator=(const SecretAudio&);
};


class LIBLIVECORESHARED_EXPORT NullAudio : public QObject, public live::AudioInterface
{
    Q_OBJECT
    Q_INTERFACES(live::AudioInterface)
public:
    static int XRUNS;
    QString m_error;
    static NullAudio* singleton;
    static QMutex x_sa;

    quint32 nframes;
    QList< AudioNull* > inputs;
    QList< AudioNull* > outputs;
    QList< AudioNull* > nulls;

    QObject* qobject() { return this; }

    NullAudio();

    ~NullAudio()
    {
        while (inputs.size())
        {
            delete inputs.takeFirst();
        }
        while (outputs.size())
        {
            delete outputs.takeFirst();
        }
        delClient();
    }

    const quint32& nFrames() { return nframes = 256; }
    qint32 sampleRate() { return 44100; }

    bool valid() { return 1; }

public slots:
    bool delClient() {return 0;}
    bool makeClient() {return 0;}
    bool refresh() {return 0;}

public:
    virtual QString name() { return "Null Audio"; }
    virtual QString description() { return "For testing."; }

    virtual QObject* settingsWidget() { return 0; }

    virtual bool shouldDisplaySettingsWidget() { return 0; }

    virtual QString errorString() { QString l=m_error; m_error=""; return l; }

    virtual live::ObjectPtr getNull(int chans) { AudioNull* p = new AudioNull(chans); nulls.push_back(p); return p; }

    virtual bool resetMappings() {return 0;}
    virtual bool addMapping(QStringList mapping, bool input,QString name) {return 0;}
    virtual int mappingCount(bool input) {return 0;}

    virtual QStringList getInputChanStringList() { return QStringList("");}
    virtual QStringList getOutputChanStringList() { return QStringList("");}

private:
    NullAudio(const SecretAudio&);
    NullAudio& operator=(const SecretAudio&);
};

}
#else  // not __QNX__
#include "audiosystem_qnx_p.h"
#endif // __QNX__

#endif // AUDIOSYSTEM_P_H

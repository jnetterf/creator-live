#ifndef MIDIBINDING_H
#define MIDIBINDING_H

#include <QObject>
#include "live/object.h"
#include "live/midievent.h"
#include "live/midi.h"
#include "live/metronome.h"
#include "live/songsystem.h"

namespace live {

class LIBLIVECORESHARED_EXPORT QuantizedAction : public QObject
{
    Q_OBJECT
    int s_cb;
    int s_pulses;
    int s_division;
    QVariant s_data;
    bool s_ok;
public:
    QuantizedAction(QObject*reciever,const char*slot,int division=-1,QVariant data=QVariant()) :
        s_cb(-1), s_division(division), s_data(data),s_ok(1)
    {
        song::current->metronome->registerSync(this);
        if(s_data.isNull())
        {
            connect(this,SIGNAL(activated()),reciever,slot);
        }
        else switch (static_cast<QMetaType::Type>(s_data.type()))
        {
        case QMetaType::Bool:
            connect(this,SIGNAL(activated(bool)),reciever,slot);
            break;
        case QMetaType::Int:
            connect(this,SIGNAL(activated(int)),reciever,slot);
            break;
        case QMetaType::Double:
            connect(this,SIGNAL(activated(double)),reciever,slot);
            break;
        case QMetaType::Float:
            connect(this,SIGNAL(activated(float)),reciever,slot);
            break;
        case QMetaType::QString:
            connect(this,SIGNAL(activated(QString)),reciever,slot);
            break;
        default:
            qFatal("QMetaType %i is not supported",s_data.type());
            Q_ASSERT(0);
        }

        connect(reciever,SIGNAL(destroyed()),this,SLOT(deleteLater()),Qt::DirectConnection);    //dc to be safe!!
    }
    ~QuantizedAction()
    {
        song::current->metronome->unregisterSync(this);
    }

public slots:
    void sync(const SyncData&sd)
    {
        if(!s_ok)
        {
            return;
        }

        if(s_cb==-1)
        {
            s_cb=sd.beat;
            s_pulses=sd.pulses;
        }
        else if(sd.beat!=s_cb||(s_division!=-1&&sd.pulses>=s_pulses+(float)sd.ppb/(float)s_division))
        {
            emit activated();
            emit activated(s_data.toBool());
            emit activated(s_data.toInt());
            emit activated(s_data.toDouble());
            emit activated(s_data.toFloat());
            emit activated(s_data.toString());
            s_ok=0;
            deleteLater();
        }
    }

signals:
    void activated();
    void activated(bool);
    void activated(int);
    void activated(double);
    void activated(float);
    void activated(QString);
};

class LIBLIVECORESHARED_EXPORT MidiBinding : public QObject, public live::Object
{
    Q_OBJECT
public:
    static QMap<ObjectPtr,ObjectPtr*>* customKey;
    static ObjectPtr customNow;
    static QList<MidiBinding*> universe;

    enum BindingType
    {
        BindingNull = 1,
        BindingClick = 2,
        BindingToggle = 3,
        BindingSetCurrentIndex = 4,
        BindingSetText = 5,
        BindingStepDown = 6,
        BindingStepUp = 7,
        BindingSeqSetPos = 8,
        BindingSlider = 9
    };

    enum GuiType
    {
        GuiAbstractButton = 0,
        GuiComboBox = 1,
        GuiAbstractSpinBox = 2,
        GuiAbstractSlider = 3,
        GuiAction = 4,
        Sequencer = 5
    };

    void* guiObject;    //ne delete pas.
    GuiType parentType;
    BindingType type;
    QString data;
    ObjectPtr obj;
    int key;
    bool pending;
    int lastEventTime;
    bool controller;    //add this plz

    MidiBinding(void* cGuiObject,GuiType cparentType,BindingType ctype,QString cdata=QString(),int ckey=-1,ObjectPtr cobj=0,bool ccontroller=0) :
        Object("MIDI Binding",false,false),
        guiObject(cGuiObject),
        parentType(cparentType),
        type(ctype),
        data(cdata),
        obj(cobj),
        key(ckey),
        lastEventTime(-1),
        controller(ccontroller)
    {
        pending=ckey==-1;
        QList<ObjectPtr> midiIn=object::get(live::MidiOnly|live::InputOnly|live::NoRefresh);
        for(int i=0;i<midiIn;i++)
        {
            midiIn[i]->hybridConnect(this);
        }
    }

protected:
    virtual void doAction(int vel=0) = 0;

public slots:
    void mIn(const Event *data, ObjectChain&p)
    {
        if(midi::getTime_msec()-lastEventTime<100)
        {
            return;
        }
        if ( pending ) {
            if((data->simpleStatus()==Event::NOTE_ON&&data->velocity())||data->simpleStatus()==0xB0)
            {
                obj=p.first();
                key=data->note();
                pending=0;
                lastEventTime=midi::getTime_msec();
                controller=data->simpleStatus()==0xB0;
            }

        }
        else if( data->note()==key&&(controller==(data->simpleStatus()==0xB0)||(data->simpleStatus()==Event::NOTE_ON&&data->velocity())) && p.first()==obj.data() )
        {
            if(controller&&type!=BindingSlider&&(data->velocity()!=127))
            {
                return;
            }
            lastEventTime=midi::getTime_msec();
            doAction(controller?data->velocity():0);
        }
    }
};

class LIBLIVECORESHARED_EXPORT MidiBindingFilter : public QObject, public live::Object
{
    //Passes through notes which are not bound
    Q_OBJECT
public:
    LIVE_HYBRID
    LIVE_EFFECT
    bool allowAudio;
    bool aOn() const { return 0; }
    bool mOn() const{ return 1; }
    MidiBindingFilter() : Object("Midi Binding Filter",false,false), allowAudio(1)
    {

    }
public slots:
    void aIn(const float *data, int chan, ObjectChain&p)
    {
        if(allowAudio)
        {
            p.push_back(this);
            aOut(data,chan,p);
            p.pop_back();
        }
    }
    void mIn(const Event *data, ObjectChain&p)
    {
        if(data->simpleStatus()==Event::NOTE_ON||data->simpleStatus()==Event::NOTE_OFF)
        {
            if(!live::MidiBinding::customKey->value(p.first(),0)||live::MidiBinding::customKey->value(p.first(),0)[data->note()].valid())
            {
                return;
            }
            for(int i=0;i<live::MidiBinding::universe;i++)
            {
                if(!live::MidiBinding::universe[i]->controller&&live::MidiBinding::universe[i]->key==data->note()&&live::MidiBinding::universe[i]->obj==p.first())
                {
                    return;
                }
            }
        }
        if(data->simpleStatus()==0xB0)
        {
            if(!live::MidiBinding::customKey->value(p.first(),0)||live::MidiBinding::customKey->value(p.first(),0)[data->note()].valid())
            {
                return;
            }
            for(int i=0;i<live::MidiBinding::universe;i++)
            {
                if(live::MidiBinding::universe[i]->controller&&live::MidiBinding::universe[i]->key==data->note()&&live::MidiBinding::universe[i]->obj==p.first())
                {
                    return;
                }
            }
        }
        p.push_back(this);
        mOut(data,p);
        p.pop_back();
    }
};

class LIBLIVECORESHARED_EXPORT bindings : public QObject {
    Q_OBJECT
    static bindings* singleton;
public:
    static bindings* me() { if(!singleton) singleton=new bindings; return singleton; }
public slots:
    void showBindings(bool ean) { emit showBindingsChanged(ean); }
signals:
    void showBindingsChanged(bool);
};

}

#endif // MIDIBINDING_H
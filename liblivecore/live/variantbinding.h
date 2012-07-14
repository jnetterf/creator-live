/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#ifndef VARIANTBINDING_H
#define VARIANTBINDING_H

#include <QVariant>
#include <QDebug>

#include "liblivecore_global.h"

namespace live {

// Warnings:
//  - these objects are slow by nature...
//  - pass references, they're objects!
//  - T must be supported by QVariant

class LIBLIVECORESHARED_EXPORT BoundBase : public QObject
{
    Q_OBJECT
public slots:
    virtual void set(const QVariant&) = 0;
    virtual void set(const int&) = 0;
    virtual void set(const bool&) = 0;
signals:
    void changeObserved();
    void changeObserved(const QVariant&now,const QVariant&before);
    void changeObserved(QString now,QString before);
    void changeObserved(int now,int before);
    void changeObserved(bool now,bool before);
};

template<class U> class LIBLIVECORESHARED_EXPORT Bound : public BoundBase
{
private:
    U&  s_t;
public:
    // Hmm... this could be weird...
    explicit Bound( U&ct = *(new U) ) :
        s_t(ct)
    {

    }

    explicit Bound( const U&ct ) :
        s_t(*(new U))
    {
        s_t=ct;
    }

    virtual ~Bound()
    {
    }

    operator const U&() const
    {
        return s_t;
    }

    operator U&()
    {
        return s_t;
    }

    const U& ref() const //fast
    {
        return s_t;
    }

    U& ref() //fast
    {
        return s_t;
    }

    void operator=(const U&that)    // slow
    {
        QVariant a(s_t),b(that);
        s_t=that;
        if(a==that) return;
        emit changeObserved(b,a);
        emit changeObserved(b.toString(),a.toString());
        emit changeObserved(b.toInt(),a.toInt());
        emit changeObserved(b.toBool(),a.toBool());    }

    void operator=(Bound<U>&that)    // slow
    {
        QVariant a(s_t),b(that.ref());
        s_t=that;

        if(a==b) return;
        emit changeObserved(b,a);
        emit changeObserved(b.toString(),a.toString());
        emit changeObserved(b.toInt(),a.toInt());
        emit changeObserved(b.toBool(),a.toBool());    }

    void operator==(const U&that)   // fast
    {
        return(s_t==that);
    }

    void set(const QVariant& that)
    {
        QVariant a(s_t);
        s_t=qvariant_cast<U>(that);

        if(a==that) return;
        emit changeObserved(that,a);
        emit changeObserved(that.toString(),a.toString());
        emit changeObserved(that.toInt(),a.toInt());
        emit changeObserved(that.toBool(),a.toBool());
    }
    void set(const int& that)
    {
        set((QVariant)that);
    }
    void set(const bool& that)
    {
        set((QVariant)that);
    }

private: //EVIL, EVIL COPY CONSURUCUORS!!! grr...
    Bound(const Bound&);
};

}

#endif // VARIANTBINDING_H
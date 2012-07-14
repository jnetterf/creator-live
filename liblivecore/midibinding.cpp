/*******************************************************

    Part of the Creator Live Music Production Suite.
Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012

                  All rights reserved.

*******************************************************/

#include "live/midibinding.h"

LIBLIVECORESHARED_EXPORT QMap<live::ObjectPtr,live::ObjectPtr*>* live::MidiBinding::customKey= new QMap<live::ObjectPtr,live::ObjectPtr*>();
LIBLIVECORESHARED_EXPORT live::ObjectPtr live::MidiBinding::customNow;

LIBLIVECORESHARED_EXPORT QList<live::MidiBinding*> live::MidiBinding::universe=QList<MidiBinding*>();
LIBLIVECORESHARED_EXPORT live::bindings* live::bindings::singleton;
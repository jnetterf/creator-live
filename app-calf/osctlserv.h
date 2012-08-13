/* Calf DSP Library
 * Open Sound Control UDP server support
 *
 * Copyright (C) 2007-2009 Krzysztof Foltman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __CALF_OSCTLSERV_H
#define __CALF_OSCTLSERV_H

#include <glib.h>
#include "osctlnet.h"

namespace osctl
{
    
struct osc_server: public osc_socket
{
    GIOChannel *ioch;
    osc_message_dump<osc_strstream, std::ostream> dump;
    osc_message_sink<osc_strstream> *sink;
    
    osc_server() : ioch(NULL), dump(std::cout), sink(&dump) {}
    
    virtual void on_bind();
    
    static gboolean on_data(GIOChannel *channel, GIOCondition cond, void *obj);
    void parse_message(const char *buffer, int len);    
    ~osc_server();
};

};

#endif

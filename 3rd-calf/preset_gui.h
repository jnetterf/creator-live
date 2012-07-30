/* Calf DSP Library
 * GUI functions for preset management.
 * Copyright (C) 2007 Krzysztof Foltman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
#ifndef __CALF_PRESET_GUI_H
#define __CALF_PRESET_GUI_H


#include <3rd-calf/giface.h>
#include <3rd-calf/gui.h>

namespace calf_plugins {

struct activate_preset_params
{
    plugin_gui *gui;
    int preset;
    bool builtin;
    
    activate_preset_params(plugin_gui *_gui, int _preset, bool _builtin)
    : gui(_gui), preset(_preset), builtin(_builtin)
    {
    }
};

void store_preset(GtkWindow *toplevel, plugin_gui *gui);
void activate_preset(GtkAction *action, activate_preset_params *params);

};

#endif

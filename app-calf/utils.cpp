/* Calf DSP Library
 * Various utility functions.
 * Copyright (C) 2007 Krzysztof Foltman
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
#include <stdio.h>
#include <assert.h>
#include <app-calf/osctl.h>
#include <app-calf/utils.h>
#include <sstream>

using namespace std;
using namespace osctl;

namespace calf_utils {

string encode_map(const dictionary &data)
{
    osctl::string_buffer sb;
    osc_stream<osctl::string_buffer> str(sb);
    str << (uint32_t)data.size();
    for (dictionary::const_iterator i = data.begin(); i != data.end(); i++)
    {
        str << i->first << i->second;
    }
    return sb.data;
}

void decode_map(dictionary &data, const string &src)
{
    osctl::string_buffer sb(src);
    osc_stream<osctl::string_buffer> str(sb);
    uint32_t count = 0;
    str >> count;
    string tmp, tmp2;
    data.clear();
    for (uint32_t i = 0; i < count; i++)
    {
        str >> tmp;
        str >> tmp2;
        data[tmp] = tmp2;
    }
}

std::string xml_escape(const std::string &src)
{
    string dest;
    for (size_t i = 0; i < src.length(); i++) {
        // XXXKF take care of string encoding
        if (src[i] < 0 || src[i] == '"' || src[i] == '<' || src[i] == '>' || src[i] == '&')
            dest += "&"+i2s((uint8_t)src[i])+";";
        else
            dest += src[i];
    }
    return dest;
}

std::string load_file(const std::string &src)
{
    std::string str;
    FILE *f = fopen(src.c_str(), "rb");
    if (!f)
        throw file_exception(src);
    while (!feof(f))
    {
        char buffer[1024];
        int len = fread(buffer, 1, sizeof(buffer), f);
        if (len < 0)
            throw file_exception(src);
        str += string(buffer, len);
    }
    return str;
}

std::string i2s(int value)
{
    char buf[32];
    sprintf(buf, "%d", value);
    
    return std::string(buf);
}

std::string f2s(double value)
{
    // XXXKF might not work with some locale settings
    stringstream ss;
    ss << value;
    return ss.str();
}

std::string indent(const std::string &src, const std::string &indent)
{
    std::string dest;
    size_t pos = 0;
    do {
        size_t epos = src.find("\n", pos);
        if (epos == string::npos)
            break;
        dest += indent + src.substr(pos, epos - pos) + "\n";
        pos = epos + 1;
    } while (pos < src.length());
    if (pos < src.length())
        dest += indent + src.substr(pos);
    return dest;
}

}

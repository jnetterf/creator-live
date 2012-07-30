/* Calf DSP Library
 * Example audio modules - parameters and LADSPA wrapper instantiation
 *
 * Copyright (C) 2001-2008 Krzysztof Foltman
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
#include <assert.h>
#include <memory.h>
#if USE_JACK
#include <jack/jack.h>
#endif
#include <3rd-calf/giface.h>
#include <3rd-calf/metadata.h>
#include <3rd-calf/audio_fx.h>

using namespace dsp;
using namespace calf_plugins;

const char *calf_plugins::calf_copyright_info = "(C) 2001-2008 Krzysztof Foltman, license: LGPL";

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(flanger) = {"In L", "In R", "Out L", "Out R"};

CALF_PORT_PROPS(flanger) = {
    { 0.1,      0.1, 10,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC | PF_PROP_GRAPH, NULL, "min_delay", "Minimum delay" },
    { 0.5,      0.1, 10,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "mod_depth", "Modulation depth" },
    { 0.25,    0.01, 20,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "mod_rate", "Modulation rate" },
    { 0.90,   -0.99, 0.99,  0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "feedback", "Feedback" },
    { 0,          0, 360,   9, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "stereo", "Stereo phase" },
    { 0,          0, 1,     2, PF_BOOL | PF_CTL_BUTTON , NULL, "reset", "Reset" },
    { 1,          0, 4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "amount", "Amount" },
    { 1.0,        0, 4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "dry", "Dry Amount" },
};

CALF_PLUGIN_INFO(flanger) = { 0x847d, "Flanger", "Calf Flanger", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "FlangerPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(phaser) = {"In L", "In R", "Out L", "Out R"};

CALF_PORT_PROPS(phaser) = {
    { 1000,      20, 20000, 0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ | PF_PROP_GRAPH, NULL, "base_freq", "Center Freq" },
    { 4000,       0, 10800,  0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "mod_depth", "Modulation depth" },
    { 0.25,    0.01, 20,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "mod_rate", "Modulation rate" },
    { 0.25,   -0.99, 0.99,  0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "feedback", "Feedback" },
    { 6,          1, 12,   12, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "stages", "# Stages" },
    { 180,        0, 360,   9, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "stereo", "Stereo phase" },
    { 0,          0, 1,     2, PF_BOOL | PF_CTL_BUTTON , NULL, "reset", "Reset" },
    { 1,          0, 4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "amount", "Amount" },
    { 1.0,        0, 4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "dry", "Dry Amount" },
};

CALF_PLUGIN_INFO(phaser) = { 0x8484, "Phaser", "Calf Phaser", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "PhaserPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(reverb) = {"In L", "In R", "Out L", "Out R"};

const char *reverb_room_sizes[] = { "Small", "Medium", "Large", "Tunnel-like", "Large/smooth", "Experimental" };

CALF_PORT_PROPS(reverb) = {
    { 1.5,      0.4, 15.0,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_SEC, NULL, "decay_time", "Decay time" },
    { 5000,    2000,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "hf_damp", "High Frq Damp" },
    { 2,          0,    5,    0, PF_ENUM | PF_CTL_COMBO , reverb_room_sizes, "room_size", "Room size", },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "diffusion", "Diffusion" },
    { 0.25,       0,    2,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "amount", "Wet Amount" },
    { 1.0,        0,    2,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "dry", "Dry Amount" },
    { 0,          0,   50,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "predelay", "Pre Delay" },
    { 300,       20, 20000, 0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "bass_cut", "Bass Cut" },
    { 5000,      20, 20000, 0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "treble_cut", "Treble Cut" },
};

CALF_PLUGIN_INFO(reverb) = { 0x847e, "Reverb", "Calf Reverb", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "ReverbPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(filter) = {"In L", "In R", "Out L", "Out R"};

const char *filter_choices[] = {
    "12dB/oct Lowpass",
    "24dB/oct Lowpass",
    "36dB/oct Lowpass",
    "12dB/oct Highpass",
    "24dB/oct Highpass",
    "36dB/oct Highpass",
    "6dB/oct Bandpass",
    "12dB/oct Bandpass",
    "18dB/oct Bandpass",
    "6dB/oct Bandreject",
    "12dB/oct Bandreject",
    "18dB/oct Bandreject",
};

CALF_PORT_PROPS(filter) = {
    { 2000,      10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ | PF_PROP_GRAPH, NULL, "freq", "Frequency" },
    { 0.707,  0.707,   32,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "res", "Resonance" },
    { biquad_filter_module::mode_12db_lp,
      biquad_filter_module::mode_12db_lp,
      biquad_filter_module::mode_count - 1,
                                1,  PF_ENUM | PF_CTL_COMBO, filter_choices, "mode", "Mode" },
    { 20,         5,  100,    20, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "inertia", "Inertia"},
};

CALF_PLUGIN_INFO(filter) = { 0x847f, "Filter", "Calf Filter", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "FilterPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(filterclavier) = {"In L", "In R", "Out L", "Out R"};

CALF_PORT_PROPS(filterclavier) = {
    { 0,        -48,   48, 48*2+1, PF_INT   | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_SEMITONES, NULL, "transpose", "Transpose" },
    { 0,       -100,  100,      0, PF_INT   | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune", "Detune" },
    { 32,     0.707,   32,      0, PF_FLOAT | PF_SCALE_GAIN   | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "maxres", "Max. Resonance" },    
    { biquad_filter_module::mode_6db_bp, 
      biquad_filter_module::mode_12db_lp,
      biquad_filter_module::mode_count - 1,
                                1, PF_ENUM  | PF_CTL_COMBO | PF_PROP_GRAPH, filter_choices, "mode", "Mode" },
    { 20,         1,  2000,    20, PF_FLOAT | PF_SCALE_LOG    | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "inertia", "Portamento time"}
};

CALF_PLUGIN_INFO(filterclavier) = { 0x849f, "Filterclavier", "Calf Filterclavier", "Krzysztof Foltman / Hans Baier", calf_plugins::calf_copyright_info, "FilterclavierPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(vintage_delay) = {"In L", "In R", "Out L", "Out R"};

const char *vintage_delay_mixmodes[] = {
    "Stereo",
    "Ping-Pong",
};

const char *vintage_delay_fbmodes[] = {
    "Plain",
    "Tape",
    "Old Tape",
};

CALF_PORT_PROPS(vintage_delay) = {
    { 120,      30,    300,2701, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_BPM, NULL, "bpm", "Tempo" },
    {  4,        1,    16,    1, PF_INT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "subdiv", "Subdivide"},
    {  3,        1,    16,    1, PF_INT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "time_l", "Time L"},
    {  5,        1,    16,    1, PF_INT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "time_r", "Time R"},
    { 0.5,       0,    1,     0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "feedback", "Feedback" },
    { 0.25,      0,    4,   100, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "amount", "Amount" },
    { 1,         0,    1,     0, PF_ENUM | PF_CTL_COMBO, vintage_delay_mixmodes, "mix_mode", "Mix mode" },
    { 1,         0,    2,     0, PF_ENUM | PF_CTL_COMBO, vintage_delay_fbmodes, "medium", "Medium" },
    { 1.0,       0,    4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "dry", "Dry Amount" },
};

CALF_PLUGIN_INFO(vintage_delay) = { 0x8482, "VintageDelay", "Calf Vintage Delay", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "DelayPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(rotary_speaker) = {"In L", "In R", "Out L", "Out R"};

const char *rotary_speaker_speed_names[] = { "Off", "Chorale", "Tremolo", "HoldPedal", "ModWheel", "Manual" };

CALF_PORT_PROPS(rotary_speaker) = {
    { 2,         0,  5, 1.01, PF_ENUM | PF_CTL_COMBO, rotary_speaker_speed_names, "vib_speed", "Speed Mode" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "spacing", "Tap Spacing" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "shift", "Tap Offset" },
    { 0.10,       0,    1,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "mod_depth", "Mod Depth" },
    { 390,       10,   600,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_LOG | PF_UNIT_RPM, NULL, "treble_speed", "Treble Motor" },
    { 410,      10,   600,    0, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_LOG | PF_UNIT_RPM, NULL, "bass_speed", "Bass Motor" },
    { 0.7,        0,    1,  101, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "mic_distance", "Mic Distance" },
    { 0.3,        0,    1,  101, PF_FLOAT | PF_CTL_KNOB | PF_SCALE_PERC, NULL, "reflection", "Reflection" },
};

CALF_PLUGIN_INFO(rotary_speaker) = { 0x8483, "RotarySpeaker", "Calf Rotary Speaker", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "SimulatorPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(multichorus) = {"In L", "In R", "Out L", "Out R"};

CALF_PORT_PROPS(multichorus) = {
    { 5,        0.1,  10,   0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC | PF_PROP_GRAPH, NULL, "min_delay", "Minimum delay" },
    { 6,        0.1,  10,   0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC| PF_PROP_GRAPH, NULL, "mod_depth", "Modulation depth" },
    { 0.5,     0.01,  20,   0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ| PF_PROP_GRAPH, NULL, "mod_rate", "Modulation rate" },
    { 180,        0, 360,  91, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "stereo", "Stereo phase" },
    { 4,          1,   8,   8, PF_INT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "voices", "Voices"},
    { 64,         0, 360,  91, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "vphase", "Inter-voice phase" },
    { 2,          0,   4,   0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "amount", "Amount" },
    { 1.0,        0,   4,     0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_COEF | PF_PROP_NOBOUNDS, NULL, "dry", "Dry Amount" },
    { 100,       10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ | PF_PROP_GRAPH, NULL, "freq", "Center Frq 1" },
    { 5000,      10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ | PF_PROP_GRAPH, NULL, "freq2", "Center Frq 2" },
    { 0.125,  0.125,   8,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "q", "Q" },
};

CALF_PLUGIN_INFO(multichorus) = { 0x8501, "MultiChorus", "Calf MultiChorus", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "ChorusPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(compressor) = {"In L", "In R", "Out L", "Out R"};

const char *compressor_detection_names[] = { "RMS", "Peak" };
const char *compressor_stereo_link_names[] = { "Average", "Maximum" };

CALF_PORT_PROPS(compressor) = {
    { 0.125,      0.000976563, 1,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_DB, NULL, "threshold", "Threshold" },
    { 2,      1, 20,  21, PF_FLOAT | PF_SCALE_LOG_INF | PF_CTL_KNOB | PF_UNIT_COEF, NULL, "ratio", "Ratio" },
    { 20,     0.01, 2000, 0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "attack", "Attack" },
    { 250,    0.01, 2000, 0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "release", "Release" },
    { 2,      1, 64,   0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_DB, NULL, "makeup", "Makeup Gain" },
    { 2.828427125,      1,  8,   0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_UNIT_DB, NULL, "knee", "Knee" },
    { 0,      0,  1,   0, PF_ENUM | PF_CTL_COMBO, compressor_detection_names, "detection", "Detection" },
    { 0,      0,  1,   0, PF_ENUM | PF_CTL_COMBO, compressor_stereo_link_names, "stereo_link", "Stereo Link" },
    { 0,      0,  1,   0, PF_BOOL | PF_CTL_TOGGLE, NULL, "aweighting", "A-weighting" },
    { 0, 0.03125, 1,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_METER | PF_CTLO_LABEL | PF_CTLO_REVERSE | PF_UNIT_DB | PF_PROP_OUTPUT | PF_PROP_OPTIONAL| PF_PROP_GRAPH, NULL, "compression", "Compression" },
    { 0,      0,  1,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_METER | PF_CTLO_LABEL | PF_UNIT_DB | PF_PROP_OUTPUT | PF_PROP_OPTIONAL, NULL, "peak", "Peak Output" },
    { 0,      0,  1,    0, PF_BOOL | PF_CTL_LED | PF_PROP_OUTPUT | PF_PROP_OPTIONAL, NULL, "clip", "0dB" },
    { 0,      0,  1,    0, PF_BOOL | PF_CTL_TOGGLE, NULL, "bypass", "Bypass" }
};

CALF_PLUGIN_INFO(compressor) = { 0x8502, "Compressor", "Calf Compressor", "Thor Harald Johansen", calf_plugins::calf_copyright_info, "CompressorPlugin" };

////////////////////////////////////////////////////////////////////////////

CALF_PORT_NAMES(monosynth) = {
    "Out L", "Out R", 
};

const char *monosynth_waveform_names[] = { "Sawtooth", "Square", "Pulse", "Sine", "Triangle", "Varistep", "Skewed Saw", "Skewed Square", 
    "Smooth Brass", "Bass", "Dark FM", "Multiwave", "Bell FM", "Dark Pad", "DCO Saw", "DCO Maze" };
const char *monosynth_mode_names[] = { "0 : 0", "0 : 180", "0 : 90", "90 : 90", "90 : 270", "Random" };
const char *monosynth_legato_names[] = { "Retrig", "Legato", "Fng Retrig", "Fng Legato" };

const char *monosynth_filter_choices[] = {
    "12dB/oct Lowpass",
    "24dB/oct Lowpass",
    "2x12dB/oct Lowpass",
    "12dB/oct Highpass",
    "Lowpass+Notch",
    "Highpass+Notch",
    "6dB/oct Bandpass",
    "2x6dB/oct Bandpass",
};

CALF_PLUGIN_INFO(monosynth) = { 0x8480, "Monosynth", "Calf Monosynth", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "SynthesizerPlugin" };

CALF_PORT_PROPS(monosynth) = {
    { monosynth_metadata::wave_saw,         0, monosynth_metadata::wave_count - 1, 1, PF_ENUM | PF_CTL_COMBO | PF_PROP_GRAPH, monosynth_waveform_names, "o1_wave", "Osc1 Wave" },
    { monosynth_metadata::wave_sqr,         0, monosynth_metadata::wave_count - 1, 1, PF_ENUM | PF_CTL_COMBO | PF_PROP_GRAPH, monosynth_waveform_names, "o2_wave", "Osc2 Wave" },
    { 10,         0,  100,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "o12_detune", "O1<>2 Detune" },
    { 12,       -24,   24,    0, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_SEMITONES, NULL, "o2_xpose", "Osc 2 transpose" },
    { 0,          0,    5,    0, PF_ENUM | PF_CTL_COMBO, monosynth_mode_names, "phase_mode", "Phase mode" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "o12_mix", "O1<>2 Mix" },
    { 1,          0,    7,    0, PF_ENUM | PF_CTL_COMBO | PF_PROP_GRAPH, monosynth_filter_choices, "filter", "Filter" },
    { 33,        10,16000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "cutoff", "Cutoff" },
    { 2,        0.7,    8,    0, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB, NULL, "res", "Resonance" },
    { 0,      -2400, 2400,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "filter_sep", "Separation" },
    { 8000,  -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "env2cutoff", "Env->Cutoff" },
    { 1,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "env2res", "Env->Res" },
    { 1,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "env2amp", "Env->Amp" },
    
    { 1,          1,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_a", "Attack" },
    { 350,       10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_d", "Decay" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr_s", "Sustain" },
    { 50,       10,20000,     0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_r", "Release" },
    
    { 0,          0,    2,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "key_follow", "Key Follow" },
    { 0,          0,    3,    0, PF_ENUM | PF_CTL_COMBO, monosynth_legato_names, "legato", "Legato Mode" },
    { 1,          1, 2000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "portamento", "Portamento" },
    
    { 0.5,        0,    1,  0.1, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "vel2filter", "Vel->Filter" },
    { 0,          0,    1,  0.1, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "vel2amp", "Vel->Amp" },

    { 0.5,         0,   1, 100, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_PROP_OUTPUT_GAIN, NULL, "master", "Volume" },
    { 200,         0, 2400,   25, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "pbend_range", "PBend Range" },
};

////////////////////////////////////////////////////////////////////////////

CALF_PLUGIN_INFO(organ) = { 0x8481, "Organ", "Calf Organ", "Krzysztof Foltman", calf_plugins::calf_copyright_info, "SynthesizerPlugin" };

const char **organ_metadata::get_default_configure_vars()
{
    static const char *data[] = { "map_curve", "2\n0 1\n1 1\n", NULL };
    return data;
}

plugin_command_info *organ_metadata::get_commands()
{
    static plugin_command_info cmds[] = {
        { "cmd_panic", "Panic!", "Stop all sounds and reset all controllers" },
        { NULL }
    };
    return cmds;
}

CALF_PORT_NAMES(organ) = {"Out L", "Out R"};

const char *organ_percussion_trigger_names[] = { "First note", "Each note", "Each, no retrig", "Polyphonic" };

const char *organ_wave_names[] = { 
    "Sin", 
    "S0", "S00", "S000", 
    "SSaw", "SSqr", "SPls", 
    "Saw", "Sqr", "Pls", 
    "S(", "Sq(", "S+", "Clvg", 
    "Bell", "Bell2", 
    "W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8", "W9",
    "DSaw", "DSqr", "DPls",
    "P:SynStr","P:WideStr","P:Sine","P:Bell","P:Space","P:Voice","P:Hiss","P:Chant",
};

const char *organ_routing_names[] = { "Out", "Flt 1", "Flt 2"  };

const char *organ_ampctl_names[] = { "None", "Direct", "Flt 1", "Flt 2", "All"  };

const char *organ_vibrato_mode_names[] = { "None", "Direct", "Flt 1", "Flt 2", "Voice", "Global"  };

const char *organ_init_map_curve = "2\n0 1\n1 1\n";

CALF_PORT_PROPS(organ) = {
    { 8,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l1", "16'" },
    { 8,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l2", "5 1/3'" },
    { 8,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l3", "8'" },
    { 0,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l4", "4'" },
    { 0,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l5", "2 2/3'" },
    { 0,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l6", "2'" },
    { 0,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l7", "1 3/5'" },
    { 0,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l8", "1 1/3'" },
    { 8,       0,  8, 80, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_FADER, NULL, "l9", "1'" },

    { 1,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f1", "Freq 1" },
    { 3,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f2", "Freq 2" },
    { 2,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f3", "Freq 3" },
    { 4,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f4", "Freq 4" },
    { 6,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f5", "Freq 5" },
    { 8,       1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f6", "Freq 6" },
    { 10,      1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f7", "Freq 7" },
    { 12,      1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f8", "Freq 8" },
    { 16,      1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "f9", "Freq 9" },

    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w1", "Wave 1" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w2", "Wave 2" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w3", "Wave 3" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w4", "Wave 4" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w5", "Wave 5" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w6", "Wave 6" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w7", "Wave 7" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w8", "Wave 8" },
    { 0,       0,  organ_enums::wave_count - 1, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_wave_names, "w9", "Wave 9" },

    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune1", "Detune 1" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune2", "Detune 2" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune3", "Detune 3" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune4", "Detune 4" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune5", "Detune 5" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune6", "Detune 6" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune7", "Detune 7" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune8", "Detune 8" },
    { 0,    -100,100, 401, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune9", "Detune 9" },

    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase1", "Phase 1" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase2", "Phase 2" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase3", "Phase 3" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase4", "Phase 4" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase5", "Phase 5" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase6", "Phase 6" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase7", "Phase 7" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase8", "Phase 8" },
    { 0,       0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "phase9", "Phase 9" },

    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan1", "Pan 1" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan2", "Pan 2" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan3", "Pan 3" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan4", "Pan 4" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan5", "Pan 5" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan6", "Pan 6" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan7", "Pan 7" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan8", "Pan 8" },
    { 0,      -1,  1, 201, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB, NULL, "pan9", "Pan 9" },

    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing1", "Routing 1" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing2", "Routing 2" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing3", "Routing 3" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing4", "Routing 4" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing5", "Routing 5" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing6", "Routing 6" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing7", "Routing 7" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing8", "Routing 8" },
    { 0,       0,  2, 0, PF_ENUM | PF_SCALE_LINEAR | PF_CTL_COMBO, organ_routing_names, "routing9", "Routing 9" },

    { 96,      0,  127, 128, PF_INT | PF_CTL_KNOB | PF_UNIT_NOTE, NULL, "foldnote", "Foldover" },
    
    { 200,         10,  3000, 100, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "perc_decay", "P: Carrier Decay" },
    { 0.25,      0,  1, 100, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB, NULL, "perc_level", "P: Level" },
    { 0,         0,  organ_enums::wave_count_small - 1, 1, PF_ENUM | PF_CTL_COMBO, organ_wave_names, "perc_waveform", "P: Carrier Wave" },
    { 6,      1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "perc_harmonic", "P: Carrier Frq" },
    { 0,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "perc_vel2amp", "P: Vel->Amp" },
    
    { 200,         10,  3000, 100, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "perc_fm_decay", "P: Modulator Decay" },
    { 0,          0,    4,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "perc_fm_depth", "P: FM Depth" },
    { 0,         0,  organ_enums::wave_count_small - 1, 1, PF_ENUM | PF_CTL_COMBO, organ_wave_names, "perc_fm_waveform", "P: Modulator Wave" },
    { 6,      1, 32, 32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "perc_fm_harmonic", "P: Modulator Frq" },
    { 0,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "perc_vel2fm", "P: Vel->FM" },
    
    { 0,         0,  organ_enums::perctrig_count - 1, 0, PF_ENUM | PF_CTL_COMBO, organ_percussion_trigger_names, "perc_trigger", "P: Trigger" },
    { 90,      0,360, 361, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "perc_stereo", "P: Stereo Phase" },

    { 0,         0,  1, 0, PF_BOOL | PF_CTL_TOGGLE, NULL, "filter_chain", "Filter 1 To 2" },
    { 0.1,         0,  1, 100, PF_FLOAT | PF_SCALE_GAIN | PF_CTL_KNOB | PF_PROP_OUTPUT_GAIN | PF_PROP_GRAPH, NULL, "master", "Volume" },
    
    { 2000,   20, 20000, 100, PF_FLOAT | PF_SCALE_LOG | PF_UNIT_HZ | PF_CTL_KNOB, NULL, "f1_cutoff", "F1 Cutoff" },
    { 2,        0.7,    8,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB, NULL, "f1_res", "F1 Res" },
    { 8000,  -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f1_env1", "F1 Env1" },
    { 0,     -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f1_env2", "F1 Env2" },
    { 0,     -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f1_env3", "F1 Env3" },
    { 0,          0,    2,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "f1_keyf", "F1 KeyFollow" },

    { 2000,   20, 20000, 100, PF_FLOAT | PF_SCALE_LOG | PF_UNIT_HZ | PF_CTL_KNOB, NULL, "f2_cutoff", "F2 Cutoff" },
    { 2,        0.7,    8,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB, NULL, "f2_res", "F2 Res" },
    { 0,     -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f2_env1", "F2 Env1" },
    { 8000,  -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f2_env2", "F2 Env2" },
    { 0,     -10800,10800,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "f2_env3", "F2 Env3" },
    { 0,          0,    2,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "f2_keyf", "F2 KeyFollow" },

    { 1,          1,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_a", "EG1 Attack" },
    { 350,       10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_d", "EG1 Decay" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr_s", "EG1 Sustain" },
    { 50,       10,20000,     0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr_r", "EG1 Release" },
    { 0,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr_v", "EG1 VelMod" },
    { 0,  0, organ_enums::ampctl_count - 1,
                              0, PF_INT | PF_CTL_COMBO, organ_ampctl_names, "eg1_amp_ctl", "EG1 To Amp"},

    { 1,          1,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr2_a", "EG2 Attack" },
    { 350,       10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr2_d", "EG2 Decay" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr2_s", "EG2 Sustain" },
    { 50,       10,20000,     0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr2_r", "EG2 Release" },
    { 0,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr2_v", "EG2 VelMod" },
    { 0,  0, organ_enums::ampctl_count - 1,
                              0, PF_INT | PF_CTL_COMBO, organ_ampctl_names, "eg2_amp_ctl", "EG2 To Amp"},

    { 1,          1,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr3_a", "EG3 Attack" },
    { 350,       10,20000,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr3_d", "EG3 Decay" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr3_s", "EG3 Sustain" },
    { 50,       10,20000,     0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_MSEC, NULL, "adsr3_r", "EG3 Release" },
    { 0,          0,    1,    0, PF_FLOAT | PF_SCALE_PERC, NULL, "adsr3_v", "EG3 VelMod" },
    { 0,  0, organ_enums::ampctl_count - 1,
                              0, PF_INT | PF_CTL_COMBO, organ_ampctl_names, "eg3_amp_ctl", "EG3 To Amp"},

    { 6.6,     0.01,   80,    0, PF_FLOAT | PF_SCALE_LOG | PF_CTL_KNOB | PF_UNIT_HZ, NULL, "vib_rate", "Vib Rate" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB , NULL, "vib_amt", "Vib Mod Amt" },
    { 0.5,        0,    1,    0, PF_FLOAT | PF_SCALE_PERC | PF_CTL_KNOB , NULL, "vib_wet", "Vib Wet" },
    { 180,        0,  360,    0, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_DEG, NULL, "vib_phase", "Vib Stereo" },
    { organ_enums::lfomode_global,        0,  organ_enums::lfomode_count - 1,    0, PF_ENUM | PF_CTL_COMBO, organ_vibrato_mode_names, "vib_mode", "Vib Mode" },
//    { 0,  0, organ_enums::ampctl_count - 1,
//                              0, PF_INT | PF_CTL_COMBO, organ_ampctl_names, "vel_amp_ctl", "Vel To Amp"},

    { -12,        -24, 24,   49, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_SEMITONES, NULL, "transpose", "Transpose" },
    { 0,       -100,  100,  201, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "detune", "Detune" },

    { 16,         1,   32,   32, PF_INT | PF_SCALE_LINEAR | PF_CTL_KNOB, NULL, "polyphony", "Polyphony" },
    
    { 1,          0,    1,    0, PF_BOOL | PF_CTL_TOGGLE, NULL, "quad_env", "Quadratic AmpEnv" },

    { 200,        0, 2400,   25, PF_FLOAT | PF_SCALE_LINEAR | PF_CTL_KNOB | PF_UNIT_CENTS, NULL, "pbend_range", "PBend Range" },
    
    { 0,          0,    0,    0, PF_STRING | PF_PROP_MSGCONTEXT, &organ_init_map_curve, "map_curve", "Key mapping curve" },
};

////////////////////////////////////////////////////////////////////////////

void calf_plugins::get_all_plugins(std::vector<plugin_metadata_iface *> &plugins)
{
    #define PER_MODULE_ITEM(name, isSynth, jackname) plugins.push_back(new name##_metadata);
    #define PER_SMALL_MODULE_ITEM(...)
    #include <3rd-calf/modulelist.h>
}


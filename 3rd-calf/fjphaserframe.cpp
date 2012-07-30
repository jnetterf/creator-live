/* This file is part of the Calf Creator Live plugin.
 * The Calf Creator Live plugin is a fork of the Calf DSP library.
 *
 * Copyright (C) Joshua Netterfield <joshua@nettek.ca> 2012
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

#include "fjphaserframe.h"
#include "ui_fjphaserframe.h"

using namespace live_widgets;

FJPhaserFrame::FJPhaserFrame(FJPhaserApp* app, QWidget *parent)
    : AppFrame(parent)
    , s_app(*app)
    , ui(new Ui::FJPhaserFrame)
{
    ui->setupUi(this);

    onFreq(s_app.getFreq());
    onDepth(s_app.getDepth());
    onRate(s_app.getRate());
    onFB(s_app.getFB());
    onStages(s_app.getStages());

    if (s_app.getAmount() + s_app.getDryAmount() < 0.1f)
    {
        onWetDryBalance(s_app.getAmount() * 50.0f / 6.0f + 50.0f);
    }
    else
    {
        onWetDryBalance(50.0f);
    }
}

FJPhaserFrame::~FJPhaserFrame()
{
    s_app.deleteLater();
    delete ui;
}

void FJPhaserFrame::onFreq(int z)
{
    s_app.setFreq(z);
    ui->dial_center->setValue(z);
}

void FJPhaserFrame::onDepth(int z)
{
    s_app.setDepth(z);
    ui->dial_depth->setValue(z);
}

void FJPhaserFrame::onRate(int z)
{
    s_app.setRate(float(z)/1000.0f);
    ui->dial_rate->setValue(z);
}

void FJPhaserFrame::onFB(int z)
{
    s_app.setFB(float(z)/100.0f);
    ui->dial_feedback->setValue(z);
}

void FJPhaserFrame::onStages(int z)
{
    s_app.setStages(z);
    ui->dial_stages->setValue(z);
}

void FJPhaserFrame::onReset()
{
    s_app.setReset(1.0f);
}

void FJPhaserFrame::onWetDryBalance(int z)
{
    s_app.setWetDryBalance(z);
    ui->verticalSlider_amount->setValue(z);
}
// Copyright (C) 2007, 2008, 2009 Marc Betoule

// This file is part of SkOP.

// SkOP is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
  
// SkOP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SkOP.  If not, see <http://www.gnu.org/licenses/>.


/**
 * \file overlaydock.cpp
 * \brief class implementation for the widget presenting web information
 */    

#include "overlaydock.h"
#include "overlaydock.moc"
#include "maplistmodel.h"
#include "toolbox.h"
#include "glview.h"
#include <iostream>
using namespace std;
//#include <qurl.h>

GraticuleTab::GraticuleTab(GLView * glview, QWidget *parent)
  :QWidget(parent),glv(glview)
{
  QGridLayout *layout = new QGridLayout(this);
  showGrat = new QCheckBox(tr("Draw the graticule"),this);
  GraticuleSpinBox * gratRes = new GraticuleSpinBox(this);
  gratRes->setMaximum(90);
  gratRes->setMinimum(1);
  gratRes->setSuffix(tr("deg"));
  layout->addWidget(showGrat,0,0);
  layout->addWidget(new QLabel(tr("Resolution:"),this),1,0);
  layout->addWidget(gratRes,1,1);
  setLayout(layout);
  connect(showGrat,SIGNAL(stateChanged(int)), glview,SLOT(changeGrat(int)));
  connect(gratRes,SIGNAL(valueChanged(int)), glview,SLOT(changeGratRes(int)));
  gratRes->setValue(10);
}

void GraticuleSpinBox::stepBy ( int steps ){
  int ngrat = 180/value();
  ngrat-=steps;
  while(180%ngrat !=0) ngrat-=steps;
  setValue(180/ngrat);
}


// Copyright (C) 2007, 2008 Marc Betoule

// This file is part of SkOP.

// SkOP is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// any later version.
  
// SkOP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
  
// You should have received a copy of the GNU General Public License
// along with SkOP; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


/**
 * \file histodock.cpp
 * \brief class implementation for the widget presenting histogram information about the map.
 */


#include "histodock.h"
#include "histodock.moc"
#include "sphericalfield.h"
#include "maplistmodel.h"
HistoDock::HistoDock(QWidget *parent,MapListModel * mapList)
   :QWidget(parent)
{
  model=mapList;
    createControls();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(extremumGroup);
    setLayout(layout);
}


void HistoDock::createControls()
{
    extremumGroup = new QGroupBox("Scale");
    minimumLabel = new QLabel(tr("Minimum value:"));
    maximumLabel = new QLabel(tr("Maximum value:"));
    
    minimumSpinBox = new EngSpinBox;
    maximumSpinBox = new EngSpinBox;
    minimumSpinBox->setRate(-1.1);
    maximumSpinBox->setRate(1.1);
        
    QGridLayout *extremumLayout = new QGridLayout;
    
    extremumLayout->addWidget(maximumLabel, 0, 0);
    extremumLayout->addWidget(maximumSpinBox, 0, 1);
    extremumLayout->addWidget(minimumLabel, 1, 0);
    extremumLayout->addWidget(minimumSpinBox, 1, 1);
    
    extremumGroup->setLayout(extremumLayout);
    
    connect(minimumSpinBox, SIGNAL(valueChanged(double)),
	    maximumSpinBox, SLOT(setFocalPoint(double)));
    connect(maximumSpinBox, SIGNAL(valueChanged(double)),
	    minimumSpinBox, SLOT(setFocalPoint(double)));
    connect(maximumSpinBox, SIGNAL(valueChanged(double)),
	    this, SLOT(setMaxBound(double)));
    connect(minimumSpinBox, SIGNAL(valueChanged(double)),
	    this, SLOT(setMinBound(double)));
}

void HistoDock::setMinBound(double newMin)
{
  currentMin = newMin;
  emit(boundChanged(currentMin, currentMax));
}

void HistoDock::setMaxBound(double newMax)
{
  currentMax = newMax;
  emit(boundChanged(currentMin, currentMax));
}

void HistoDock::selectMap(const QModelIndex &current, const QModelIndex &previous)
{
  model->setData(previous,QVariant(currentMin),MinScaleRole);
  model->setData(previous,QVariant(currentMax),MaxScaleRole);
  
  minimumSpinBox->setRange(current.data(MinValRole).toDouble(), current.data(MaxValRole).toDouble());
  maximumSpinBox->setRange(current.data(MinValRole).toDouble(), current.data(MaxValRole).toDouble());
  
  minimumSpinBox->setValue(current.data(MinScaleRole).toDouble());
  maximumSpinBox->setValue(current.data(MaxScaleRole).toDouble());
    
}

// Copyright 2007 Marc Betoule

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
HistoDock::HistoDock()
{
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
    
    minimumSpinBox = new QSpinBox;
    maximumSpinBox = new QSpinBox;
    minimumSpinBox->setRange(0, 100);
    maximumSpinBox->setRange(0, 100);
    minimumSpinBox->setSingleStep(1);
    maximumSpinBox->setSingleStep(1);
    minimumSpinBox->setSuffix(tr("%"));
    maximumSpinBox->setSuffix(tr("%"));
    minimumSlider = new QSlider(Qt::Horizontal);
    maximumSlider = new QSlider(Qt::Horizontal);
    minimumSlider->setMinimum(0);
    minimumSlider->setMaximum(100);
    maximumSlider->setMinimum(0);
    maximumSlider->setMaximum(100);
    
    QGridLayout *extremumLayout = new QGridLayout;
    
    extremumLayout->addWidget(minimumLabel, 0, 0);
    extremumLayout->addWidget(minimumSpinBox, 0, 1);
    extremumLayout->addWidget(minimumSlider, 1, 0,1,2);
    extremumLayout->addWidget(maximumLabel, 2, 0);
    extremumLayout->addWidget(maximumSpinBox, 2, 1);
    extremumLayout->addWidget(maximumSlider, 3, 0,1,2);
    extremumGroup->setLayout(extremumLayout);
    
    connect(minimumSlider, SIGNAL(valueChanged(int)),
	    this, SLOT(setMinBound(int)));
    connect(maximumSlider, SIGNAL(valueChanged(int)),
	    this, SLOT(setMaxBound(int)));
    connect(minimumSpinBox, SIGNAL(valueChanged(int)),
	    minimumSlider, SLOT(setValue(int)));
    connect(maximumSpinBox, SIGNAL(valueChanged(int)),
	    maximumSlider, SLOT(setValue(int)));
    connect(minimumSlider, SIGNAL(valueChanged(int)),
	    minimumSpinBox, SLOT(setValue(int)));
    connect(maximumSlider, SIGNAL(valueChanged(int)),
	    maximumSpinBox, SLOT(setValue(int)));
}

void HistoDock::setMinBound(int newMin)
{
    //if(newMin < newMax){
	currentMin = absMin + newMin * (absMax - absMin) /100.;
	emit(boundChanged(currentMin, currentMax));
	//}
}

void HistoDock::setMaxBound(int newMax)
{
    //if(newMax > newMin){
	currentMax = absMin + newMax * (absMax -absMin) / 100.;
	emit(boundChanged(currentMin, currentMax));
	//}
    
}

void HistoDock::selectMap(const QModelIndex &current, const QModelIndex &previous)
{
    SphericalField * val = current.data(Qt::UserRole).value<SphericalField *>();
    absMin = val->getMin();
    absMax = val->getMax();
    setMinBound(0);
    setMaxBound(100);
    
}

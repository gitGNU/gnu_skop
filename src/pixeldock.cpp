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
 * \file pixeldock.cpp
 * \brief class implementation for the widget presenting information about the selected pixel
 */    

#include "pixeldock.h"
#include "pixeldock.moc"
#include "toolbox.h"
#include "maplistmodel.h"

PixelDock::PixelDock(QWidget *glview, QWidget *parent)
   :QWidget(parent)
{
  createControls();
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(selectedPixelGroup);
  layout->addWidget(selectedRegionGroup);
  layout->addWidget(generalGroup);
  setLayout(layout);
  
  connect(showSel,SIGNAL(stateChanged(int)), glview,SLOT(changeSel(int)));
  connect(selRadius,SIGNAL(valueChanged(double)),glview,SLOT(changeSelRadius(double)));
  selRadius->setValue(10);
}


void PixelDock::createControls()
{
  selectedPixelGroup = new QGroupBox(tr("Selected pixel"),this);
    thetaLabel = new QLabel(tr("theta (rad):"));
    phiLabel= new QLabel(tr("phi (rad):"));
    lon= new QLabel(tr("lon (deg):"));
    lat= new QLabel(tr("lat (deg):"));
    thetaLabelVal = new QLabel();
    phiLabelVal= new QLabel();
    lonVal= new QLabel();
    latVal= new QLabel();
    pixelValue= new QLabel(tr("T(N/S):"));
    pixelValueVal = new QLabel();
    QGridLayout *pixelLayout = new QGridLayout(selectedPixelGroup);
    pixelLayout->addWidget(thetaLabel, 0, 0);
    pixelLayout->addWidget(thetaLabelVal, 0, 1);
    pixelLayout->addWidget(phiLabel, 0, 2);
    pixelLayout->addWidget(phiLabelVal, 0, 3);
    pixelLayout->addWidget(lon, 1, 0);
    pixelLayout->addWidget(lonVal, 1, 1);
    pixelLayout->addWidget(lat, 1, 2);
    pixelLayout->addWidget(latVal, 1, 3);
    pixelLayout->addWidget(pixelValue, 2, 0);
    pixelLayout->addWidget(pixelValueVal, 2, 1);
    selectedPixelGroup->setLayout(pixelLayout);

    selectedRegionGroup = new QGroupBox(tr("Circular region"),this);
    showSel = new QCheckBox(tr("Draw a circle around selected pixel"),selectedRegionGroup);
    selRadius = new QDoubleSpinBox(selectedRegionGroup);
    selRadius->setSuffix(tr("arcmin"));
    QGridLayout *regionLayout = new QGridLayout(selectedRegionGroup);
    regionLayout->addWidget(showSel,0,0,1,2);
    regionLayout->addWidget(selRadius,1,1);
    regionLayout->addWidget(new QLabel(tr("Radius:"),selectedRegionGroup),1,0);

    generalGroup = new QGroupBox("General info",this);
    nside = new QLabel(tr("nside:"));
    nsideVal = new QLabel();
    QGridLayout *generalLayout = new QGridLayout;
    generalLayout->addWidget(nside, 0, 0);
    generalLayout->addWidget(nsideVal, 0, 1);
    generalGroup->setLayout(generalLayout);
//     connect(maximumSpinBox, SIGNAL(valueChanged(double)),
// 	    this, SLOT(setMaxBound(double)));
//     connect(minimumSpinBox, SIGNAL(valueChanged(double)),
// 	    this, SLOT(setMinBound(double)));
}

void PixelDock::update(){
  thetaLabelVal->setNum(theta);
  phiLabelVal->setNum(phi);
  lonVal->setNum(fmodulo(phi,M_PI*2)*180/M_PI);
  latVal->setNum(90-(fmodulo(theta,M_PI)*180/M_PI));
  pixelValueVal->setNum(map.data(PointerRole).value<SphericalField *>()->getValue(theta,phi));
  nsideVal->setNum(sqrt(map.data(NsideRole).toInt()/12));
  pixelValue->setText(tr("T(%1):").arg(map.data(UnitRole).toString()));
}

void PixelDock::selectPixel(double theta0, double phi0){
  theta = theta0;
  phi = phi0;
  update();
}
void PixelDock::selectMap(const QModelIndex &current, const QModelIndex &previous){
  map = current;
  update();
}

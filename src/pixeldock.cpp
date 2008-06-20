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
 * \file pixeldock.cpp
 * \brief class implementation for the widget presenting information about the selected pixel
 */    

#include "pixeldock.h"
#include "pixeldock.moc"
#include "maplistmodel.h"
PixelDock::PixelDock(QWidget *parent)
   :QWidget(parent)
{
  createControls();
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(selectedPixelGroup);
  layout->addWidget(generalGroup);
  setLayout(layout);
}


void PixelDock::createControls()
{
    selectedPixelGroup = new QGroupBox("Selected pixel");
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
    QGridLayout *pixelLayout = new QGridLayout;
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
    
    generalGroup = new QGroupBox("General info");
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
  lonVal->setNum(phi*180/M_PI);
  latVal->setNum(90-(theta*180/M_PI));
  pixelValueVal->setNum(map.data(PointerRole).value<SphericalField *>()->getValue(theta,phi));
  nsideVal->setNum(map.data(NsideRole).toInt());
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

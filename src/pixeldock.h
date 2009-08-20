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
 * \file pixeldock.h
 * \brief class definition for the widget presenting information about the selected pixel
 */    

#ifndef PIXELDOCK_H
#define PIXELDOCK_H

#include <QtGui>

class PixelDock : public QWidget{
    Q_OBJECT
public slots:
    void selectPixel(double theta, double phi);
    void selectMap(const QModelIndex &current, const QModelIndex &previous);
    
public:
  PixelDock(QWidget *glview ,QWidget *parent = 0);
    
private:
  void createControls();
  void update();
  QGroupBox *selectedPixelGroup;
    
  QLabel *thetaLabel;
  QLabel *phiLabel;
  QLabel *lon;
  QLabel *lat;
  QLabel *thetaLabelVal;
  QLabel *phiLabelVal;
  QLabel *lonVal;
  QLabel *latVal;
  QLabel *pixelValue;
  QLabel *pixelValueVal;

  QGroupBox *generalGroup;
  QLabel *nside;
  QLabel *nsideVal;
  
  QPersistentModelIndex map;
  
  QGroupBox *selectedRegionGroup;
  QCheckBox *showSel;
  QDoubleSpinBox *selRadius;
  double theta, phi;
};

#endif

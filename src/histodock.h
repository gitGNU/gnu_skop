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
 * \file histodock.h
 * \brief class definition for the widget presenting histogram information about the map.
 */    

#ifndef HISTODOCK_H
#define HISTODOCK_H

#include <QtGui>
#include "engspinbox.h"

class MapListModel;

class HistoDock : public QWidget{
    Q_OBJECT
public slots:
    void setMinBound(double newMin);
    void setMaxBound(double newMax);
    void selectMap(const QModelIndex &current, const QModelIndex &previous);
signals:
    void minChanged(double newMin);
    void maxChanged(double newMax);
    void boundChanged(double newMin, double newMax);
    
public:
  HistoDock(QWidget *parent = 0,MapListModel * mapList = 0);
    
private:
    void createControls();
    
    QGroupBox *extremumGroup;
    
    QLabel *minimumLabel;
    EngSpinBox *minimumSpinBox;
        
    QLabel *maximumLabel;
    EngSpinBox *maximumSpinBox;
    

    double currentMin;
    double currentMax;
    
    double absMin;
    double absMax;
    
    MapListModel * model;
};

#endif

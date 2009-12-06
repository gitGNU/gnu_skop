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


/***
 * \file overlaydock.h 
 * \brief class definition for the widgets presenting the graticule setup and all the stuff displayed over the glview
 */    

#ifndef OVERLAYDOCK_H
#define OVERLAYDOCK_H

#include <QtGui>

class GLView;

class GraticuleSpinBox:public QSpinBox{
  Q_OBJECT
public:
  GraticuleSpinBox(QWidget *parent = 0)
    :QSpinBox(parent){}
  void stepBy ( int steps );
  
};

class GraticuleTab: public QWidget{
    Q_OBJECT
public slots:
  
public:
  GraticuleTab(GLView * glview, QWidget *parent = 0);
  
private:
  GLView *glv;
  QCheckBox *showGrat;
};

#endif

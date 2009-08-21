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
 * \file simbaddock.h
 * \brief class definition for the widget presenting web information
 */    

#ifndef SIMBADDOCK_H
#define SIMBADDOCK_H

#include <QtGui>

class QWebView;
class GLView;

class SimbadDock : public QWidget{
    Q_OBJECT
public slots:
  void selectPixel(double theta, double phi);
  // void selectMap(const QModelIndex &current, const QModelIndex &previous);
  void querySimbad();

public:
  SimbadDock(GLView * glview, QWidget *parent = 0);
  
private:
  QWebView *view;
  QPushButton *button;
  double theta, phi;
  GLView *gl;
  QRadioButton * typesButton[6];
  QLineEdit * typeLine;
  const static QString oTypesNames[6];
};

#endif

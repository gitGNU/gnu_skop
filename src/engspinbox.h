// Copyright (C) 2008, 2009 Marc Betoule

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
 * \file engspinbox.cpp
 * \brief class definition for SpinBox with special behaviour adapted to floating point values.
 */    


#ifndef ENGSPINBOX_H
#define ENGSPINBOX_H

#include <QtGui>
#include <iostream>
class EngSpinBox:public QDoubleSpinBox
{
  Q_OBJECT
  public slots:
  
  void setFocalPoint(double fp){fP = fp;}
  
  public:
  void setRate(double rt){rate=rt;}
  EngSpinBox(QWidget *parent = 0);
  double focalPoint(){return fP;};
  
  virtual double valueFromText(const QString &text) const;  
  virtual QString textFromValue ( double value ) const;
  virtual QValidator::State validate ( QString & input, int & pos ) const;
  //virtual void fixup(QString &str) const;
  virtual void stepBy(int steps);
private:
  double fP;
  double rate;
};

#endif

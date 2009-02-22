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
 * \brief class implementation for SpinBox with special behaviour adapted to floating point values.
 */    

#include <iostream>
#include "engspinbox.h"
#include "engspinbox.moc"
#include <cmath>

EngSpinBox::EngSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent),fP(0),rate(2.0)
{
  setDecimals(15);
}

double EngSpinBox::valueFromText(const QString &text) const{
  //std::cout << (lineEdit()->inputMask()).toStdString() << "\n";
  return text.toDouble();
}

QString EngSpinBox::textFromValue ( double value ) const{
  return tr("%1").arg(value,0,'g');
}

QValidator::State EngSpinBox::validate ( QString & input, int & pos ) const{
  return QValidator::Acceptable;
}

//void EngSpinBox::fixup(QString &str) const{

//}

void EngSpinBox::stepBy(int steps){
  if (steps == -1){
    if(rate > 0)
      setValue(fP + (value()-fP)/fabs(rate));
    else setValue(fP + (value()-fP)*fabs(rate));
  }else{
    if(rate > 0)
      setValue(fP + (value()-fP)*fabs(rate));
    else setValue(fP + (value()-fP)/fabs(rate));
  }
}

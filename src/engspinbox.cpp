// Copyright 2007-2008 Marc Betoule

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
 * \file engspinbox.cpp
 * \brief class implementation for SpinBox with special behaviour adapted to floating point values.
 */    

#include <iostream>
#include "engspinbox.h"
#include "engspinbox.moc"

EngSpinBox::EngSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent),fP(0),rate(2.0)
{
}

double EngSpinBox::valueFromText(const QString &text) const{
  std::cout << (lineEdit()->inputMask()).toStdString() << "\n";
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
    std::cout << fP + (value()-fP)/2.<<"\n";
    if(rate > 0)
      setValue(fP + (value()-fP)/abs(rate));
    else setValue(fP + (value()-fP)*abs(rate));
  }else{
    std::cout << fP + (value()-fP)*2.<<"\n";
    if(rate > 0)
      setValue(fP + (value()-fP)*abs(rate));
    else setValue(fP + (value()-fP)/abs(rate));
  }
}

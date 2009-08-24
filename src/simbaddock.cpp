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
 * \file simbaddock.cpp
 * \brief class implementation for the widget presenting web information
 */    

#include "simbaddock.h"
#include "simbaddock.moc"
#include "maplistmodel.h"
#include "toolbox.h"
#include "glview.h"
#include <QtWebKit>
//#include <qurl.h>

const QString SimbadDock::oTypesNames[6]={"Any", "Rad", "IR", "Neb", "Psr","Other"};


SimbadDock::SimbadDock(GLView * glview, QWidget *parent)
   :QWidget(parent)
{
  gl = glview;
  QVBoxLayout *generalLayout = new QVBoxLayout(this);
  button = new QPushButton("Query Simbad ", this);
  generalLayout->addWidget(button);

  QGroupBox * oTypeGroup = new QGroupBox("Object type",this);
  generalLayout->addWidget(oTypeGroup);
  
  typeLine = new QLineEdit(this);
  generalLayout->addWidget(typeLine);

  view = new QWebView(this);
  generalLayout->addWidget(view);
  connect(button, SIGNAL(clicked()), this,SLOT(querySimbad()));
  view->show();

  QHBoxLayout *layout = new QHBoxLayout(oTypeGroup);
  oTypeGroup->setLayout(layout);
  for (int i = 0; i<6;i++){
    typesButton[i] = new QRadioButton(oTypesNames[i],oTypeGroup);
    layout->addWidget(typesButton[i]);
  }
  connect(typesButton[5],SIGNAL(toggled(bool)),typeLine,SLOT(setEnabled(bool)));
  typesButton[0]->setChecked(true);
  typeLine->setEnabled(false);
  
}

void
SimbadDock::querySimbad(){
  int i=-1;
  while(!typesButton[++i]->isChecked());
  
  QString coord;
  if (90-(fmodulo(theta,M_PI)*180/M_PI)<0){
    coord = "Criteria=region(circle,GAL,%1d%2,%3d)";
  }
  else{
    coord = "Criteria=region(circle,GAL,%1d+%2,%3d)";
  }
  coord = coord.arg(fmodulo(phi,M_PI*2)*180/M_PI).arg(90-(fmodulo(theta,M_PI)*180/M_PI)).arg(gl->getSelRadius()*rad2degr);

  switch(i){
  case 0:
    break;
  case 5:
    coord = QString("%1 & maintypes=%2").arg(coord).arg(typeLine->text());
    break;
  default:
    coord = QString("%1 & maintypes=%2").arg(coord).arg(oTypesNames[i]);
  }
  //cout << coord.toStdString()<< endl;
  QByteArray regQuery = QUrl::toPercentEncoding (coord,"=()");
  //cout << &(*regQuery) << endl;
  QUrl surl("http://simbad.u-strasbg.fr/simbad/sim-sam");
  surl.setEncodedQuery(regQuery);
  surl.addQueryItem("OutputMode","LIST");
  view->load(surl);
  view->show();
}

void SimbadDock::selectPixel(double theta0, double phi0){
  theta = theta0;
  phi = phi0;
}

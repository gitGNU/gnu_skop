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
#include <QtWebKit>
//#include <qurl.h>

SimbadDock::SimbadDock(QWidget *parent)
   :QWidget(parent)
{
  view = new QWebView(this);
  view->show();
  button = new QPushButton("Query Simbad ", this);
  connect(button, SIGNAL(clicked()), this,SLOT(querySimbad()));
}

void
SimbadDock::querySimbad(){
  QString coord = "%1d%2";
  QUrl surl("http://simbad.u-strasbg.fr/simbad/sim-coo");
  surl.addQueryItem("CooFrame","GAL");
  surl.addQueryItem("Coord",coord.arg(fmodulo(phi,M_PI*2)*180/M_PI).arg(90-(fmodulo(theta,M_PI)*180/M_PI)));
  surl.addQueryItem("Radius","10");
  surl.addQueryItem("Radius.unit","arcmin");
  view->load(surl);
  view->show();
}

void SimbadDock::selectPixel(double theta0, double phi0){
  theta = theta0;
  phi = phi0;
}

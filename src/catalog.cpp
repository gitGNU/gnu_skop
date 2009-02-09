// Copyright (C) 2009 Marc Betoule

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
 * \file catalog.cpp
 * \brief implementation for the object of point source catalog.
 */    

#include "catalog.h"
#include <QPainter>

void Source::draw(QPainter & painter,GLView * view){
  int x, y,radius = 5;
  if(view->sky2pixel(theta,phi,x,y)){
    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    painter.save();
    painter.translate(x - radius, y - radius);
    painter.drawEllipse(0, 0, int(2*radius), int(2*radius));
    painter.restore();  
  }
}

void Source::read(istream &is){
  is >> theta
     >> phi
     >> radius;
}

void Catalog::read(string filename){
  ifstream is(filename.c_str(),ios::in);
  char comment[1000];
  while(is){
    while(is.peek() == '#') is.getline(comment, 1000);
    Source s;
    s.read(is);
    sources.push_back(s);
  }
}

void Catalog::draw(QPainter & painter,GLView * view){
  list<Source>::iterator s;
  int i =0;
  cout << "il ya " << sources.size() << endl;
  for (s = sources.begin() ; s != sources.end();s++){
    cout << "source: "<<i++ << endl;
    cout << (*s).theta << "," << (*s).phi << ","<< (*s).radius << ","<<endl;
    (*s).draw(painter, view);
    
  }

}

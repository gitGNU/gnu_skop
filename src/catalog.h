// Copyright (C) 2009 Marc Betoule

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
 * \file catalog.h
 * \brief class definition for the object of point source catalog.
 */    


#ifndef CATALOG_H
#define CATALOG_H

#include <string>
#include <list>
#include "glview.h"
#include <fstream>
using namespace std;

class Source
{
public:  
  void draw(QPainter & painter, GLView * view);
  void read(istream &is);
  double theta, phi, radius;
};

class Catalog
{   
 public:
  
  void draw(QPainter & painter, GLView * view);
  void read(string filename);
 private:
  list<Source> sources;

};

#endif

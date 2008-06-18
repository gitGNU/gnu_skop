// Copyright 2007 Marc Betoule

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
 * \file toolbox.cpp
 * \brief Miscellanous usefull tools
 */    

#include "toolbox.h"

void Mat3::eulerZXZ(double alpha, double beta, double gamma){
  Mat3 b(cos(gamma),sin(gamma),0,-sin(gamma),cos(gamma),0,0,0,1);
  Mat3 c(1,0,0,0,cos(beta),sin(beta),0,-sin(beta),cos(beta));
  Mat3 d(cos(alpha),sin(alpha),0,-sin(alpha),cos(alpha),0,0,0,1);
  *this = b*c*d;
}

void Mat3::eulerZYZ(double alpha, double beta, double gamma){
  Mat3 b(cos(gamma),sin(gamma),0,-sin(gamma),cos(gamma),0,0,0,1);
  Mat3 c(cos(beta),0,-sin(beta),0,1,0,sin(beta),0,cos(beta));
  Mat3 d(cos(alpha),sin(alpha),0,-sin(alpha),cos(alpha),0,0,0,1);
  *this = b*c*d;
}

ostream & operator << (ostream &os, Mat3 & m){
  os << "(" << m[0][0] << "\t,\t" << m[0][1] << "\t,\t" << m[0][2] << "\t)\n"
     << "(" << m[1][0] << "\t,\t" << m[1][1] << "\t,\t" << m[1][2] << "\t)\n"
     << "(" << m[2][0] << "\t,\t" << m[2][1] << "\t,\t" << m[2][2] << "\t)\n";
  return os;
}

ostream & operator << (ostream &os, Vec3 & v){
  os << "(" << v[0] << "\t)\n"
     << "(" << v[1] << "\t)\n"
     << "(" << v[2] << "\t)\n";
  return os;
}

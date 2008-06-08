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

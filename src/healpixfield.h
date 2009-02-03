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
 * \file healpixfield.h
 * \brief class for healpix pixelized fields definition.
 */    


#ifndef HEALPIXFIELD_H
#define HEALPIXFIELD_H

#include "sphericalfield.h"
#include <cmath>
#include <bitset>
/*
 * \brief lightweight implementation of the healpix pixelisation functionnalities
 */
class HealpixField: public SphericalField{
  //  Q_OBJECT
public:
    HealpixField(){
      shader = "ang2pix_nest.frag";
    }
    
	    

    ind coord2pix(double theta, double phi);
    
    void pix2coord(ind pix,
		   double & theta, double & phi);
    
    void read(fitsfile ** fptr);

    void ring2xyf (ind pix, ind &x, ind &y, ind &f) const;
private:
    ind nside;
    ind order;
    ind maskface;  // Stored for optimization purpose
    ind ncap;      // Stored for optimization purpose

};


//! Returns the remainder of the division \a v1/v2.
/*! The result is non-negative.
    \a v1 can be positive or negative; \a v2 must be positive. */
inline double modulo (double v1, double v2)
  {
  using namespace std;
  return (v1>=0) ? ((v1<v2) ? v1 : fmod(v1,v2)) : (fmod(v1,v2)+v2);
  //return (v1>=0) ?  fmod(v1,v2) : (fmod(v1,v2)+v2);
  }

//! Returns the remainder of the division \a v1/v2.
/*! The result is non-negative.
    \a v1 can be positive or negative; \a v2 must be positive. */
inline int modulo (int v1, int v2)
  { return (v1>=0) ? ((v1<v2) ? v1 : (v1%v2)) : ((v1%v2)+v2); }

//! Returns the remainder of the division \a v1/v2.
/*! The result is non-negative.
    \a v1 can be positive or negative; \a v2 must be positive. */
inline long modulo (long v1, long v2)
  { return (v1>=0) ? ((v1<v2) ? v1 : (v1%v2)) : ((v1%v2)+v2); }

inline unsigned int isqrt (unsigned int arg){
  using namespace std;
  return unsigned (sqrt(arg+0.5));
}


inline ind xy2pix(ind x, ind y){
  bitset<16> xb(x), yb(y);
  bitset<32> pb;
  for(size_t i = 0; i < 16; i++){
    pb[i<<1]=xb[i];
    pb[(i<<1) + 1]=yb[i];
  }
  return pb.to_ulong();
}

inline void pix2xy(ind pix, ind & x, ind & y){
  bitset<16> xb, yb;
  bitset<32> pb(pix);
  for(size_t i = 0; i < 16; i++){
    xb[i]=pb[i<<1];
    yb[i]=pb[(i<<1) + 1];
  }
  x=xb.to_ulong();
  y=yb.to_ulong();
}
#endif

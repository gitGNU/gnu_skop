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


#include "healpixfield.h"
#include <cmath>
ind HealpixField::coord2pix(double theta, double phi){
  double z = cos(theta);
  double za = abs(z);


  double tt = modulo(phi,2*M_PI) * M_2_PI; // in [0,4)

//   double tt = __builtin_fmod(phi,2*M_PI);
//   tt = signbit(tt) ? tt : tt + 2*M_PI;
//   tt *= M_2_PI;
  ind order_max=13;
  
  
  const ind ns_max = 1<<order_max;
  ind face_num, ix, iy;
  
  if (za<=2./3.){ // Equatorial region
    double temp1 = ns_max*(0.5+tt);
    double temp2 = ns_max*z*0.75;
    int jp = int(temp1-temp2); // index of  ascending edge line
    int jm = int(temp1+temp2); // index of descending edge line
    int ifp = jp >> order_max;  // in {0,4}
    int ifm = jm >> order_max;
    if (ifp == ifm)           // faces 4 to 7
      face_num = (ifp==4) ? 4: ifp+4;
    else if (ifp < ifm)       // (half-)faces 0 to 3
      face_num = ifp;
    else                      // (half-)faces 8 to 11
      face_num = ifm + 8;
    
    ix = jm & (ns_max-1);
    iy = ns_max - (jp & (ns_max-1)) - 1;
  }
  else{ // polar region, za > 2/3
    int ntt = int(tt);
    double tp = tt-ntt;
    double tmp = ns_max*sqrt(3*(1-za));
    
    int jp = int(tp*tmp); // increasing edge line index
    int jm = int((1.0-tp)*tmp); // decreasing edge line index
    if (jp>=ns_max) jp = ns_max-1; // for points too close to the boundary
    if (jm>=ns_max) jm = ns_max-1;
    if (z >= 0)
      {
        face_num = ntt;  // in {0,3}
        ix = ns_max - jm - 1;
        iy = ns_max - jp - 1;
      }
    else
      {
        face_num = ntt + 8; // in {8,11}
        ix =  jp;
        iy =  jm;
      }
  }
  ind itex, ytex, xtex;
  
  itex = face_num % 4;  
  ytex = itex >> 1;  
  xtex = itex - (ytex << 1);  
  xtex = (xtex << order) + (ix>>(order_max-order));  
  ytex = (ytex << order) + (iy>>(order_max-order));

  int ipf = xy2pix (ix, iy);

  return ((face_num >> 2) << (2*order +2)) + xtex + (ytex << (order+1));
  
  //int ipf = xy2pix (ix, iy);
  
  //ipf >>= (2*(order_max-order));  // in {0, nside**2 - 1}
  
  //return ipf + (face_num<<(2*order));    // in {0, 12*nside**2 - 1}
}



void HealpixField::pix2coord(ind pix, double & theta, double & phi){
  
}

void HealpixField::ring2xyf (ind pix, ind &x, ind &y, ind &f) const
  {
  int iring, iphi, kshift, nr;
  
  int nl2 = 2*nside;

  if (pix<ncap) // North Polar cap
    {
      iring = int(0.5*(1+isqrt(1+2*pix))); //counted from North pole
      iphi  = (pix+1) - 2*iring*(iring-1);
      kshift = 0;
      nr = iring;
      f=0;
      int tmp = iphi-1;
      if (tmp>=(2*iring))
	{
	  f=2;
	  tmp-=2*iring;
	}
      if (tmp>=iring) ++f;
    }
  else if (pix<(npix-ncap)) // Equatorial region
    {
      int ip = pix - ncap;
      if (order>=0){
	iring = (ip>>(order+2)) + nside; // counted from North pole
	iphi  = (ip&(4*nside-1)) + 1;
      }
      else{
	iring = (ip/(4*nside)) + nside; // counted from North pole
	iphi  = (ip%(4*nside)) + 1;
      }
      kshift = (iring+nside)&1;
      nr = nside;
      unsigned int ire = iring-nside+1;
      unsigned int irm = nl2+2-ire;
      int ifm, ifp;
      if (order>=0){
	ifm = (iphi - ire/2 + nside -1) >> order;
	ifp = (iphi - irm/2 + nside -1) >> order;
      }
      else{
	ifm = (iphi - ire/2 + nside -1) / nside;
	ifp = (iphi - irm/2 + nside -1) / nside;
      }
      if (ifp == ifm) // faces 4 to 7
	f = (ifp==4) ? 4 : ifp+4;
      else if (ifp<ifm) // (half-)faces 0 to 3
	f = ifp;
      else // (half-)faces 8 to 11
	f = ifm + 8;
    }
  else // South Polar cap
    {
      int ip = npix - pix;
      iring = int(0.5*(1+isqrt(2*ip-1))); //counted from South pole
      iphi  = 4*iring + 1 - (ip - 2*iring*(iring-1));
      kshift = 0;
      nr = iring;
      iring = 2*nl2-iring;
      f=8;
      int tmp = iphi-1;
      if (tmp>=(2*nr))
	{
	  f=10;
	  tmp-=2*nr;
	}
      if (tmp>=nr) ++f;
    }

  int irt = iring - ((int)((2+(f>>2))*nside)) + 1;
  int ipt = 2*iphi- (((f & 3)<<1)+((~f & 4)>>2))*nr - kshift -1;
  if (ipt>=nl2){cout<<"merde"; ipt-=8*nside;}

  x =  (ipt-irt) >>1;
  y =(-(ipt+irt))>>1;
}


void HealpixField::read(fitsfile ** fptr){
  int result(0), status(0);
  int ncols = 0;
  long nrows = 0;
  int typecode = 0;
  long repeat = 0;
  char scheme[80];
  bool ring = false;
  nside = 0;
  result = fits_get_num_rows(*fptr, &nrows, &status);
  if(result){fits_report_error(stdout, status);}
  result = 0;
  result = fits_get_num_cols(*fptr, &ncols, &status);
  if(result){fits_report_error(stdout, status);}
  result = 0;
  result = fits_read_key(*fptr,TINT, "NSIDE", &nside, NULL, &status);
  if(result){fits_report_error(stdout, status);}
  result = 0;
  result = fits_get_coltype(*fptr, 1, &typecode, &repeat,NULL, &status);
  if(result){fits_report_error(stdout, status);}
  if(nrows*repeat != 12*nside*nside){
    printf("Inconsistent nside parameter : %d\n",nside);
  }
  npix = 12 * nside * nside;
  maskface = nside * nside -1;
  ncap = 2*(nside*nside - nside);
  result = 0;
  result = fits_read_key(*fptr,TSTRING, "ORDERING", scheme, NULL, &status);
  if(result){fits_report_error(stdout, status);}
  ring = (string("RING") == scheme);

  float * tmp = new float[npix];
  data = new float[npix];
  result = fits_read_col(*fptr, TFLOAT, 1, 1, 1, nrows*repeat, 0,tmp, 0, &status);
  if(result){fits_report_error(stdout, status);}
  //order = log2(nside);
  //! \todo Check nside is correct
  for(order = 0; (nside >> order) != 1; order++);
  maxV = tmp[0];
  minV = tmp[0];
  if(ring)
    for (ind i = 0; i< npix;i++){
      ind iface,xf,yf, xtex, ytex, itex, index;
      ring2xyf(i,xf,yf,iface);
      itex = iface % 4;
      ytex = itex >> 1;
      xtex = itex - (ytex << 1);
      xtex = (xtex << order) + xf;
      ytex = (ytex << order) + yf;
      index = ((iface >> 2) << (2*order +2)) + xtex + (ytex << (order+1));
      data [index] = tmp[i];
      if (maxV < tmp[i]) maxV = tmp[i];
      if (minV > tmp[i]) minV = tmp[i];
    }
  else
    for (ind i = 0; i< npix;i++){
      ind iface = i >> (2*order);
      ind xf,yf, xtex, ytex, itex, index;
      pix2xy(i&maskface,xf,yf);
      itex = iface % 4;
      ytex = itex >> 1;
      xtex = itex - (ytex << 1);
      xtex = (xtex << order) + xf;
      ytex = (ytex << order) + yf;
      index = ((iface >> 2) << (2*order +2)) + xtex + (ytex << (order+1));
      data [index] = tmp[i];
      if (maxV < tmp[i]) maxV = tmp[i];
      if (minV > tmp[i]) minV = tmp[i];
    }
  delete tmp;
  
}

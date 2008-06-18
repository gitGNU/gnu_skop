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
      //shader="ang2pix_nest.frag";
      // GLSL code
      shader = "uniform sampler2D tex1,tex2,tex3;\n"
"uniform int nside;\n"
"uniform float dist, theta0, phi0, minv, maxv;\n"
"const float pi = 3.141592653589793238462643383279502884197;\n"
"const mat3 rot = mat3(cos(phi0)*cos(theta0), -sin(phi0)*cos(theta0), sin(theta0),\n"
"		sin(phi0), cos(phi0), 0,\n"
"		-sin(theta0)*cos(phi0),sin(theta0)*sin(phi0),cos(theta0));\n"
"const float nsidef = float(nside);\n"
"const float nsmax=8192.0;\n"
"\n"
"const vec3 col0 = vec3(0,0,0.5);\n"
"const vec3 col1 = vec3(0,0,1);\n"
"const vec3 col2 = vec3(0,1,1);\n"
"const vec3 col3 = vec3(1,1,0);\n"
"const vec3 col4 = vec3(1,0.33,0);\n"
"const vec3 col5 = vec3(0.5,0,0);\n"
"const float diff=maxv-minv;\n"
"const float val1=0.15*diff+minv;\n"
"const float val2=0.4*diff+minv;\n"
"const float val3=0.7*diff+minv;\n"
"const float val4=0.9*diff+minv;\n"
"vec2 coord2thetaphi(vec2 coor){\n"
"  vec2 thetaphi = vec2(acos(coor.y),asin(coor.x/sin(acos(coor.y))));\n"
"  vec3 coord=vec3(cos(thetaphi.y)*sin(thetaphi.x),\n"
"		  sin(thetaphi.y)*sin(thetaphi.x),\n"
"		  cos(thetaphi.x));\n"
"  \n"
"  coord = rot*coord;\n"
"  thetaphi = vec2(acos(coord.z),atan(coord.y,coord.x));\n"
"  return thetaphi;\n"
"}\n"
"\n"
"\n"
"vec4 jet(float val){\n"
"  val = clamp(val,minv,maxv);\n"
"  vec3 res = step(minv, val) * step(0.0, val1-val)\n"
"    * mix(col0,col1,(val-minv)/(val1-minv));\n"
"  res += step(val1, val) * step(0.0, val2-val)\n"
"    * mix(col1,col2,(val-val1)/(val2-val1));\n"
"  res += step(val2, val) * step(0.0, val3-val)\n"
"    * mix(col2,col3,(val-val2)/(val3-val2));\n"
"  res += step(val3, val) * step(0.0, val4-val)\n"
"    * mix(col3,col4,(val-val3)/(val4-val3));\n"
"  res += step(val4, val) * step(0.0, maxv-val)\n"
"    * mix(col4,col5,(val-val4)/(maxv-val4));\n"
"  return vec4(res,0.0);\n"
"}\n"
"\n"
"void main(){\n"
"  vec2 texco = 2.0*gl_TexCoord[0].st-vec2(1.0,1.0);\n"
"  texco = dist * texco;\n"
"  if(length(texco)<=1.0){\n"
"    vec2 thetaphi = coord2thetaphi(texco);\n"
"    texco.y = cos(thetaphi.x);\n"
"    float za = abs(texco.y);\n"
"    float tt = mod(thetaphi.y,2.0*pi)*2.0/pi;\n"
"    int face_num, ix,iy;\n"
"    if(za <= 2./3.){\n"
"      float temp1 = nsmax*(0.5+tt);\n"
"      float temp2 = nsmax*texco.y*0.75;\n"
"      int jp = int(temp1-temp2);\n"
"      int jm = int(temp1+temp2);\n"
"      int ifp = int(float(jp) / nsmax);\n"
"      int ifm = int(float(jm) / nsmax);\n"
"      if(ifp == ifm){\n"
"	face_num = (ifp==4) ? 4: ifp+4;\n"
"      }\n"
"      else if(ifp < ifm){\n"
"	face_num = ifp;\n"
"      }\n"
"      else{\n"
"	face_num = ifm+8;\n"
"      }\n"
"      ix = int(mod(float(jm),nsmax)); \n"
"      iy = int(nsmax) - int(mod(float(jp),nsmax)) - 1;\n"
"    }\n"
"    else{\n"
"      float tp = fract(tt);\n"
"      float tmp = nsmax*sqrt(3.0*(1.0-za));\n"
"      int jp = int(tp*tmp);\n"
"      int jm = int((1.0-tp)*tmp);\n"
"      \n"
"      jp = int(clamp(float(jp),0.0,nsmax-1.0));\n"
"      jm = int(clamp(float(jm),0.0,nsmax-1.0));\n"
"      if(texco.y >=0){\n"
"	face_num = int(tt);  //in {0,3}\n"
"	ix = nsmax - jm - 1;\n"
"	iy = nsmax - jp - 1;\n"
"      }\n"
"      else{\n"
"	face_num = int(tt) + 8; // in {8,11}\n"
"        ix =  jp;\n"
"        iy =  jm;\n"
"      }\n"
"    }\n"
"    ix = ix * nside / nsmax;\n"
"    iy = iy * nside /nsmax;\n"
"    if(face_num < 4){\n"
"      int itex = mod(float(face_num),4.0);\n"
"      int xf = mod(float(itex),2.0);\n"
"      xf = xf * nside + ix;\n"
"      int yf = int(itex/2);\n"
"      yf = yf * nside + iy;\n"
"      vec2 ij = vec2(xf,yf);\n"
"      \n"
"      //vec4 valvec = vec4(texture2D(tex1, ij/(2*nsidef)));\n"
"      float valvec = texture2D(tex1, ij/(2*nsidef)).x;\n"
"      gl_FragColor = jet(valvec);\n"
"      \n"
"      //gl_FragColor = vec4(texture2D(tex1, ij/(2*nsidef)));\n"
"      //gl_FragColor=vec4(face_num/12.0,0.0,0.0,1.0);\n"
"      //gl_FragColor=vec4(ij/(2*nside),0.0,1.0);\n"
"      //gl_FragColor=vec4(ij/(2*nside),0.0,1.0);      \n"
"    }\n"
"    else if(face_num < 8){\n"
"      int itex = mod(float(face_num),4.0);\n"
"      int xf = mod(float(itex),2.0);\n"
"      xf = xf * nside + ix;\n"
"      int yf = int(itex/2);\n"
"      yf = yf * nside + iy;\n"
"      vec2 ij = vec2(xf,yf);\n"
"      \n"
"      //vec4 valvec = vec4(texture2D(tex2, ij/(2*nsidef)));\n"
"      float valvec = texture2D(tex2, ij/(2*nsidef)).x;\n"
"      gl_FragColor = jet(valvec);\n"
"      \n"
"      //gl_FragColor=vec4(texture2D(tex2, ij/(2*nsidef)));\n"
"      //gl_FragColor=vec4((face_num+1)/4.0,0,0,1.0);\n"
"      //gl_FragColor=vec4(face_num/12.0,0.0,0.0,1.0);\n"
"      //gl_FragColor=vec4(0.0,0.0,1.0,1.0);\n"
"      //gl_FragColor=vec4((xf+1)/2.0,(yf+1)/2,0.0,1.0);\n"
"    }\n"
"    else{\n"
"      int itex = mod(float(face_num),4.0);\n"
"     int xf = mod(float(itex),2.0);\n"
"      xf = xf * nside + ix;\n"
"      int yf = int(itex/2);\n"
"      yf = yf * nside + iy;\n"
"      vec2 ij = vec2(xf,yf);\n"
"      //vec4 valvec = vec4(texture2D(tex3, ij/(2*nsidef)));\n"
"      float valvec = texture2D(tex3, ij/(2*nsidef)).x;\n"
"      gl_FragColor = jet(valvec);\n"
"      //gl_FragColor=vec4(texture2D(tex3, ij/(2*nsidef)));\n"
"      //gl_FragColor=vec4(face_num/12.0,0.0,0.0,1.0);\n"
"      //gl_FragColor=vec4(0.0,(face_num+1)/4.0,0,1.0);\n"
"      //gl_FragColor=vec4(ij/(2*nside),0.0,1.0);\n"
"      //gl_FragColor=vec4((xf+1)/2.0,(yf+1)/2,0.0,1.0);\n"
"      //gl_FragColor=vec4(0.0,0.0,1.0,1.0);\n"
"    }\n"
"  }\n"
"  else{\n"
"    gl_FragColor=vec4(0.0,0,0,1.0);\n"
"  }\n"
"  //gl_FragColor=texture2D(tex, texco);\n"
"  //gl_FragColor=vec4(texco,0,1.0);\n"
"  //gl_FragColor=vec4(gl_TexCoord[0].st,0,1.0);\n"
"  //gl_FragColor=vec4(texture2D(tex, gl_TexCoord[0].st));\n"
"  //gl_FragColor=vec4(1.0,0,0,1.0);\n"
"}\n";
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

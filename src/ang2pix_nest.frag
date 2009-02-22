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

uniform sampler2D tex1,tex2,tex3;
uniform int nside;
uniform float dist, theta0, phi0, minv, maxv;
const float pi = 3.141592653589793238462643383279502884197;
mat3 rot = mat3(cos(phi0)*cos(theta0), -sin(phi0)*cos(theta0), sin(theta0),
sin(phi0), cos(phi0), 0,
		-sin(theta0)*cos(phi0),sin(theta0)*sin(phi0),cos(theta0));
float nsidef = float(nside);
float nsmax=8192.0;
vec3 col0 = vec3(0,0,0.5);
vec3 col1 = vec3(0,0,1);
vec3 col2 = vec3(0,1,1);
vec3 col3 = vec3(1,1,0);
vec3 col4 = vec3(1,0.33,0);
vec3 col5 = vec3(0.5,0,0);
float diff=maxv-minv;
float val1=0.15*diff+minv;
float val2=0.4*diff+minv;
float val3=0.7*diff+minv;
float val4=0.9*diff+minv;
//vec2 coord2thetaphi(vec2 coor){
  //vec2 thetaphi = vec2(acos(coor.y),asin(coor.x/sin(acos(coor.y))));
  //vec3 coord=vec3(cos(thetaphi.y)*sin(thetaphi.x),
  //sin(thetaphi.y)*sin(thetaphi.x),
  //cos(thetaphi.x));
  
  //coord = rot*coord;
  //thetaphi = vec2(acos(coord.z),atan(coord.y,coord.x));
  //return thetaphi;
//  return coor;
//}

vec2 coord2zphi(vec2 coor){
  vec2 thetaphi = vec2(acos(coor.y),asin(coor.x/sin(acos(coor.y))));
  vec3 coord=vec3(cos(thetaphi.y)*sin(thetaphi.x),
  sin(thetaphi.y)*sin(thetaphi.x),
  cos(thetaphi.x));
  
  coord = rot*coord;
  thetaphi = vec2(coord.z,atan(coord.y,coord.x));
  return thetaphi;
  return coor;
}


vec4 jet(float val){
  val = clamp(val,minv,maxv);
  vec3 res = step(minv, val) * step(0.0, val1-val)
    * mix(col0,col1,(val-minv)/(val1-minv));
  res += step(val1, val) * step(0.0, val2-val)
    * mix(col1,col2,(val-val1)/(val2-val1));
  res += step(val2, val) * step(0.0, val3-val)
    * mix(col2,col3,(val-val2)/(val3-val2));
  res += step(val3, val) * step(0.0, val4-val)
    * mix(col3,col4,(val-val3)/(val4-val3));
  res += step(val4, val) * step(0.0, maxv-val)
    * mix(col4,col5,(val-val4)/(maxv-val4));
  return vec4(res,0.0);
}

void main(){
  vec2 texco = 2.0*gl_TexCoord[0].st-vec2(1.0,1.0);
  texco = dist * texco;
  if(length(texco)<=1.0){
    float tt, za;
      {
	vec2 zphi = coord2zphi(texco);
	texco.y = zphi.x;
	za = abs(texco.y);
	tt = mod(zphi.y,2.0*pi)*2.0/pi;
      }
          int face_num, ix,iy;
    if(za <= 2./3.){
      float temp1 = nsmax*(0.5+tt);
      float temp2 = nsmax*texco.y*0.75;
      int jp = int(temp1-temp2);
      int jm = int(temp1+temp2);
      int ifp = int(float(jp) / nsmax);
      int ifm = int(float(jm) / nsmax);
      if(ifp == ifm){
	face_num = (ifp==4) ? 4: ifp+4;
      }
      else if(ifp < ifm){
	face_num = ifp;
      }
      else{
	face_num = ifm+8;
      }
      ix = int(mod(float(jm),nsmax));
      iy = int(mod(float(jp),nsmax));
      iy = int(nsmax)-iy;
      iy = iy -1;
    }
    else{
      float tp = fract(tt);
      float tmp = nsmax*sqrt(3.0*(1.0-za));
      int jp = int(tp*tmp);
      int jm = int((1.0-tp)*tmp);
      
      jp = int(clamp(float(jp),0.0,nsmax-1.0));
      jm = int(clamp(float(jm),0.0,nsmax-1.0));
      if(texco.y >= 0.0){
	face_num = int(tt);  //in {0,3}
	ix = int(nsmax) - jm - 1;
	iy = int(nsmax) - jp - 1;
      }
      else{
	face_num = int(tt) + 8; // in {8,11}
        ix =  jp;
        iy =  jm;
      }
    }
    ix = ix * nside / int(nsmax);
    iy = iy * nside / int(nsmax);
    gl_FragColor=vec4(ix,iy,0,1.0);
    if(face_num < 4){
      int itex = int(mod(float(face_num),4.0));
      int xf = int(mod(float(itex),2.0));
      xf = xf * nside + ix;
      int yf = int(itex/2);
      yf = yf * nside + iy;
      vec2 ij = vec2(xf,yf)/(2*nsidef);
      
      float valvec = texture2D(tex1, ij.xy).x;
      gl_FragColor = jet(valvec);
      //gl_FragColor=valvec;
      //gl_FragColor=vec4(valvec.w, 0.0, 0.0,1.0);
/*       gl_FragColor=vec4(ij.x / (2.0*nside),ij.y / (2.0*nside),0,1.0); */
    }
    
    else if(face_num < 8){
      int itex = int(mod(float(face_num),4.0));
      int xf = int(mod(float(itex),2.0));
      xf = xf * nside + ix;
      int yf = int(itex/2);
      yf = yf * nside + iy;
      vec2 ij = vec2(xf,yf);
      
      float valvec = texture2D(tex2, ij/(2*nsidef)).x;
      gl_FragColor = jet(valvec);
    }
    else{
      int itex = int(mod(float(face_num),4.0));
      int xf = int(mod(float(itex),2.0));
      xf = xf * nside + ix;
      int yf = int(itex/2);
      yf = yf * nside + iy;
      vec2 ij = vec2(xf,yf);
      
      float valvec = texture2D(tex3, ij/(2*nsidef)).x;
      gl_FragColor = jet(valvec);
    }
  }
  else{
   gl_FragColor=vec4(0.0,0,0,1.0);
  }
  //gl_FragColor=texture2D(tex, texco);
  //gl_FragColor=vec4(texco,0,1.0);
  //gl_FragColor=vec4(gl_TexCoord[0].st,0,1.0);
  //gl_FragColor=vec4(texture2D(tex, gl_TexCoord[0].st));
  //gl_FragColor=vec4(1.0,0,0,1.0);
}

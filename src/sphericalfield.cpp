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
 * \file sphericalfield.cpp
 * \brief base class for pixelized fields on the sphere implementation.
 */    

#include "shader.h"
#include "sphericalfield.h"
#include "healpixfield.h"
#include <fstream>

void SphericalField::serialize(){
  if(serialized) return;
  buffername="essai.dat";
  ofstream sortie (buffername.c_str(), ios::out);
  sortie.write((char *)data,sizeof(float)*npix);
  sortie.close();
  delete data;
  serialized = true;
}

void SphericalField::load(){
  if(!serialized) return;
  ifstream entree (buffername.c_str(), ios::in);
  data = new float[npix];
  entree.read((char *)data,sizeof(float)*npix);
  entree.close();
  serialized = false;
}

void SphericalField::bind(){
  if(bound) return;
  load();
  int internal_format = GlslContext::internalFormat;
  int texture_target = GL_TEXTURE_2D;
  int texture_format = GL_LUMINANCE;
  for(int i = 0; i<3; i++){
    int hsize = sqrt(npix/3);
    
    glActiveTexture(GL_TEXTURE0+i+1);
    glGenTextures( 1, textureId+i);
    glBindTexture(texture_target, textureId[i]);
    
    // turn off filtering and set proper wrap mode 
    // (obligatory for float textures atm)
    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // set texenv to replace instead of the default modulate

    // and allocate graphics memory
    glTexImage2D(texture_target, 0, internal_format, 
		 hsize, hsize, 0, texture_format, GL_FLOAT, 0);
    glTexSubImage2D(texture_target,0,0,0,hsize,hsize,
		    texture_format,GL_FLOAT,data+(i*hsize*hsize));
    //glTexImage2D(GL_TEXTURE_2D, 0, internalformat, hsize, hsize, 0,
    //GL_LUMINANCE, GL_FLOAT, data+(i*hsize*hsize) );
    
    cout << textureId[i]<<endl;
    
  }
  bound = true;
  glActiveTexture(GL_TEXTURE0);
}

void SphericalField::clear(){
  if(!bound) return;
  glDeleteTextures( 3, textureId );
  bound = false;
}

float SphericalField::getValue(double theta, double phi){
  load();
  ind i;
  i = coord2pix(theta, phi);
  return data[i];
}

string SphericalField::name()
{
    return idname;
}

void SphericalField::setName(string n)
{
    idname = n;
}

SphericalField::~SphericalField(){
  clear();
  load();
  delete data;
  
}


SphericalField * SphericalField::readFits(string filename){
  fitsfile *fptr;
  int status=0, match=0, exact=0;
  char pixtype[80];

  SphericalField * field = NULL;

  fftopn(&fptr, filename.c_str(), READONLY, &status);
  status = 0;
  ffgky(fptr, TSTRING, "PIXTYPE", pixtype, NULL, &status);
  
  ffcmps("HEALPIX", pixtype, FALSE, &match, &exact);
  if(!match){
    cout << "No valid pixelisation keyword found, defaulting to HEALPIX\n";
    field = new HealpixField();
    field->read(&fptr);
    field->setName(filename);
  }
  else{
    field = new HealpixField();
    field->read(&fptr);
    field->setName(filename);
  }

  status = 0;
  ffgky(fptr, TSTRING, "TUNIT1", pixtype, NULL, &status);
  field->unit = pixtype;
  status = 0;
  fits_close_file(fptr, &status);
  return field;
}


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
 * \file sphericalfield.h
 * \brief base class for pixelized fields on the sphere definition.
 */    


#ifndef SPHERICALFIELD_H
#define SPHERICALFIELD_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fitsio.h>
using namespace std;

typedef unsigned int ind;

class SphericalField{

public:
    //! Write the map to the disk and free the memory
    void serialize();
    //! Reallocate memory and read the map from the disk
    void load();
    //! Load data and program in the texture and shader memory
    void bind();
    //! Free texture and shader memory
    void clear();
    //! Return the value at theta-phi
    float getValue(double theta, double phi);
    //! Return the id name of the map
    string name();
    //! Set the id name of the map
    void setName(string name);
    //! Return the name of the fragment program
    string getShader(){
	return shader;
    }
    string getUnit(){
      return unit;
    }
    double getMin(){
	return minV;
    }
    double getMax(){
	return maxV;
    }
    //-----------Pixelisation dependant function------------
    virtual ind coord2pix(double theta, double phi) = 0;
    virtual void pix2coord(ind pix,
			   double & theta, double & phi) = 0;
    virtual void read(fitsfile ** fptr) = 0;
    //-----------Constructor--------------------------------
    SphericalField()
	:serialized(false),bound(false),idname("default")
	{}
    
    //-----------Destructor---------------------------------
    ~SphericalField();
    
    //-----------Getters------------------------------------
    ind getNPix(){return npix;}
    
    //----------Static functions----------------------------
    static SphericalField * readFits(string filename);
private:
    string buffername;  
    bool serialized;
    bool bound;
    string idname;
    GLuint textureId[3];
protected:
    float * data;
    int npix;
    string shader;
    string unit;
    float minV;
    float maxV;
    //GLSLProg prog;

};



#endif

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
 * \file healpixtest.cpp
 * \brief Test for the healpix routines
 */

#include "healpixtest.h"
#include <ctime>
void HealpixTest::initTestCase(){ 
  f = SphericalField::readFits("/data/marc/WMAP/wmap_band_iqumap_r9_3yr_Ka_v2.fits");
}

void HealpixTest::cleanupTestCase(){
  delete f;
}

void HealpixTest::fromFitsTest(){
  QCOMPARE(f->getNPix(),(ind)12*512*512);
}
void HealpixTest::serializeTest(){
  f->serialize();
}

void HealpixTest::loadTest(){
  f->load();
}

void HealpixTest::getValueTest(){
  float val = f->getValue(1.5694942,0.78539816);
  QCOMPARE(val,461.97387793f);
}

void HealpixTest::speedTest(){
  unsigned int xsize = 2048;
  unsigned int ysize = 2048;
  double stepx = 1./xsize;
  double stepy = 1./ysize;
  double theta = 0;
  double phi = 0;
  float * val = new float[xsize*ysize];
  clock_t begin, end;
  begin = clock();
  for(unsigned int i = 0; i < xsize; i++){
    theta += stepx;
    phi = 0;
    for(unsigned int j = 0; j < ysize; j++){
      phi += stepy;
      val[i] = f->getValue(theta,phi);
    }
  }
  end = clock();
  QWARN(tr("computing %1 pixels in %2 sec").arg(xsize*ysize).arg(((double)(end-begin))/CLOCKS_PER_SEC).toAscii());
}

QTEST_MAIN(HealpixTest)
#include "healpixtest.moc"

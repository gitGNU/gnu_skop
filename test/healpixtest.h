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
 * \file healpixtest.h
 * \brief Test for the healpix routines
 */    

#ifndef HEALPIXTEST_H
#define HEALPIXTEST_H

#include <QtTest/QtTest>
#include "../src/sphericalfield.h"
#include "../src/healpixfield.h"
class HealpixTest: public QObject
 {
   Q_OBJECT
private slots:
   void initTestCase();
   void cleanupTestCase();
   void fromFitsTest();
   void serializeTest();
   void loadTest(); 
   void getValueTest();
   void speedTest();
 private:
   SphericalField * f;
};

#endif

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
 * \file fitstest.cpp
 * \brief Test for the fits frontend
 */

#include "fitstest.h"

void FitsTest::initTestCase(){ 
  
}

void FitsTest::cleanupTestCase(){

}

void FitsTest::errorTest(){
  FitsError e(104);
  QCOMPARE(QString(e.getMessage().c_str()),QString("could not open the named file"));
}
void FitsTest::createTest(){
  try{
    f.createNew("test.fits");
    QCOMPARE(f.getNumHDUS(),1);
    f.close();
    QCOMPARE(f.getNumHDUS(),0);
  }catch(FitsError e){
    QFAIL((e.getStatus()+e.getMessage()).c_str());
  }
}
void FitsTest::openTest(){
  try{
    f.open("test.fits");
    QCOMPARE(f.getNumHDUS(),1);
    f.close();
  }catch(FitsError e){
    QFAIL(e.getMessage().c_str());
  }
}
QTEST_MAIN(FitsTest)
#include "fitstest.moc"

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
 * \file fitstest.h
 * \brief Test for the fits frontend
 */    

#ifndef FITSTEST_H
#define FITSTEST_H

#include <QtTest/QtTest>
#include <QObject>
#include "../fitsio/fitserror.h"
#include "../fitsio/fitsfile.h"
class FitsTest: public QObject
 {
   Q_OBJECT
private slots:
   void initTestCase();
   void cleanupTestCase();
   void errorTest();
   void createTest();
   void openTest();
   
 private:
   FitsFile f;
};

#endif

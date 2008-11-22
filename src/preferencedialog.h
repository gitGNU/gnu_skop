// Copyright 2007, 2008 Marc Betoule

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


#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

 #include <QDialog>

 class QListWidget;
 class QListWidgetItem;
 class QStackedWidget;

 class PreferenceDialog : public QDialog
 {
     Q_OBJECT

 public:
     PreferenceDialog(QWidget *parent = 0);

 public slots:
     void changePage(QListWidgetItem *current, QListWidgetItem *previous);

 private:
     void createIcons();

     QListWidget *contentsWidget;
     QStackedWidget *pagesWidget;
 };

 #endif

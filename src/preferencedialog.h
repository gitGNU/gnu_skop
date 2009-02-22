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

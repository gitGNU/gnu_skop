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
 * \file skop.h
 * \brief class definition for the main window of the application.
 */    

#ifndef SKOP_H
#define SKOP_H

#include <QMainWindow>
#include "maplistmodel.h"

class QAction;
//class QListWidget;
class QMenu;
class GLView;
class QListView;
class QItemSelectionModel;


class Skop : public QMainWindow{
    Q_OBJECT
    
public:
    Skop();
signals:
    void mapChanged(const QModelIndex & newmap);

    
private slots:
    void openMap();
    void selectMap(const QModelIndex &current, const QModelIndex &previous);
    void about();
    void captureScreen();


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createModel();
    

    GLView *glview;

    //QListWidget *mapList;
    MapListModel * mapList;
    QListView *listView;
    QItemSelectionModel * selectionModel;
    
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;

    QAction *openMapAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *capture;
};

#endif

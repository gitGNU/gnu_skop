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
 * \file skop.cpp
 * \brief class implementation for the main window of the application.
 */    

#include <QtGui>

#include "skop.h"
#include "glview.h"
#include "skop.moc"
#include "sphericalfield.h"
#include "histodock.h"

//#include "healpixfield.h"
#include <QFileDialog>

Skop::Skop()
 {
     glview = new GLView(this);
     
     setCentralWidget(glview);

     connect(this, SIGNAL(mapChanged(const QModelIndex &)),
	     glview, SLOT(updateShader(const QModelIndex &)));

     createActions();
     createMenus();
     createToolBars();
     createStatusBar();
     createModel();
     createDockWindows();
     
     setWindowTitle(tr("Sky Orthographic Projector"));

     //newLetter();
 }

void Skop::openMap()
{
    QString fn = QFileDialog::getOpenFileName(this, 
					      "Open a new map","Select an healpix map file","Healpix Files (*.fits)");
    if ( !fn.isEmpty() ){
	statusBar()->showMessage( tr("Loading %1").arg(fn));
	SphericalField * hmap = SphericalField::readFits(fn.toAscii().data());
	mapList->insertRows(0,1);
	QModelIndex inserted = mapList->index(0, 0);
	QVariant v;
	v.setValue(hmap);
	mapList->setData(inserted, v,Qt::UserRole);
	mapList->setData(inserted,QString(hmap->name().c_str()));
	mapList->setData(inserted,hmap->getMin(), MinValRole);
	mapList->setData(inserted,hmap->getMax(), MaxValRole);
	//new QListWidgetItem(tr("file %1").arg(fn), mapList);
    }
    
}

void Skop::selectMap(const QModelIndex &current,
		     const QModelIndex &previous)
{
    cout << "ben alors ?\n";
    
    statusBar()->showMessage( tr("%1 selected").arg(current.data().toString()));
    SphericalField * prev = previous.data(Qt::UserRole).value<SphericalField *>();
    if (prev != NULL) prev->clear();
    SphericalField * val = current.data(Qt::UserRole).value<SphericalField *>();
    cout << val<< endl;
    
    
    val->bind();
    //emit mapChanged(current.data(Qt::UserRole+1).toString());
    emit mapChanged(current);
    
}


void Skop::createDockWindows()
 {
     QDockWidget *dock = new QDockWidget(tr("Maps"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     
     listView = new QListView(dock);
     listView->setModel(mapList);
     
     //mapList = new QListWidget(dock);
 //     customerList->addItems(QStringList()
//              << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
//              << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
//              << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
//              << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
//              << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
//              << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
     dock->setWidget(listView);
     addDockWidget(Qt::RightDockWidgetArea, dock);
     viewMenu->addAction(dock->toggleViewAction());

     listView->setIconSize(QSize(32,32));
     

     selectionModel = listView->selectionModel();
     connect(selectionModel, SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)),
	     this, SLOT(selectMap(const QModelIndex &,const QModelIndex &)));
     
     dock = new QDockWidget(tr("Pixel Information"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     addDockWidget(Qt::RightDockWidgetArea, dock);
     viewMenu->addAction(dock->toggleViewAction());

     dock = new QDockWidget(tr("Histogram"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     HistoDock *hd = new HistoDock;
     dock->setWidget(hd);
     addDockWidget(Qt::RightDockWidgetArea, dock);
     connect(hd, SIGNAL(boundChanged(double , double)),
	     glview,SLOT(updateScale(double, double)));
     connect(selectionModel, SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)),
	     hd, SLOT(selectMap(const QModelIndex &,const QModelIndex &)));
     
     viewMenu->addAction(dock->toggleViewAction());

     dock = new QDockWidget(tr("Overlay"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     addDockWidget(Qt::LeftDockWidgetArea, dock);
     viewMenu->addAction(dock->toggleViewAction());
     
 }

void Skop::createModel()
{
    mapList = new MapListModel();
}


void Skop::about()
 {
    QMessageBox::about(this, tr("About Dock Widgets"),
             tr("The <b>Dock Widgets</b> example demonstrates how to "
                "use Qt's dock widgets. You can enter your own text, "
                "click a customer to add a customer name and "
                "address, and click standard paragraphs to add them."));
 }

void Skop::createActions()
 {
     openMapAct = new QAction(tr("&Open"),this);
     openMapAct->setShortcut(tr("Ctrl+N"));
     openMapAct->setStatusTip(tr("Load a map from a fits file"));
     connect(openMapAct, SIGNAL(triggered()), this, SLOT(openMap()));

     quitAct = new QAction(tr("&Quit"), this);
     quitAct->setShortcut(tr("Ctrl+Q"));
     quitAct->setStatusTip(tr("Quit the application"));
     connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

     aboutAct = new QAction(tr("&About"), this);
     aboutAct->setStatusTip(tr("Show the application's About box"));
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

     aboutQtAct = new QAction(tr("About &Qt"), this);
     aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
 }

void Skop::createMenus()
 {
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(openMapAct);
     fileMenu->addSeparator();
     fileMenu->addAction(quitAct);

     viewMenu = menuBar()->addMenu(tr("&View"));

     menuBar()->addSeparator();

     helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutQtAct);
 }

void Skop::createToolBars()
 {
     fileToolBar = addToolBar(tr("File"));
     fileToolBar->addAction(openMapAct);

 }

void Skop::createStatusBar()
 {
     statusBar()->showMessage(tr("Ready"));
 }


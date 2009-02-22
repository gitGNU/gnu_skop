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
 * \file skop.cpp
 * \brief class implementation for the main window of the application.
 */    

#include <QtGui>

#include "skop.h"
#include "glview.h"
#include "skop.moc"
#include "sphericalfield.h"
#include "histodock.h"
#include "pixeldock.h"
#include "preferencedialog.h"
//#include "healpixfield.h"
#include <QFileDialog>
#include "catalog.h"

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
     setAcceptDrops(true);
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
	mapList->setData(inserted,QFileInfo(QString(hmap->name().c_str())).baseName());
	mapList->setData(inserted,hmap->getMin(), MinValRole);
	mapList->setData(inserted,hmap->getMax(), MaxValRole);
	//new QListWidgetItem(tr("file %1").arg(fn), mapList);
    }
    
}

void Skop::openCat()
{
    QString fn = QFileDialog::getOpenFileName(this, 
					      "Open a new catalog","Select a catalog file","text files (*.cat)");
    if ( !fn.isEmpty() ){
	statusBar()->showMessage( tr("Loading %1").arg(fn));
	Catalog * cat = new Catalog();
	cat->read(fn.toAscii().data());
	glview->changeCat(cat);
	cout << cat << "emis\n";
    }
    
}

void Skop::setConfig()
{
  PreferenceDialog dialog(this);
  dialog.exec();
}

void Skop::captureScreen()
{
    QString fn = QFileDialog::getSaveFileName(this, 
					      tr("Save screenshot to"),"~/untitled.png",tr("Images (*.png *.xpm *.jpg)"));
    
    if ( !fn.isEmpty() ){
	statusBar()->showMessage( tr("Saving to %1").arg(fn));
	glview->grabFrameBuffer().save(fn);
    }
    
}


void Skop::selectMap(const QModelIndex &current,
		     const QModelIndex &previous)
{
    
    statusBar()->showMessage( tr("%1 selected").arg(current.data().toString()));
    SphericalField * prev = previous.data(Qt::UserRole).value<SphericalField *>();
    if (prev != NULL) prev->clear();
    SphericalField * val = current.data(Qt::UserRole).value<SphericalField *>();
    
    
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
     PixelDock *pd = new PixelDock(dock);
     dock->setWidget(pd);
     addDockWidget(Qt::RightDockWidgetArea, dock);
     viewMenu->addAction(dock->toggleViewAction());

     dock = new QDockWidget(tr("Histogram"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     HistoDock *hd = new HistoDock(dock,mapList);
     dock->setWidget(hd);
     addDockWidget(Qt::RightDockWidgetArea, dock);
     connect(hd, SIGNAL(boundChanged(double , double)),
	     glview,SLOT(updateScale(double, double)));
     connect(selectionModel, SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)),
	     hd, SLOT(selectMap(const QModelIndex &,const QModelIndex &)));
     connect(selectionModel, SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)),
	     pd, SLOT(selectMap(const QModelIndex &,const QModelIndex &)));
     connect(glview, SIGNAL(pixelSelected(double , double )),
	     pd, SLOT(selectPixel(double, double)));
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
    QMessageBox::about(this, tr("About Skop"),
             tr("The <b>Sky Orthographic Projector</b> is a powerfull tool "
                "to visualize pixelized data on the sphere.\n\n"
		"(c) 2007, 2008 Marc Betoule\n\n"
		"This software is distributed under the term of GPL v3."));
 }

void Skop::createActions()
 {
     openMapAct = new QAction(tr("&Open"),this);
     openMapAct->setShortcut(QKeySequence::Open);
     openMapAct->setStatusTip(tr("Load a map from a fits file"));
     connect(openMapAct, SIGNAL(triggered()), this, SLOT(openMap()));

     openCatAct = new QAction(tr("Open&Cat"),this);
     openCatAct->setStatusTip(tr("Load a catalog from a text file"));
     connect(openCatAct, SIGNAL(triggered()), this, SLOT(openCat()));
     
     capture = new QAction(tr("&capture"),this);
     capture->setShortcut(QKeySequence::Copy);
     capture->setStatusTip(tr("Capture the screen and save to a file"));
     connect(capture, SIGNAL(triggered()), this, SLOT(captureScreen()));

     setupAct = new  QAction(tr("&Settings"),this);
     openMapAct->setShortcut(tr("Ctrl+P"));
     openMapAct->setStatusTip(tr("Change user settings"));
     connect(setupAct, SIGNAL(triggered()), this, SLOT(setConfig()));
     
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
     fileMenu->addAction(openCatAct);
     fileMenu->addAction(setupAct);
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
     fileToolBar->addAction(capture);

 }

void Skop::createStatusBar()
 {
     statusBar()->showMessage(tr("Ready"));
 }

void Skop::dragEnterEvent(QDragEnterEvent *event)
{
  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasText()) {
    QString fn = mimeData->text();
    if (fn.endsWith(".fits"))
      event->acceptProposedAction();
  }
}

void Skop::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
    
}

void Skop::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasText()) {
      QString fn = mimeData->text();    
      if (fn.endsWith(".fits")){
	statusBar()->showMessage( tr("Loading %1").arg(fn));
	SphericalField * hmap = SphericalField::readFits(fn.toAscii().data());
	mapList->insertRows(0,1);
	QModelIndex inserted = mapList->index(0, 0);
	QVariant v;
	v.setValue(hmap);
	mapList->setData(inserted, v,Qt::UserRole);
	mapList->setData(inserted,QFileInfo(QString(hmap->name().c_str())).baseName());
	mapList->setData(inserted,hmap->getMin(), MinValRole);
	mapList->setData(inserted,hmap->getMax(), MaxValRole);
	event->acceptProposedAction();
      }
    } else {
      statusBar()->showMessage( tr("Cannot display data."));
    }
    
}

void Skop::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

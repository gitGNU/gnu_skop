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
 * \file glview.h
 * \brief class definition for the opengl viewer widget.
 */    

#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>

class QWidget;
class Program;

class GLView : public QGLWidget
{
    Q_OBJECT
    
public:
    GLView(QWidget *parent = 0);
    ~GLView();

public slots:
    void updateShader(const QModelIndex & current);
    void updateScale(double minBound, double maxBound);
protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    //void paintGL();
    void wheelEvent ( QWheelEvent * e );
    
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    Program * p;    

private:
    QPoint lastPos;
    void setupViewport(int width, int height);
    QColor trolltechPurple;
    
    double dist;
    double theta0;
    double phi0;
    double minV;
    double maxV;

    int nside;
};

#endif

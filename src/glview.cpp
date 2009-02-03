// Copyright (C) 2007, 2008 Marc Betoule

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
 * \file glview.cpp
 * \brief class implementation for the opengl viewer widget.
 */    

#include "shader.h"

#include <QtGui>
#include <QtOpenGL>
#include <cmath>

#include "glview.h"
#include "glview.moc"
#include "maplistmodel.h"
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLView::GLView(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    theta0 = 0;
    phi0 = 0;
    dist = 1;
    minV = 0;
    maxV = 0;
    rot.eulerZXZ(0,theta0, phi0);
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    setAttribute(Qt::WA_NoSystemBackground);
    setMinimumSize(200, 200);
}

GLView::~GLView()
{
    
}


void GLView::initializeGL()
{
    GlslContext::initGLExtensions();
 
    string vert = "void main()\n{\ngl_TexCoord[0] = gl_MultiTexCoord0;\n  gl_Position = ftransform();\n}";
    string black = "void main(){gl_FragColor=vec4(0.0,0,0,1.0);}";
    p = new Program(vert, black);
    cout << p->getLog();
}

void GLView::updateShader(const QModelIndex &current)
{
    delete p;
    string vert = "void main()\n{\ngl_TexCoord[0] = gl_MultiTexCoord0;\n  gl_Position = ftransform();\n}";
    string fragcode = current.data(ShaderRole).toString().toStdString();
    p = new Program(vert, fragcode);
    cout << p->getLog();
    nside = current.data(NsideRole).toInt();
    nside = sqrt(nside/12);
    update();
    
}

void GLView::updateScale(double minBound, double maxBound)
{
    maxV = maxBound;
    minV = minBound;
    update();
}


void GLView::wheelEvent ( QWheelEvent * e ){

  if(e->delta() > 0){
    dist = dist * 1.1;
  }
  else{
    dist = dist * 0.9;
  }
  update();
}

bool GLView::pixel2sky(int x, int y, double & theta, double & phi){
  double u = (2. * x / width()  - 1.) * dist;
  double v = -(2. * y / height() - 1.) * dist;
  if( u*u+v*v <= 1){
    theta = acos(v);
    phi = asin(u/sin(theta));
    Vec3 coord(theta,phi);
    coord = rot * coord;
    coord.ang(theta,phi);
    return true;
  }
  return false;
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
	lastPos = event->pos();
    else if (event->buttons() & Qt::RightButton) {
      double theta, phi;
      pixel2sky(event->x(), event->y(), theta, phi);
      emit pixelSelected(theta, phi);
    }
    
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton) {
	theta0 += dy * dist / width ();
	phi0 += dx * dist / height ();
	lastPos = event->pos();
	update();
	rot.eulerZYZ(0,theta0,phi0);
    }
    
}


void GLView::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    qglClearColor(trolltechPurple.dark());
    setupViewport(width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p->start();
    p->set1i("tex1",0);
    p->set1i("tex2",1);
    p->set1i("tex3",2);
    p->set1i("nside", nside);
    p->set1f("dist", dist);
    p->set1f("theta0", theta0);
    p->set1f("phi0", phi0);
    p->set1f("minv",minV);
    p->set1f("maxv",maxV);
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);glVertex3f (-1.0, -1.0, -0.5f);
    glTexCoord2f(1,0);glVertex3f (1.0, -1.0, -0.5f);
    glTexCoord2f(1,1);glVertex3f (1.0, 1.0, -0.5f);
    glTexCoord2f(0,1);glVertex3f (-1.0, 1.0, -0.5f);
    glEnd();
    p->stop();
    
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    //painter.drawImage((width() - image.width())/2, 0, image);
    painter.end();
}
/*
void GLView::paintGL()
{
    p->start();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);glVertex3f (-1.0, -1.0, -0.5f);
    glTexCoord2f(1,0);glVertex3f (1.0, -1.0, -0.5f);
    glTexCoord2f(1,1);glVertex3f (1.0, 1.0, -0.5f);
    glTexCoord2f(0,1);glVertex3f (-1.0, 1.0, -0.5f);
    glEnd();
    p->stop();
     
}
*/
void GLView::resizeGL(int width, int height)
{
    setupViewport(width,height);
}

void GLView::setupViewport(int width, int height)
{
    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0,1.0,-1.0,1.0,-1.0,2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

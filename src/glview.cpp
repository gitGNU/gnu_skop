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
 * \file glview.cpp
 * \brief class implementation for the opengl viewer widget.
 */    

#include "shader.h"

#include <QtGui>
#include <QtOpenGL>
#include <cmath>

#include "catalog.h"
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
    selTheta=0;
    selPhi=0;
    selRadius = 10*arcmin2rad;
    showSelectedRegion=false;
    showPolarVector=false;
    showGraticule=false;
    dist = 1;
    minV = 0;
    maxV = 0;
    rot.eulerZXZ(0,theta0, phi0);
    invrot.eulerZXZ(-phi0, -theta0,0);
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    setAttribute(Qt::WA_NoSystemBackground);
    setMinimumSize(200, 200);
    cat = new Catalog();
    graticuleRes = 10*degr2rad;
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

bool GLView::sky2pixel(double theta, double phi, int & x, int & y){
  Vec3 coord(theta,phi);
  coord = invrot * coord;
  double u = coord.a[1];
  double v = coord.a[2];
  x = int(0.5*width() *(u/dist +1));
  y = int(0.5*height() *(-v/dist +1));
  if (coord.a[0] >= 0)
    return true;
  else 
    return false;
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
	lastPos = event->pos();
    else if (event->buttons() & Qt::RightButton) {
      pixel2sky(event->x(), event->y(), selTheta, selPhi);
      emit pixelSelected(selTheta, selPhi);
      if (showSelectedRegion) update();
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
	rot.eulerZYZ(0,theta0,phi0);
	invrot.eulerZYZ(-phi0,-theta0,0);
	update();
    }
    
}


void GLView::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  makeCurrent();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  qglClearColor(trolltechPurple.dark());
  setupViewport(width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  p->start();
  p->set1i("tex1",1);
  p->set1i("tex2",2);
  p->set1i("tex3",3);
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
  
  
  //painter.begin(this);

  if (cat != NULL)
    cat->draw(painter, this);
  //painter.drawImage((width() - image.width())/2, 0, image);
  if (showSelectedRegion){
    QPen pen(Qt::white,1);
    //QFont font("Arial", 10);
    //font.setStyleStrategy(QFont::NoAntialias);
    //painter.setFont(font);
    int x, y,radiusx,radiusy,radius;
    if(sky2pixel(selTheta,selPhi,x,y)){
      sky2pixel(selTheta,selPhi+selRadius,radiusx, radiusy);
      radius = sqrt((radiusx-x)*(radiusx-x)+(radiusy-y)*(radiusy-y));
      radius = radius > 0 ? radius : 1;
      painter.setPen(pen);
      painter.save();
      painter.translate(x - radius, y - radius);
      painter.drawEllipse(0, 0, int(2*radius), int(2*radius));
      //painter.drawText(QPoint(0,0),"Bordel !!");
      painter.restore();
    }
  }
  if (showPolarVector){
    QPen pen(Qt::white,1);
    painter.setPen(pen);
    double theta,phi;
    for(int i=0; i<width();i+=10){
      for(int j=0; j<height();j+=10){
	if(pixel2sky(i, j, theta, phi)){
	  painter.drawLine(i,j,i+5,j);
	}
      }
    }
  }
  if (showGraticule){
    QPen pen(Qt::white,1);
    painter.setPen(pen);
    double theta,phi;
    for(double theta=0; theta<2*M_PI;theta+=graticuleRes){
      for(double phi=0; phi<M_PI;phi+=1*degr2rad){
	int x1,x2,y1,y2;
	if(sky2pixel(theta, phi,x1,y1) | sky2pixel(theta, phi+1*degr2rad,x2,y2)){
	  painter.drawLine(x1,y1,x2,y2);
	}
      }
    }
    for(double phi=0; phi<M_PI;phi+=graticuleRes){
      for(double theta=0; theta<2*M_PI;theta+=1*degr2rad){
	int x1,x2,y1,y2;
	if(sky2pixel(theta, phi,x1,y1) | sky2pixel(theta+1*degr2rad, phi,x2,y2)){
	  painter.drawLine(x1,y1,x2,y2);
	}
      }
    }
  }
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

void GLView::changeCat(Catalog * _cat){
  cat = _cat;
  cout << "change cat\n";
}

void GLView::writeCat(string filename){
  cat->write(filename);
}

void GLView::changeSelRadius(double radius){
  selRadius = radius*arcmin2rad;
  if (showSelectedRegion) update();
}

void GLView::changeSel(int state){
  if (state == Qt::Checked) showSelectedRegion = true;
  else showSelectedRegion = false;
  update();
}

void GLView::changeGrat(int state){
  if (state == Qt::Checked) showGraticule = true;
  else showGraticule = false;
  update();
}

void GLView::changeGratRes(int value){
  graticuleRes = value*degr2rad;
  update();
}

void GLView::addSource(){
  bool ok;
  QString text = QInputDialog::getText(this, tr("Add a new source"),
				       tr("Source name:"), QLineEdit::Normal,
				       "", &ok);
  if (ok && !text.isEmpty()){
    cat->add(Source(selTheta,selPhi, selRadius, text));
    update();
  }
  
}

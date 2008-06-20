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
 * \file maplistmodel.cpp
 * \brief implementation of a simple list Model for managing access to pixelized
 * in the qt model/view framework way.
 */

#include "maplistmodel.h"
#include "maplistmodel.moc"
#include "sphericalfield.h"
#include "toolbox.h"
#include <QImage>
#include <cmath> 

QVariant MapInfo::get(int role) const
{
  if (role == Qt::DisplayRole){
    return QString::fromStdString(name);
  }
  if (role == Qt::DecorationRole){
    int imsize = 64;
    QImage im(imsize,imsize,QImage::Format_ARGB32);
    for(int i = 0; i<imsize ; i++){
      for(int j = 0; j<imsize ; j++){
	double u,v;
	u = (i-imsize*0.5)/(imsize * .5);
	v = (j-imsize*0.5)/(imsize * .5);
	if (u*u+v*v >=1)
	  im.setPixel(i,j,qRgba ( 0, 0, 0, 0 ));
	else{
	  float pixelval = pointer->getValue(acos(v),  asin(u/sin(acos(v))));
	  float rescaled = log(1+pixelval - minValue)/log(1+maxValue-minValue);
	  im.setPixel(i,j,jet(rescaled, 255 ));
	  //im.setPixel(i,j,qRgba(rescaled*255, rescaled*255,rescaled*255,255 ));
	}
      }
      
    }
    return im;
  }
  if (role == PointerRole){
    QVariant v;
    v.setValue(pointer);
    return v;
  }
  if (role == ShaderRole){
    return QString::fromStdString(pointer->getShader());
  }
  if (role == NsideRole){
    return QVariant(pointer->getNPix());
  }
  if (role == MinValRole){
    return QVariant(minValue);
  }
  if (role == MaxValRole){
    return QVariant(maxValue);
  }
  if (role == MinScaleRole){
    return QVariant(minScale);
  }
  if (role == MaxScaleRole){
    return QVariant(maxScale);
  }
  if (role == UnitRole){
    return QString::fromStdString(pointer->getUnit());
  }
  else
    return QVariant();    
}

bool MapInfo::edit(const QVariant &value, int role)
{
  if (role == Qt::EditRole) {
    name = value.toString().toStdString();
    return true;
  }
  if (role == PointerRole){
    pointer = value.value<SphericalField *>();
    if(pointer != 0){
      minValue = minScale = pointer->getMin();
      maxValue = maxScale = pointer->getMax();
      cout << minValue << "," << maxValue << endl;
    }
    cout << "Now pointing at : "<< pointer << endl;
    return true;
  }
  if (role == MinScaleRole){
    minScale = value.toDouble();
    return true;
  }
  if (role == MaxScaleRole){
    maxScale = value.toDouble();
    return true;
  }
  else return false;
}

int MapListModel::rowCount(const QModelIndex &parent) const
{
    return mapList.count();    
}

QVariant MapListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
	return QVariant();
    if (index.row() >= mapList.size())
	return QVariant();
    else
      return mapList.at(index.row()).get(role);
}

QVariant MapListModel::headerData(int section, Qt::Orientation orientation,
				     int role) const
{
    if (role != Qt::DisplayRole)
	return QVariant();
    if (orientation == Qt::Horizontal)
	return QString("Column %1").arg(section);
    else
	return QString("Row %1").arg(section);
}

Qt::ItemFlags MapListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
	return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;    
}

bool MapListModel::setData(const QModelIndex &index,
			   const QVariant &value, int role)
{
    if (index.isValid()) {
      if(mapList[index.row()].edit(value, role)){
	emit dataChanged(index, index);
	return true;
      }
      else{
	return false;
      }
    }
    return false;    
}


bool MapListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
      mapList.insert(position, MapInfo());
    }
    endInsertRows();
    return true;
}

bool MapListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
	mapList.removeAt(position);
    }
    endRemoveRows();
    return true;
}

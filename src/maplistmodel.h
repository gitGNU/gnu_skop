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
 * \file maplistmodel.h
 * \brief definition of a simple list Model for managing access to pixelized
 * in the qt model/view framework way.
 */


#ifndef MAPLISTMODEL_H
#define MAPLISTMODEL_H

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QVariant>

//class SphericalField;
#include "sphericalfield.h"
Q_DECLARE_METATYPE(SphericalField *)

enum Roles {PointerRole = Qt::UserRole, MinValRole, MaxValRole, MinScaleRole,MaxScaleRole,ThetaRole,PhiRole,ShaderRole,NsideRole};

class MapInfo
{
public:
  
  bool edit(const QVariant &value, int role);
  QVariant get(int role) const;
  
  float minValue;
  float maxValue;
  float minScale;
  float maxScale;
  string name;
  double theta;
  double phi;
  SphericalField * pointer;
};

class MapListModel : public QAbstractListModel
{
    Q_OBJECT
public:
  MapListModel(QObject *parent = 0)
      : QAbstractListModel(parent), mapList() 
  {
  }
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value,
	       int role = Qt::EditRole);
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
private:
  QList<MapInfo> mapList;
};


#endif

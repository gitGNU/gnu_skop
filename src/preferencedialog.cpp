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


#include <QtGui>

#include "preferencedialog.h"
#include "preferencedialog.moc"
#include "pages.h"

PreferenceDialog::PreferenceDialog(QWidget *parent)
  :QDialog(parent)
{
  
  contentsWidget = new QListWidget;
  contentsWidget->setViewMode(QListView::IconMode);
  contentsWidget->setIconSize(QSize(96, 84));
  contentsWidget->setMovement(QListView::Static);
  contentsWidget->setMaximumWidth(128);
  contentsWidget->setSpacing(12);

  pagesWidget = new QStackedWidget;
  pagesWidget->addWidget(new ConfigurationPage);
  pagesWidget->addWidget(new UpdatePage);
  pagesWidget->addWidget(new QueryPage);

  QPushButton *closeButton = new QPushButton(tr("Close"));
  
  createIcons();
  contentsWidget->setCurrentRow(0);
  
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  
  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(contentsWidget);
  horizontalLayout->addWidget(pagesWidget, 1);
  
  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(closeButton);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addStretch(1);
  mainLayout->addSpacing(12);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);
  
  setWindowTitle(tr("User settings"));
}

 void PreferenceDialog::createIcons()
 {
     QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
     //configButton->setIcon(QIcon(":/images/config.png"));
     configButton->setText(tr("Configuration"));
     configButton->setTextAlignment(Qt::AlignHCenter);
     configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

     QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
     //updateButton->setIcon(QIcon(":/images/update.png"));
     updateButton->setText(tr("Update"));
     updateButton->setTextAlignment(Qt::AlignHCenter);
     updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

     QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
     //queryButton->setIcon(QIcon(":/images/query.png"));
     queryButton->setText(tr("Query"));
     queryButton->setTextAlignment(Qt::AlignHCenter);
     queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

     connect(contentsWidget,
             SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
             this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
 }

 void PreferenceDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
 {
     if (!current)
         current = previous;

     pagesWidget->setCurrentIndex(contentsWidget->row(current));
 }

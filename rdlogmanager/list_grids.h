// list_grids.h
//
// List Rivendell Log Grids
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: list_grids.h,v 1.3 2007/02/14 21:57:04 fredg Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef LIST_GRIDS_H
#define LIST_GRIDS_H

#include <qdialog.h>
#include <qsqldatabase.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlistview.h>

#include <rduser.h>
#include <rdmatrix.h>


class ListGrids : public QDialog
{
 Q_OBJECT
 public:
  ListGrids(QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void editData();
  void doubleClickedData(QListViewItem *,const QPoint &,int);
  void closeData();

 private:
  void RefreshList();
  QListView *edit_grids_list;
};


#endif


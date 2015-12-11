// edit_svc_perms.h
//
// Edit Rivendell Service Permissions
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_svc_perms.h,v 1.4 2007/09/14 14:06:27 fredg Exp $
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

#ifndef EDIT_SVC_PERMS_H
#define EDIT_SVC_PERMS_H

#include <qdialog.h>
#include <qsqldatabase.h>

#include <rdlistselector.h>

#include <rdsvc.h>


class EditSvcPerms : public QDialog
{
  Q_OBJECT
 public:
  EditSvcPerms(RDSvc *svc,QWidget *parent=0,const char *name=0);
  ~EditSvcPerms();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void okData();
  void cancelData();

 private:
  RDListSelector *svc_host_sel;
  RDSvc *svc_svc;
};


#endif


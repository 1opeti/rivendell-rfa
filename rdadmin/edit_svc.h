// edit_svc.h
//
// Edit a Rivendell Service
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_svc.h,v 1.21.2.2 2008/12/08 19:00:24 fredg Exp $
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

#ifndef EDIT_SVC_H
#define EDIT_SVC_H

#include <qdialog.h>
#include <qsqldatabase.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>

#include <rdsvc.h>


class EditSvc : public QDialog
{
 Q_OBJECT
 public:
  EditSvc(QString svc,QWidget *parent=0,const char *name=0);
  ~EditSvc();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void autofillData();
  void enableHostsData();
  void trafficData();
  void musicData();
  void textChangedData(const QString &);
  void valueChangedData(int);
  void okData();
  void cancelData();

 private:
  void TestDataImport(RDSvc::ImportSource src);
  void Save();
  RDSvc *svc_svc;
  QLineEdit *svc_name_edit;
  QLineEdit *svc_description_edit;
  QLineEdit *svc_template_edit;
  QComboBox *svc_voice_group_box;
  QLineEdit *svc_tfc_path_edit;
  QLineEdit *svc_tfc_win_path_edit;
  QLineEdit *svc_tfc_label_cart_edit;
  QLineEdit *svc_tfc_track_edit;
  QSpinBox *svc_tfccart_offset_spin;
  QSpinBox *svc_tfccart_length_spin;
  QSpinBox *svc_tfctitle_offset_spin;
  QSpinBox *svc_tfctitle_length_spin;
  QSpinBox *svc_tfchours_offset_spin;
  QSpinBox *svc_tfchours_length_spin;
  QSpinBox *svc_tfcminutes_offset_spin;
  QSpinBox *svc_tfcminutes_length_spin;
  QSpinBox *svc_tfcseconds_offset_spin;
  QSpinBox *svc_tfcseconds_length_spin;
  QSpinBox *svc_tfclenhours_offset_spin;
  QSpinBox *svc_tfclenhours_length_spin;
  QSpinBox *svc_tfclenminutes_offset_spin;
  QSpinBox *svc_tfclenminutes_length_spin;
  QSpinBox *svc_tfclenseconds_offset_spin;
  QSpinBox *svc_tfclenseconds_length_spin;
  QSpinBox *svc_tfcannctype_offset_spin;
  QSpinBox *svc_tfcannctype_length_spin;
  QSpinBox *svc_tfcdata_offset_spin;
  QSpinBox *svc_tfcdata_length_spin;
  QSpinBox *svc_tfceventid_offset_spin;
  QSpinBox *svc_tfceventid_length_spin;
  QLineEdit *svc_mus_path_edit;
  QLineEdit *svc_mus_win_path_edit;
  QLineEdit *svc_mus_label_cart_edit;
  QLineEdit *svc_mus_break_edit;
  QLineEdit *svc_mus_track_edit;
  QSpinBox *svc_muscart_offset_spin;
  QSpinBox *svc_muscart_length_spin;
  QSpinBox *svc_mustitle_offset_spin;
  QSpinBox *svc_mustitle_length_spin;
  QSpinBox *svc_mushours_offset_spin;
  QSpinBox *svc_mushours_length_spin;
  QSpinBox *svc_musminutes_offset_spin;
  QSpinBox *svc_musminutes_length_spin;
  QSpinBox *svc_musseconds_offset_spin;
  QSpinBox *svc_musseconds_length_spin;
  QSpinBox *svc_muslenhours_offset_spin;
  QSpinBox *svc_muslenhours_length_spin;
  QSpinBox *svc_muslenminutes_offset_spin;
  QSpinBox *svc_muslenminutes_length_spin;
  QSpinBox *svc_muslenseconds_offset_spin;
  QSpinBox *svc_muslenseconds_length_spin;
  QSpinBox *svc_musannctype_offset_spin;
  QSpinBox *svc_musannctype_length_spin;
  QSpinBox *svc_musdata_offset_spin;
  QSpinBox *svc_musdata_length_spin;
  QSpinBox *svc_museventid_offset_spin;
  QSpinBox *svc_museventid_length_spin;
  QCheckBox *svc_chain_box;
  QCheckBox *svc_autorefresh_box;
  QCheckBox *svc_loglife_box;
  QSpinBox *svc_loglife_spin;
  QCheckBox *svc_shelflife_box;
  QSpinBox *svc_shelflife_spin;
  bool import_changed;
};


#endif


// edit_svc.cpp
//
// Edit a Rivendell Service
//
//   (C) Copyright 2002-2004,2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_svc.cpp,v 1.40.2.2 2008/12/08 19:00:24 fredg Exp $
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

#include <qdialog.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

#include <rddb.h>
#include <rd.h>
#include <rduser.h>
#include <rdpasswd.h>
#include <rdtextvalidator.h>

#include <edit_svc.h>
#include <test_import.h>
#include <autofill_carts.h>
#include <edit_svc_perms.h>


EditSvc::EditSvc(QString svc,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  RDSqlQuery *q;
  QString group;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  svc_svc=new RDSvc(svc);

  setCaption(tr("Edit Service"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont section_font=QFont("Helvetica",14,QFont::Bold);
  section_font.setPixelSize(14);

  //
  // Text Validators
  //
  RDTextValidator *validator=new RDTextValidator(this,"validator");
  RDTextValidator *log_validator=new RDTextValidator(this,"validator");
  log_validator->addBannedChar('-');
  log_validator->addBannedChar('!');
  log_validator->addBannedChar('@');
  log_validator->addBannedChar('#');
  log_validator->addBannedChar('$');
  log_validator->addBannedChar('^');
  log_validator->addBannedChar('&');
  log_validator->addBannedChar('*');
  log_validator->addBannedChar('(');
  log_validator->addBannedChar(')');
  log_validator->addBannedChar('[');
  log_validator->addBannedChar(']');
  log_validator->addBannedChar('{');
  log_validator->addBannedChar('}');
  log_validator->addBannedChar('+');
  log_validator->addBannedChar('=');
  log_validator->addBannedChar('\\');
  log_validator->addBannedChar('|');
  log_validator->addBannedChar('?');
  log_validator->addBannedChar(';');
  log_validator->addBannedChar(':');
  log_validator->addBannedChar('.');
  log_validator->addBannedChar('<');
  log_validator->addBannedChar('>');
  log_validator->addBannedChar(',');
  log_validator->addBannedChar('/');

  //
  // General Section
  //
  QLabel *label=new QLabel("General",this,"traffic_import_label");
  label->setGeometry(10,10,120,24);
  label->setFont(section_font);
  label->setAlignment(AlignLeft);

  //
  // Service Name
  //
  svc_name_edit=new QLineEdit(this,"svc_name_edit");
  svc_name_edit->setGeometry(155,31,80,19);
  svc_name_edit->setMaxLength(10);
  svc_name_edit->setReadOnly(true);
  label=new QLabel(svc_name_edit,tr("&Service Name:"),this,
			   "svc_name_label");
  label->setGeometry(10,33,140,19);
  label->setAlignment(AlignRight|ShowPrefix);

  //
  // Service Description
  //
  svc_description_edit=new QLineEdit(this,"svc_description_edit");
  svc_description_edit->setGeometry(155,52,270,19);
  svc_description_edit->setMaxLength(255);
  svc_description_edit->setValidator(validator);
  label=new QLabel(svc_description_edit,
		   tr("Service &Description:"),this,
		   "svc_description_label");
  label->setGeometry(10,54,140,19);
  label->setAlignment(AlignRight|ShowPrefix);

  //
  // Service Template
  //
  svc_template_edit=new QLineEdit(this,"svc_template_edit");
  svc_template_edit->setGeometry(155,73,270,19);
  svc_template_edit->setMaxLength(255);
  svc_template_edit->setValidator(log_validator);
  label=new QLabel(svc_template_edit,
		   tr("Log &Template Name:"),this,
		   "svc_template_label");
  label->setGeometry(10,75,140,19);
  label->setAlignment(AlignRight|ShowPrefix);

  //
  // Voicetracking Group
  //
  svc_voice_group_box=new QComboBox(this,"svc_voice_group_group_box");
  svc_voice_group_box->setGeometry(155,94,270,19);
  svc_voice_group_box->insertItem(tr("[none]"));
  label=new QLabel(svc_voice_group_box,tr("Voicetrack Group:"),this,
		   "svc_voice_group_label");
  label->setGeometry(10,94,140,19);
  label->setAlignment(AlignRight|ShowPrefix);

  //
  // Chain Log
  //
  svc_chain_box=new QCheckBox(this,"svc_chain_box");
  svc_chain_box->setGeometry(30,117,15,15);
  label=new QLabel(svc_chain_box,tr("Insert CHAIN TO at log end"),
		   this,"svc_chain_label");
  label->setGeometry(50,117,170,19);
  label->setAlignment(AlignLeft|ShowPrefix);

  //
  // Default Auto Refresh
  //
  svc_autorefresh_box=new QCheckBox(this,"svc_autorefresh_box");
  svc_autorefresh_box->setGeometry(230,117,15,15);
  label=new QLabel(svc_autorefresh_box,tr("Enable AutoRefresh By Default"),
		   this,"svc_autorefresh_label");
  label->setGeometry(250,117,200,19);
  label->setAlignment(AlignLeft|ShowPrefix);

  //
  // Autofill Button
  //
  QPushButton *button=new QPushButton(this,"autofill_button");
  button->setGeometry(455,31,150,50);
  button->setFont(font);
  button->setText(tr("Configure \n&Autofill Carts"));
  connect(button,SIGNAL(clicked()),this,SLOT(autofillData()));

  //
  // Purge Expired Logs
  //
  svc_loglife_box=new QCheckBox(this,"svc_loglife_box");
  svc_loglife_box->setGeometry(460,95,15,15);
  label=new QLabel(svc_loglife_box,tr("Purge Logs after"),
		   this,"svc_loglife_label");
  label->setGeometry(480,95,200,19);
  label->setAlignment(AlignLeft|ShowPrefix);
  svc_loglife_spin=new QSpinBox(this,"svc_loglife_spin");
  svc_loglife_spin->setGeometry(585,93,50,19);
  svc_loglife_spin->setRange(0,365);
  connect(svc_loglife_box,SIGNAL(toggled(bool)),
	  svc_loglife_spin,SLOT(setEnabled(bool)));
  label=new QLabel(svc_loglife_box,tr("days"),
		   this,"svc_loglife_unit");
  label->setGeometry(645,95,40,19);
  label->setAlignment(AlignLeft|ShowPrefix);

  //
  // Purge Expired ELR Data
  //
  svc_shelflife_box=new QCheckBox(this,"svc_shelflife_box");
  svc_shelflife_box->setGeometry(460,117,15,15);
  label=new QLabel(svc_shelflife_box,tr("Purge ELR Data after"),
		   this,"svc_shelflife_label");
  label->setGeometry(480,117,200,19);
  label->setAlignment(AlignLeft|ShowPrefix);
  svc_shelflife_spin=new QSpinBox(this,"svc_shelflife_spin");
  svc_shelflife_spin->setGeometry(610,115,50,19);
  svc_shelflife_spin->setRange(0,365);
  connect(svc_shelflife_box,SIGNAL(toggled(bool)),
	  svc_shelflife_spin,SLOT(setEnabled(bool)));
  label=new QLabel(svc_shelflife_box,tr("days"),
		   this,"svc_shelflife_unit");
  label->setGeometry(670,117,40,19);
  label->setAlignment(AlignLeft|ShowPrefix);

  //
  // Enable Hosts Button
  //
  button=new QPushButton(this,"hosts_button");
  button->setGeometry(625,31,150,50);
  button->setFont(font);
  button->setText(tr("Enable &Hosts"));
  connect(button,SIGNAL(clicked()),this,SLOT(enableHostsData()));

  //
  // Traffic Import Section
  //
  label=new QLabel(tr("Traffic Data Import"),this,"traffic_import_label");
  label->setGeometry(10,150,160,24);
  label->setFont(section_font);
  label->setAlignment(AlignLeft);

  //
  // Linux Traffic Import Path
  //
  svc_tfc_path_edit=new QLineEdit(this,"svc_tfc_path_edit");
  svc_tfc_path_edit->setGeometry(185,171,240,19);
  svc_tfc_path_edit->setMaxLength(255);
  svc_tfc_path_edit->setValidator(validator);
  label=new QLabel(svc_tfc_path_edit,
		   tr("Linux Import Path:"),this,
		   "svc_tfc_path_label");
  label->setGeometry(10,171,170,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfc_path_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Windows Traffic Import Path
  //
  svc_tfc_win_path_edit=new QLineEdit(this,"svc_tfc_win_path_edit");
  svc_tfc_win_path_edit->setGeometry(185,192,240,19);
  svc_tfc_win_path_edit->setMaxLength(255);
  label=new QLabel(svc_tfc_win_path_edit,
		   tr("Windows Import Path:"),this,
		   "svc_tfc_win_path_label");
  label->setGeometry(10,192,170,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfc_win_path_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Label Cart
  //
  svc_tfc_label_cart_edit=new QLineEdit(this,"svc_tfc_label_cart_edit");
  svc_tfc_label_cart_edit->setGeometry(185,213,240,19);
  svc_tfc_label_cart_edit->setMaxLength(32);
  label=new QLabel(svc_tfc_label_cart_edit,
		   tr("Note Cart String:"),this,
		   "svc_tfc_label_cart_label");
  label->setGeometry(10,213,170,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfc_label_cart_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Track String
  //
  svc_tfc_track_edit=new QLineEdit(this,"svc_tfc_track_edit");
  svc_tfc_track_edit->setGeometry(185,234,240,19);
  svc_tfc_track_edit->setMaxLength(32);
  label=new QLabel(svc_tfc_track_edit,
		   tr("Insert Voice Track String:"),this,
		   "svc_tfc_track_cart_track");
  label->setGeometry(10,234,170,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfc_track_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Traffic Cartname Parser Data Section
  //
  label=new QLabel(tr("Cart Number:"),this,
		   "svc_tfc_cartname_label");
  label->setGeometry(10,255,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Cart Offset
  //
  svc_tfccart_offset_spin=new QSpinBox(this,"svc_tfccart_offset_spin");
  svc_tfccart_offset_spin->setGeometry(185,255,50,19);
  svc_tfccart_offset_spin->setRange(0,1024);
  label=new QLabel(svc_tfccart_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfccart_win_path_label");
  label->setGeometry(135,255,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfccart_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Cart Length
  //
  svc_tfccart_length_spin=new QSpinBox(this,"svc_tfccart_length_spin");
  svc_tfccart_length_spin->setGeometry(295,255,50,19);
  svc_tfccart_length_spin->setRange(0,6);
  label=new QLabel(svc_tfccart_length_spin,
		   tr("Length:"),this,
		   "svc_tfccart_win_path_label");
  label->setGeometry(240,255,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfccart_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Title Parser Data Section
  //
  label=new QLabel(tr("Title:"),this,"svc_tfc_cartname_label");
  label->setGeometry(10,276,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Title Cart Offset
  //
  svc_tfctitle_offset_spin=new QSpinBox(this,"svc_tfctitle_offset_spin");
  svc_tfctitle_offset_spin->setGeometry(185,276,50,19);
  svc_tfctitle_offset_spin->setRange(0,1024);
  label=new QLabel(svc_tfctitle_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfctitle_win_path_label");
  label->setGeometry(135,276,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfctitle_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Title Cart Length
  //
  svc_tfctitle_length_spin=new QSpinBox(this,"svc_tfctitle_length_spin");
  svc_tfctitle_length_spin->setGeometry(295,276,50,19);
  svc_tfctitle_length_spin->setRange(0,255);
  label=new QLabel(svc_tfctitle_length_spin,
		   tr("Length:"),this,
		   "svc_tfctitle_win_path_label");
  label->setGeometry(240,276,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfctitle_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Hours Parser Data Section
  //
  label=new QLabel(tr("Start Time - Hours:"),this,
		   "svc_tfc_startname_label");
  label->setGeometry(10,297,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Start Hours Offset
  //
  svc_tfchours_offset_spin=new QSpinBox(this,"svc_tfchours_offset_spin");
  svc_tfchours_offset_spin->setGeometry(185,297,50,19);
  svc_tfchours_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfchours_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfchours_win_path_label");
  label->setGeometry(135,297,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfchours_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Hours Length
  //
  svc_tfchours_length_spin=new QSpinBox(this,"svc_tfchours_length_spin");
  svc_tfchours_length_spin->setGeometry(295,297,50,19);
  svc_tfchours_length_spin->setRange(0,8);
  label=new QLabel(svc_tfchours_length_spin,
		   tr("Length:"),this,
		   "svc_tfchours_win_path_label");
  label->setGeometry(240,297,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfchours_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Minutes Parser Data Section
  //
  label=new QLabel(tr("Start Time - Minutes:"),this,
		   "svc_tfc_startname_label");
  label->setGeometry(10,318,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Start Minutes Offset
  //
  svc_tfcminutes_offset_spin=new QSpinBox(this,"svc_tfcminutes_offset_spin");
  svc_tfcminutes_offset_spin->setGeometry(185,318,50,19);
  svc_tfcminutes_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfcminutes_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfcminutes_win_path_label");
  label->setGeometry(135,318,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcminutes_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Minutes Length
  //
  svc_tfcminutes_length_spin=new QSpinBox(this,"svc_tfcminutes_length_spin");
  svc_tfcminutes_length_spin->setGeometry(295,318,50,19);
  svc_tfcminutes_length_spin->setRange(0,8);
  label=new QLabel(svc_tfcminutes_length_spin,
		   tr("Length:"),this,
		   "svc_tfcminutes_win_path_label");
  label->setGeometry(240,318,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcminutes_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Seconds Parser Data Section
  //
  label=new QLabel(tr("Start Time - Seconds:"),this,
		   "svc_tfc_startname_label");
  label->setGeometry(10,339,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Start Seconds Offset
  //
  svc_tfcseconds_offset_spin=new QSpinBox(this,"svc_tfcseconds_offset_spin");
  svc_tfcseconds_offset_spin->setGeometry(185,339,50,19);
  svc_tfcseconds_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfcseconds_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfcseconds_win_path_label");
  label->setGeometry(135,339,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcseconds_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Start Seconds Length
  //
  svc_tfcseconds_length_spin=new QSpinBox(this,"svc_tfcseconds_length_spin");
  svc_tfcseconds_length_spin->setGeometry(295,339,50,19);
  svc_tfcseconds_length_spin->setRange(0,8);
  label=new QLabel(svc_tfcseconds_length_spin,
		   tr("Length:"),this,
		   "svc_tfcseconds_win_path_label");
  label->setGeometry(240,339,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcseconds_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Hours Parser Data Section
  //
  label=new QLabel(tr("Length - Hours:"),this,
		   "svc_tfc_lengthname_label");
  label->setGeometry(10,360,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Length Hours Offset
  //
  svc_tfclenhours_offset_spin=new QSpinBox(this,"svc_tfclenhours_offset_spin");
  svc_tfclenhours_offset_spin->setGeometry(185,360,50,19);
  svc_tfclenhours_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfclenhours_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfclenhours_win_path_label");
  label->setGeometry(135,360,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenhours_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Hours Length
  //
  svc_tfclenhours_length_spin=new QSpinBox(this,"svc_tfclenhours_length_spin");
  svc_tfclenhours_length_spin->setGeometry(295,360,50,19);
  svc_tfclenhours_length_spin->setRange(0,8);
  label=new QLabel(svc_tfclenhours_length_spin,
		   tr("Length:"),this,
		   "svc_tfclenhours_win_path_label");
  label->setGeometry(240,360,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenhours_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Minutes Parser Data Section
  //
  label=new QLabel(tr("Length - Minutes:"),this,
		   "svc_tfc_lengthname_label");
  label->setGeometry(10,381,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Length Minutes Offset
  //
  svc_tfclenminutes_offset_spin=new QSpinBox(this,"svc_tfclenminutes_offset_spin");
  svc_tfclenminutes_offset_spin->setGeometry(185,381,50,19);
  svc_tfclenminutes_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfclenminutes_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfclenminutes_win_path_label");
  label->setGeometry(135,381,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenminutes_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Minutes Length
  //
  svc_tfclenminutes_length_spin=new QSpinBox(this,"svc_tfclenminutes_length_spin");
  svc_tfclenminutes_length_spin->setGeometry(295,381,50,19);
  svc_tfclenminutes_length_spin->setRange(0,8);
  label=new QLabel(svc_tfclenminutes_length_spin,
		   tr("Length:"),this,
		   "svc_tfclenminutes_win_path_label");
  label->setGeometry(240,381,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenminutes_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Seconds Parser Data Section
  //
  label=new QLabel(tr("Length - Seconds:"),this,
		   "svc_tfc_lengthname_label");
  label->setGeometry(10,402,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Length Seconds Offset
  //
  svc_tfclenseconds_offset_spin=new QSpinBox(this,"svc_tfclenseconds_offset_spin");
  svc_tfclenseconds_offset_spin->setGeometry(185,402,50,19);
  svc_tfclenseconds_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfclenseconds_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfclenseconds_win_path_label");
  label->setGeometry(135,402,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenseconds_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Length Seconds Length
  //
  svc_tfclenseconds_length_spin=new QSpinBox(this,"svc_tfclenseconds_length_spin");
  svc_tfclenseconds_length_spin->setGeometry(295,402,50,19);
  svc_tfclenseconds_length_spin->setRange(0,8);
  label=new QLabel(svc_tfclenseconds_length_spin,
		   tr("Length:"),this,
		   "svc_tfclenseconds_win_path_label");
  label->setGeometry(240,402,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfclenseconds_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Contract # Parser Data Section
  //
  label=new QLabel(tr("Contract #:"),this,
		   "svc_tfc_dataname_label");
  label->setGeometry(10,423,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Contract # Offset
  //
  svc_tfcdata_offset_spin=new QSpinBox(this,"svc_tfcdata_offset_spin");
  svc_tfcdata_offset_spin->setGeometry(185,423,50,19);
  svc_tfcdata_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfcdata_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfcdata_win_path_label");
  label->setGeometry(135,423,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcdata_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Contract # Length
  //
  svc_tfcdata_length_spin=new QSpinBox(this,"svc_tfcdata_length_spin");
  svc_tfcdata_length_spin->setGeometry(295,423,50,19);
  svc_tfcdata_length_spin->setRange(0,32);
  label=new QLabel(svc_tfcdata_length_spin,
		   tr("Length:"),this,
		   "svc_tfcdata_win_path_label");
  label->setGeometry(240,423,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcdata_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Event ID Parser Data Section
  //
  label=new QLabel(tr("Event ID:"),this,
		   "svc_tfc_eventidname_label");
  label->setGeometry(10,444,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Event ID Offset
  //
  svc_tfceventid_offset_spin=new QSpinBox(this,"svc_tfceventid_offset_spin");
  svc_tfceventid_offset_spin->setGeometry(185,444,50,19);
  svc_tfceventid_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfceventid_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfceventid_win_path_label");
  label->setGeometry(135,444,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfceventid_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Event ID Length
  //
  svc_tfceventid_length_spin=new QSpinBox(this,"svc_tfceventid_length_spin");
  svc_tfceventid_length_spin->setGeometry(295,444,50,19);
  svc_tfceventid_length_spin->setRange(0,8);
  label=new QLabel(svc_tfceventid_length_spin,
		   tr("Length:"),this,
		   "svc_tfceventid_win_path_label");
  label->setGeometry(240,444,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfceventid_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Announcement Type Parser Data Section
  //
  label=new QLabel(tr("Annc. Type:"),this,
		   "svc_tfc_annctypename_label");
  label->setGeometry(10,465,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Traffic Announcement Type Offset
  //
  svc_tfcannctype_offset_spin=new QSpinBox(this,"svc_tfcannctype_offset_spin");
  svc_tfcannctype_offset_spin->setGeometry(185,465,50,19);
  svc_tfcannctype_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_tfcannctype_offset_spin,
		   tr("Offset:"),this,
		   "svc_tfcannctype_win_path_label");
  label->setGeometry(135,465,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcannctype_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Traffic Announcement Type Length
  //
  svc_tfcannctype_length_spin=new QSpinBox(this,"svc_tfcannctype_length_spin");
  svc_tfcannctype_length_spin->setGeometry(295,465,50,19);
  svc_tfcannctype_length_spin->setRange(0,8);
  label=new QLabel(svc_tfcannctype_length_spin,
		   tr("Length:"),this,
		   "svc_tfcannctype_win_path_label");
  label->setGeometry(240,465,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_tfcannctype_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  //  Traffic Test Button
  //
  button=new QPushButton(this,"tfc_button");
  button->setGeometry(360,255,60,40);
  button->setFont(font);
  button->setText(tr("Test \n&Traffic"));
  connect(button,SIGNAL(clicked()),this,SLOT(trafficData()));

  //
  // Music Import Section
  //
  label=new QLabel(tr("Music Data Import"),this,"music_import_label");
  label->setGeometry(445,150,160,24);
  label->setFont(section_font);
  label->setAlignment(AlignLeft);

  //
  // Linux Music Import Path
  //
  svc_mus_path_edit=new QLineEdit(this,"svc_mus_path_edit");
  svc_mus_path_edit->setGeometry(620,171,240,19);
  svc_mus_path_edit->setMaxLength(255);
  svc_mus_path_edit->setValidator(validator);
  label=new QLabel(svc_mus_path_edit,
		   tr("Linux Import Path:"),this,
		   "svc_mus_path_label");
  label->setGeometry(450,171,165,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mus_path_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Windows Music Import Path
  //
  svc_mus_win_path_edit=new QLineEdit(this,"svc_mus_win_path_edit");
  svc_mus_win_path_edit->setGeometry(620,192,240,19);
  svc_mus_win_path_edit->setMaxLength(255);
  label=new QLabel(svc_mus_win_path_edit,
		   tr("Windows Import Path:"),this,
		   "svc_mus_win_path_label");
  label->setGeometry(450,192,165,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mus_win_path_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Label Cart
  //
  svc_mus_label_cart_edit=new QLineEdit(this,"svc_mus_label_cart_edit");
  svc_mus_label_cart_edit->setGeometry(620,213,240,19);
  svc_mus_label_cart_edit->setMaxLength(32);
  label=new QLabel(svc_mus_label_cart_edit,
		   tr("Note Cart String:"),this,
		   "svc_mus_label_cart_label");
  label->setGeometry(450,213,170,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mus_label_cart_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Insert Voice Track String
  //
  svc_mus_track_edit=new QLineEdit(this,"svc_mus_track_edit");
  svc_mus_track_edit->setGeometry(620,234,240,19);
  svc_mus_track_edit->setMaxLength(255);
  label=new QLabel(svc_mus_track_edit,
		   tr("Insert Voice Track String:"),this,
		   "svc_mus_track_label");
  label->setGeometry(450,234,165,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mus_track_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Insert Spot Break String
  //
  svc_mus_break_edit=new QLineEdit(this,"svc_mus_break_edit");
  svc_mus_break_edit->setGeometry(620,256,240,19);
  svc_mus_break_edit->setMaxLength(255);
  label=new QLabel(svc_mus_break_edit,
		   tr("Insert Traffic Break String:"),this,
		   "svc_mus_break_label");
  label->setGeometry(450,256,165,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mus_break_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Music Cartname Parser Data Section
  //
  label=new QLabel(tr("Cart Number:"),this,
		   "svc_mus_cartname_label");
  label->setGeometry(445,276,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Cart Offset
  //
  svc_muscart_offset_spin=new QSpinBox(this,"svc_muscart_offset_spin");
  svc_muscart_offset_spin->setGeometry(620,276,50,19);
  svc_muscart_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_muscart_offset_spin,
		   tr("Offset:"),this,
		   "svc_muscart_win_path_label");
  label->setGeometry(570,276,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muscart_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Cart Length
  //
  svc_muscart_length_spin=new QSpinBox(this,"svc_muscart_length_spin");
  svc_muscart_length_spin->setGeometry(730,276,50,19);
  svc_muscart_length_spin->setRange(0,6);
  label=new QLabel(svc_muscart_length_spin,
		   tr("Length:"),this,
		   "svc_muscart_win_path_label");
  label->setGeometry(675,276,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muscart_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Title Parser Data Section
  //
  label=new QLabel(tr("Title:"),this,"svc_tfc_cartname_label");
  label->setGeometry(445,297,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Title Cart Offset
  //
  svc_mustitle_offset_spin=new QSpinBox(this,"svc_mustitle_offset_spin");
  svc_mustitle_offset_spin->setGeometry(620,297,50,19);
  svc_mustitle_offset_spin->setRange(0,1024);
  label=new QLabel(svc_mustitle_offset_spin,
		   tr("Offset:"),this,
		   "svc_mustitle_win_path_label");
  label->setGeometry(570,297,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mustitle_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Title Cart Length
  //
  svc_mustitle_length_spin=new QSpinBox(this,"svc_mustitle_length_spin");
  svc_mustitle_length_spin->setGeometry(730,297,50,19);
  svc_mustitle_length_spin->setRange(0,255);
  label=new QLabel(svc_mustitle_length_spin,
		   tr("Length:"),this,
		   "svc_mustitle_win_path_label");
  label->setGeometry(675,297,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mustitle_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Hours Parser Data Section
  //
  label=new QLabel(tr("Start Time - Hours:"),this,
		   "svc_mus_startname_label");
  label->setGeometry(445,318,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Start Hours Offset
  //
  svc_mushours_offset_spin=new QSpinBox(this,"svc_mushours_offset_spin");
  svc_mushours_offset_spin->setGeometry(620,318,50,19);
  svc_mushours_offset_spin->setRange(0,1024);
  label=new QLabel(svc_mushours_offset_spin,
		   tr("Offset:"),this,
		   "svc_mushours_win_path_label");
  label->setGeometry(570,318,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mushours_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Hours Length
  //
  svc_mushours_length_spin=new QSpinBox(this,"svc_mushours_length_spin");
  svc_mushours_length_spin->setGeometry(730,318,50,19);
  svc_mushours_length_spin->setRange(0,8);
  label=new QLabel(svc_mushours_length_spin,
		   tr("Length:"),this,
		   "svc_mushours_win_path_label");
  label->setGeometry(675,318,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_mushours_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Minutes Parser Data Section
  //
  label=new QLabel(tr("Start Time - Minutes:"),this,
		   "svc_mus_startname_label");
  label->setGeometry(445,339,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Start Minutes Offset
  //
  svc_musminutes_offset_spin=new QSpinBox(this,"svc_musminutes_offset_spin");
  svc_musminutes_offset_spin->setGeometry(620,339,50,19);
  svc_musminutes_offset_spin->setRange(0,1024);
  label=new QLabel(svc_musminutes_offset_spin,
		   tr("Offset:"),this,
		   "svc_musminutes_win_path_label");
  label->setGeometry(570,339,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musminutes_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Minutes Length
  //
  svc_musminutes_length_spin=new QSpinBox(this,"svc_musminutes_length_spin");
  svc_musminutes_length_spin->setGeometry(730,339,50,19);
  svc_musminutes_length_spin->setRange(0,8);
  label=new QLabel(svc_musminutes_length_spin,
		   tr("Length:"),this,
		   "svc_musminutes_win_path_label");
  label->setGeometry(675,339,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musminutes_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Seconds Parser Data Section
  //
  label=new QLabel(tr("Start Time - Seconds:"),this,
		   "svc_mus_startname_label");
  label->setGeometry(445,360,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Start Seconds Offset
  //
  svc_musseconds_offset_spin=new QSpinBox(this,"svc_musseconds_offset_spin");
  svc_musseconds_offset_spin->setGeometry(620,360,50,19);
  svc_musseconds_offset_spin->setRange(0,1024);
  label=new QLabel(svc_musseconds_offset_spin,
		   tr("Offset:"),this,
		   "svc_musseconds_win_path_label");
  label->setGeometry(570,360,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musseconds_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Start Seconds Length
  //
  svc_musseconds_length_spin=new QSpinBox(this,"svc_musseconds_length_spin");
  svc_musseconds_length_spin->setGeometry(730,360,50,19);
  svc_musseconds_length_spin->setRange(0,8);
  label=new QLabel(svc_musseconds_length_spin,
		   tr("Length - Seconds:"),this,
		   "svc_musseconds_win_path_label");
  label->setGeometry(675,360,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musseconds_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Hours Parser Data Section
  //
  label=new QLabel(tr("Length - Hours:"),this,
		   "svc_mus_lengthname_label");
  label->setGeometry(445,381,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Length Hours Offset
  //
  svc_muslenhours_offset_spin=new QSpinBox(this,"svc_muslenhours_offset_spin");
  svc_muslenhours_offset_spin->setGeometry(620,381,50,19);
  svc_muslenhours_offset_spin->setRange(0,1024);
  label=new QLabel(svc_muslenhours_offset_spin,
		   tr("Offset:"),this,
		   "svc_muslenhours_win_path_label");
  label->setGeometry(570,381,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenhours_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Hours Length
  //
  svc_muslenhours_length_spin=new QSpinBox(this,"svc_muslenhours_length_spin");
  svc_muslenhours_length_spin->setGeometry(730,381,50,19);
  svc_muslenhours_length_spin->setRange(0,8);
  label=new QLabel(svc_muslenhours_length_spin,
		   tr("Length:"),this,
		   "svc_muslenhours_win_path_label");
  label->setGeometry(675,381,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenhours_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Minutes Parser Data Section
  //
  label=new QLabel(tr("Length - Minutes:"),this,
		   "svc_mus_lengthname_label");
  label->setGeometry(445,402,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Length Minutes Offset
  //
  svc_muslenminutes_offset_spin=new QSpinBox(this,"svc_muslenminutes_offset_spin");
  svc_muslenminutes_offset_spin->setGeometry(620,402,50,19);
  svc_muslenminutes_offset_spin->setRange(0,1024);
  label=new QLabel(svc_muslenminutes_offset_spin,
		   tr("Offset:"),this,
		   "svc_muslenminutes_win_path_label");
  label->setGeometry(570,402,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenminutes_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Minutes Length
  //
  svc_muslenminutes_length_spin=new QSpinBox(this,"svc_muslenminutes_length_spin");
  svc_muslenminutes_length_spin->setGeometry(730,402,50,19);
  svc_muslenminutes_length_spin->setRange(0,8);
  label=new QLabel(svc_muslenminutes_length_spin,
		   tr("Length:"),this,
		   "svc_muslenminutes_win_path_label");
  label->setGeometry(675,402,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenminutes_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Seconds Parser Data Section
  //
  label=new QLabel(tr("Length - Seconds:"),this,
		   "svc_mus_lengthname_label");
  label->setGeometry(445,423,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Length Seconds Offset
  //
  svc_muslenseconds_offset_spin=new QSpinBox(this,"svc_muslenseconds_offset_spin");
  svc_muslenseconds_offset_spin->setGeometry(620,423,50,19);
  svc_muslenseconds_offset_spin->setRange(0,1024);
  label=new QLabel(svc_muslenseconds_offset_spin,
		   tr("Offset:"),this,
		   "svc_muslenseconds_win_path_label");
  label->setGeometry(570,423,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenseconds_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Length Seconds Length
  //
  svc_muslenseconds_length_spin=new QSpinBox(this,"svc_muslenseconds_length_spin");
  svc_muslenseconds_length_spin->setGeometry(730,423,50,19);
  svc_muslenseconds_length_spin->setRange(0,8);
  label=new QLabel(svc_muslenseconds_length_spin,
		   tr("Length:"),this,
		   "svc_muslenseconds_win_path_label");
  label->setGeometry(675,423,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_muslenseconds_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Contract # Parser Data Section
  //
  label=new QLabel(tr("Contract #:"),this,
		   "svc_mus_dataname_label");
  label->setGeometry(445,444,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Contract # Offset
  //
  svc_musdata_offset_spin=new QSpinBox(this,"svc_musdata_offset_spin");
  svc_musdata_offset_spin->setGeometry(620,444,50,19);
  svc_musdata_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_musdata_offset_spin,
		   tr("Offset:"),this,
		   "svc_musdata_win_path_label");
  label->setGeometry(570,444,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musdata_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Contract # Length
  //
  svc_musdata_length_spin=new QSpinBox(this,"svc_musdata_length_spin");
  svc_musdata_length_spin->setGeometry(730,444,50,19);
  svc_musdata_length_spin->setRange(0,32);
  label=new QLabel(svc_musdata_length_spin,
		   tr("Length:"),this,
		   "svc_musdata_win_path_label");
  label->setGeometry(675,444,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musdata_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Event ID Parser Data Section
  //
  label=new QLabel(tr("Event ID:"),this,
		   "svc_mus_eventidname_label");
  label->setGeometry(445,465,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Event ID Offset
  //
  svc_museventid_offset_spin=new QSpinBox(this,"svc_museventid_offset_spin");
  svc_museventid_offset_spin->setGeometry(620,465,50,19);
  svc_museventid_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_museventid_offset_spin,
		   tr("Offset:"),this,
		   "svc_museventid_win_path_label");
  label->setGeometry(570,465,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_museventid_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Event ID Length
  //
  svc_museventid_length_spin=new QSpinBox(this,"svc_museventid_length_spin");
  svc_museventid_length_spin->setGeometry(730,465,50,19);
  svc_museventid_length_spin->setRange(0,8);
  label=new QLabel(svc_museventid_length_spin,
		   tr("Length:"),this,
		   "svc_museventid_win_path_label");
  label->setGeometry(675,465,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_museventid_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Announcement Type Parser Data Section
  //
  label=new QLabel(tr("Annc. Type:"),this,
		   "svc_mus_annctypename_label");
  label->setGeometry(445,486,120,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Music Announcement Type Offset
  //
  svc_musannctype_offset_spin=new QSpinBox(this,"svc_musannctype_offset_spin");
  svc_musannctype_offset_spin->setGeometry(620,486,50,19);
  svc_musannctype_offset_spin->setRange(0,RD_MAX_IMPORT_LINE_LENGTH);
  label=new QLabel(svc_musannctype_offset_spin,
		   tr("Offset:"),this,
		   "svc_musannctype_win_path_label");
  label->setGeometry(570,486,45,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musannctype_offset_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Announcement Type Length
  //
  svc_musannctype_length_spin=new QSpinBox(this,"svc_musannctype_length_spin");
  svc_musannctype_length_spin->setGeometry(730,486,50,19);
  svc_musannctype_length_spin->setRange(0,8);
  label=new QLabel(svc_musannctype_length_spin,
		   tr("Length:"),this,
		   "svc_musannctype_win_path_label");
  label->setGeometry(675,486,50,19);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  connect(svc_musannctype_length_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(valueChangedData(int)));

  //
  // Music Test Button
  //
  button=new QPushButton(this,"mus_button");
  button->setGeometry(795,277,60,40);
  button->setFont(font);
  button->setText(tr("Test \n&Music"));
  connect(button,SIGNAL(clicked()),this,SLOT(musicData()));

  //
  //  Ok Button
  //
  button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(font);
  button->setText(tr("&OK"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,
			     80,50);
  button->setFont(font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Group List
  //
  group=svc_svc->trackGroup();
  sql="select NAME from GROUPS order by NAME";
  q=new RDSqlQuery(sql);
  while(q->next()) {
    svc_voice_group_box->insertItem(q->value(0).toString());
    if(q->value(0).toString()==group) {
      svc_voice_group_box->setCurrentItem(svc_voice_group_box->count()-1);
    }
  }
  delete q;

  //
  // Populate Fields
  //
  svc_name_edit->setText(svc_svc->name());
  svc_description_edit->setText(svc_svc->description());
  svc_template_edit->setText(svc_svc->nameTemplate());
  svc_chain_box->setChecked(svc_svc->chainto());
  svc_autorefresh_box->setChecked(svc_svc->autoRefresh());
  if(svc_svc->defaultLogShelflife()>=0) {
    svc_loglife_box->setChecked(true);
    svc_loglife_spin->setValue(svc_svc->defaultLogShelflife());
  }
  else {
    svc_loglife_spin->setDisabled(true);
  }
  if(svc_svc->elrShelflife()>=0) {
    svc_shelflife_box->setChecked(true);
    svc_shelflife_spin->setValue(svc_svc->elrShelflife());
  }
  else {
    svc_shelflife_spin->setDisabled(true);
  }
  svc_tfc_path_edit->setText(svc_svc->importPath(RDSvc::Traffic,RDSvc::Linux));
  svc_tfc_win_path_edit->
    setText(svc_svc->importPath(RDSvc::Traffic,RDSvc::Windows));  
  svc_tfc_label_cart_edit->setText(svc_svc->labelCart(RDSvc::Traffic));
  svc_tfc_track_edit->setText(svc_svc->trackString(RDSvc::Traffic));
  svc_tfccart_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::CartNumber));
  svc_tfccart_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::CartNumber));
  svc_tfctitle_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::Title));
  svc_tfctitle_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::Title));
  svc_tfchours_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::StartHours));
  svc_tfchours_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::StartHours));
  svc_tfcminutes_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::StartMinutes));
  svc_tfcminutes_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::StartMinutes));
  svc_tfcseconds_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::StartSeconds));
  svc_tfcseconds_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::StartSeconds));
  svc_tfclenhours_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::LengthHours));
  svc_tfclenhours_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::LengthHours));
  svc_tfclenminutes_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::LengthMinutes));
  svc_tfclenminutes_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::LengthMinutes));
  svc_tfclenseconds_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::LengthSeconds));
  svc_tfclenseconds_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::LengthSeconds));
  svc_tfcdata_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::ExtData));
  svc_tfcdata_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::ExtData));
  svc_tfceventid_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::ExtEventId));
  svc_tfceventid_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::ExtEventId));
  svc_tfcannctype_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Traffic,RDSvc::ExtAnncType));
  svc_tfcannctype_length_spin->
    setValue(svc_svc->importLength(RDSvc::Traffic,RDSvc::ExtAnncType));
  svc_mus_path_edit->setText(svc_svc->importPath(RDSvc::Music,RDSvc::Linux));
  svc_mus_win_path_edit->
    setText(svc_svc->importPath(RDSvc::Music,RDSvc::Windows));  
  svc_mus_label_cart_edit->setText(svc_svc->labelCart(RDSvc::Music));
  svc_mus_break_edit->
    setText(svc_svc->breakString());
  svc_mus_track_edit->
    setText(svc_svc->trackString(RDSvc::Music));
  svc_muscart_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::CartNumber));
  svc_muscart_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::CartNumber));
  svc_mustitle_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::Title));
  svc_mustitle_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::Title));
  svc_mushours_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::StartHours));
  svc_mushours_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::StartHours));
  svc_musminutes_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::StartMinutes));
  svc_musminutes_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::StartMinutes));
  svc_musseconds_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::StartSeconds));
  svc_musseconds_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::StartSeconds));
  svc_muslenhours_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::LengthHours));
  svc_muslenhours_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::LengthHours));
  svc_muslenminutes_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::LengthMinutes));
  svc_muslenminutes_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::LengthMinutes));
  svc_muslenseconds_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::LengthSeconds));
  svc_muslenseconds_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::LengthSeconds));
  svc_musdata_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::ExtData));
  svc_musdata_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::ExtData));
  svc_museventid_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::ExtEventId));
  svc_museventid_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::ExtEventId));
  svc_musannctype_offset_spin->
    setValue(svc_svc->importOffset(RDSvc::Music,RDSvc::ExtAnncType));
  svc_musannctype_length_spin->
    setValue(svc_svc->importLength(RDSvc::Music,RDSvc::ExtAnncType));
  import_changed=false;
}


EditSvc::~EditSvc()
{
  delete svc_name_edit;
  delete svc_description_edit;
}


QSize EditSvc::sizeHint() const
{
  return QSize(870,586);
} 


QSizePolicy EditSvc::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditSvc::autofillData()
{
  AutofillCarts *autofill=new AutofillCarts(svc_svc,this,"autofill");
  autofill->exec();
  delete autofill;
}


void EditSvc::enableHostsData()
{
  EditSvcPerms *edit_perms=new EditSvcPerms(svc_svc,this,"edit_perms");
  edit_perms->exec();
  delete edit_perms;
}


void EditSvc::trafficData()
{
  TestDataImport(RDSvc::Traffic);
}


void EditSvc::musicData()
{
  TestDataImport(RDSvc::Music);
}


void EditSvc::textChangedData(const QString &)
{
  import_changed=true;
}


void EditSvc::valueChangedData(int)
{
  import_changed=true;
}


void EditSvc::okData()
{
  Save();
  done(0);
}


void EditSvc::cancelData()
{
  done(-1);
}


void EditSvc::TestDataImport(RDSvc::ImportSource src)
{
  if(import_changed) {
    switch(QMessageBox::question(this,tr("Save Import Data"),
				 tr("Before testing, the import configuration\nmust be saved.  Save now?"),QMessageBox::Yes,QMessageBox::No)) {
	case QMessageBox::No:
	case QMessageBox::NoButton:
	  return;

	default:
	  break;
    }
    Save();
  }
  TestImport *testimport=new TestImport(svc_svc,src,this,"testimport");
  testimport->exec();
  delete testimport;
}


void EditSvc::Save()
{
  svc_svc->setDescription(svc_description_edit->text());
  svc_svc->setNameTemplate(svc_template_edit->text());
  svc_svc->setChainto(svc_chain_box->isChecked());
  svc_svc->setAutoRefresh(svc_autorefresh_box->isChecked());
  if(svc_loglife_box->isChecked()) {
    svc_svc->setDefaultLogShelflife(svc_loglife_spin->value());
  }
  else {
    svc_svc->setDefaultLogShelflife(-1);
  }
  if(svc_shelflife_box->isChecked()) {
    svc_svc->setElrShelflife(svc_shelflife_spin->value());
  }
  else {
    svc_svc->setElrShelflife(-1);
  }
  svc_svc->
    setImportPath(RDSvc::Traffic,RDSvc::Linux,svc_tfc_path_edit->text());
  svc_svc->
    setImportPath(RDSvc::Traffic,RDSvc::Windows,svc_tfc_win_path_edit->text());
  svc_svc->setLabelCart(RDSvc::Traffic,svc_tfc_label_cart_edit->text());
  svc_svc->setTrackString(RDSvc::Traffic,svc_tfc_track_edit->text());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::CartNumber,
			   svc_tfccart_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::CartNumber,
			   svc_tfccart_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::Title,
			   svc_tfctitle_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::Title,
			   svc_tfctitle_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::StartHours,
			   svc_tfchours_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::StartHours,
			   svc_tfchours_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::StartMinutes,
			   svc_tfcminutes_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::StartMinutes,
			   svc_tfcminutes_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::StartSeconds,
			   svc_tfcseconds_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::StartSeconds,
			   svc_tfcseconds_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::LengthHours,
			   svc_tfclenhours_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::LengthHours,
			   svc_tfclenhours_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::LengthMinutes,
			   svc_tfclenminutes_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::LengthMinutes,
			   svc_tfclenminutes_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::LengthSeconds,
			   svc_tfclenseconds_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::LengthSeconds,
			   svc_tfclenseconds_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::ExtData,
			   svc_tfcdata_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::ExtData,
			   svc_tfcdata_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::ExtEventId,
			   svc_tfceventid_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::ExtEventId,
			   svc_tfceventid_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Traffic,RDSvc::ExtAnncType,
			   svc_tfcannctype_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Traffic,RDSvc::ExtAnncType,
			   svc_tfcannctype_length_spin->value());
  svc_svc->setImportPath(RDSvc::Music,RDSvc::Linux,svc_mus_path_edit->text());
  svc_svc->
    setImportPath(RDSvc::Music,RDSvc::Windows,svc_mus_win_path_edit->text());
  svc_svc->setBreakString(svc_mus_break_edit->text());
  svc_svc->setTrackString(RDSvc::Music,svc_mus_track_edit->text());
  svc_svc->setLabelCart(RDSvc::Music,svc_mus_label_cart_edit->text());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::CartNumber,
			   svc_muscart_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::CartNumber,
			   svc_muscart_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::Title,
			   svc_mustitle_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::Title,
			   svc_mustitle_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::StartHours,
			   svc_mushours_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::StartHours,
			   svc_mushours_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::StartMinutes,
			   svc_musminutes_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::StartMinutes,
			   svc_musminutes_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::StartSeconds,
			   svc_musseconds_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::StartSeconds,
			   svc_musseconds_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::LengthHours,
			   svc_muslenhours_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::LengthHours,
			   svc_muslenhours_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::LengthMinutes,
			   svc_muslenminutes_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::LengthMinutes,
			   svc_muslenminutes_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::LengthSeconds,
			   svc_muslenseconds_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::LengthSeconds,
			   svc_muslenseconds_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::ExtData,
			   svc_musdata_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::ExtData,
			   svc_musdata_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::ExtEventId,
			   svc_museventid_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::ExtEventId,
			   svc_museventid_length_spin->value());
  svc_svc->setImportOffset(RDSvc::Music,RDSvc::ExtAnncType,
			   svc_musannctype_offset_spin->value());
  svc_svc->setImportLength(RDSvc::Music,RDSvc::ExtAnncType,
			   svc_musannctype_length_spin->value());
  import_changed=false;
  if(svc_voice_group_box->currentItem()==0) {
    svc_svc->setTrackGroup("");
  }
  else {
    svc_svc->setTrackGroup(svc_voice_group_box->currentText());
  }
}

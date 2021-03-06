// rlmhost.h
//
// A container class for a Rivendell Loadable Module host.
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: rlmhost.h,v 1.1.2.4 2008/12/01 13:36:07 fredg Exp $
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

#ifndef RLMHOST_H
#define RLMHOST_H

#include <vector>

#include <qstring.h>
#include <qsocketdevice.h>
#include <qtimer.h>

#include <rdttydevice.h>
#include <rdlog_line.h>
#include "../rlm/rlm.h"

class RLMHost : public QObject
{
  Q_OBJECT
 public:
  RLMHost(const QString &path,const QString &arg,QSocketDevice *udp_socket,
	  QObject *parent=0,const char *name=0);
  ~RLMHost();
  QString pluginPath() const;
  QString pluginArg() const;
  void sendEvent(const QString &svcname,int lognum,RDLogLine **loglines,
		 bool onair);
  bool load();
  void unload();
  static void loadMetadata(const RDLogLine *logline,struct rlm_pad *pad);
  static void saveMetadata(const struct rlm_pad *pad,RDLogLine *logline);
 private slots:
  void timerData(int timernum);

 private:
  QString plugin_path;
  QString plugin_arg;
  QSocketDevice *plugin_udp_socket;
  void *plugin_handle;
  void (*plugin_start_sym)(void *,const char *);
  void (*plugin_free_sym)(void *);
  void (*plugin_pad_data_sent_sym)
    (void *,const char *,int,int,const struct rlm_pad *,const struct rlm_pad *);
  void (*plugin_timer_expired_sym)(void *,int);

  std::vector<RDTTYDevice *> plugin_tty_devices;
  QTimer *plugin_callback_timers[RLM_MAX_TIMERS];
  friend void RLMSendUdp(void *ptr,const char *ipaddr,uint16_t port,
			 const char *data,int len);
  friend int RLMOpenSerial(void *ptr,const char *port,int speed,
			   int parity,int word_length);
  friend void RLMSendSerial(void *ptr,int handle,const char *data,
			    int len);
  friend void RLMCloseSerial(void *ptr,int handle);
  friend const char *RLMDateTime(int offset_msecs,const char *format);
  friend const char *RLMResolveNowNext(void *ptr,
				       const struct rlm_pad *now,
				       const struct rlm_pad *next,
				       const char *format);
  friend void RLMLog(void *ptr,int prio,const char *msg);
  friend void RLMStartTimer(void *ptr,int timernum,int msecs,
			    int mode);
  friend void RLMStopTimer(void *ptr,int timernum);
  friend const char *RLMDateTime(void *ptr,int offset_msecs,
				 const char *format);
  friend const char *RLMGetStringValue(void *ptr,const char *filename,
				       const char *section,const char *label,
				       const char *default_value);
  char plugin_value_string[1024];
};


#endif  // RLMHOST_H

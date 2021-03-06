// rdlog_event.h
//
// Abstract Rivendell Log Events
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: rdlog_event.h,v 1.32.2.1 2009/02/12 14:21:26 cvs Exp $
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

#include <vector>

#include <qdatetime.h>
#include <qsqldatabase.h>

#include <rdlog_line.h>


#ifndef RDLOG_EVENT_H
#define RDLOG_EVENT_H

#define INSERT_STEP_SIZE 500

using namespace std;

class RDLogEvent
{
  public:
   RDLogEvent(QString logname="");
   ~RDLogEvent();
   bool exists();
   bool exists(int line);
   QString logName() const;
   void setLogName(QString logname);
   QString serviceName() const;
   int load();
   void save(bool update_tracks=true,int line=-1);
   int validate(QString *report,const QDate &date);
   void clear();
   void refresh(int line);
   int size() const;
   void insert(int line,int num_lines,bool preserve_trans=false);
   void remove(int line,int num_lines,bool preserve_trans=false);
   void move(int from_line,int to_line);
   void copy(int from_line,int to_line);
   int length(int from_line,int to_line,QTime *sched_time=NULL);
   int lengthToStop(int from_line,QTime *sched_time=NULL);
   bool blockLength(int *nominal_length,int *actual_length,int line);
   QTime blockStartTime(int line);
   RDLogLine *logLine(int line);
   RDLogLine *loglineById(int id);
   int lineById(int id) const;
   int nextTimeStart(QTime after);
   RDLogLine::TransType nextTransType(int);
   void removeCustomTransition(int line);
   int nextId() const;
   int nextLinkId() const;

  private:
   void SaveLine(int line);
   void LoadNowNext();
   QString log_name;
   QString log_service_name;
   int log_max_id;
   vector<RDLogLine *> log_line;
};


#endif 

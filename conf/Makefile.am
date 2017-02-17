## automake.am
##
## Top level automake.am for Rivendell
##
## (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
##
##      $Id: Makefile.am,v 1.11.2.7 2009/05/13 17:50:32 cvs Exp $
##      $Date: 2009/05/13 17:50:32 $
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

install-exec-local:	
	mkdir -p $(DESTDIR)/etc/httpd/conf.d
	cp rd-bin.conf $(DESTDIR)/etc/httpd/conf.d/

uninstall-local:	
	rm -f $(DESTDIR)/etc/httpd/conf.d/rd-bin.conf

EXTRA_DIST = rd.conf-sample rd.conf-complete-sample rd.conf-slax\
             my.cnf-master my.cnf-standby rd-sample.ini rd-bin.conf.in\
             rlm_serial.conf rlm_xmpad.conf rlm_udp.conf rlm_twitter.conf\
             rlm_facebook.conf rlm_ando.conf rlm_inno713.conf

CLEANFILES = *~
DISTCLEANFILES = rdfeed.conf
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4

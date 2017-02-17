## automake.am
##
## Automake.am for rivendell/rlm
##
##   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
##
##      $Id: Makefile.am,v 1.1.2.10 2009/05/13 17:50:32 cvs Exp $
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
## Use automake to process this into a Makefile.in

AM_CFLAGS = -fPIC -Wall

RLM_MODULES=rlm_test.rlm rlm_serial.rlm rlm_xmpad.rlm rlm_udp.rlm\
            rlm_twitter.rlm rlm_facebook.rlm rlm_ando.rlm rlm_inno713.rlm

all:	$(RLM_MODULES)

%.rlm:	%.c
	$(CC) $(AM_CFLAGS) -I$(top_srcdir) -shared $< -o $@

install-exec-local:	all
	mkdir -p $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell
	cp $(RLM_MODULES) $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/
	mkdir -p $(DESTDIR)$(headerdir)
	cp rlm.h $(DESTDIR)$(headerdir)
	mkdir -p $(DESTDIR)$(prefix)/src/rlm
	cp *.c $(DESTDIR)$(prefix)/src/rlm/
	cp Makefile-example $(DESTDIR)$(prefix)/src/rlm/Makefile

uninstall-local:	
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_test.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_serial.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_xmpad.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_udp.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_twitter.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_facebook.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_ando.rlm
	rm -f $(DESTDIR)$(prefix)/@RD_LIB_PATH@/rivendell/rlm_inno713.rlm

headerdir = $(includedir)/rlm
header_HEADERS = rlm.h

EXTRA_DIST = rlm.h rlm_test.c rlm_serial.c rlm_xmpad.c rlm_udp.c\
             rlm_twitter.c rlm_facebook.c rlm_ando.c rlm_inno713.c\
             Makefile-example

CLEANFILES = *~ moc_* *.qm *.obj *.idb *.pdb *ilk *.rlm
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 *.tar.gz moc_*

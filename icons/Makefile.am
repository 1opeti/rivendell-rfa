## automake.am
##
## icons/automake.am for Rivendell
##
## (C) Copyright 2002-2006 Fred Gleason <fredg@salemradiolabs.com>
##
##      $Id: Makefile.am,v 1.16.2.2 2009/06/30 22:31:51 cvs Exp $
##      $Date: 2009/06/30 22:31:51 $
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

install-exec-am:	
	mkdir -p $(DESTDIR)@libexecdir@
	cp greencheckmark.png $(DESTDIR)@libexecdir@
	cp redx.png $(DESTDIR)@libexecdir@
	cp greenball.png $(DESTDIR)@libexecdir@
	cp redball.png $(DESTDIR)@libexecdir@
	cp whiteball.png $(DESTDIR)@libexecdir@
	cp progressbar.gif $(DESTDIR)@libexecdir@
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps
	cp rivendell-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps
	cp rivendell-22x22.png $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps
	cp rivendell-32x32.png $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps
	cp rivendell-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/64x64/apps
	cp rivendell-64x64.png $(DESTDIR)@prefix@/share/icons/hicolor/64x64/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/128x128/apps
	cp rivendell-128x128.png $(DESTDIR)@prefix@/share/icons/hicolor/128x128/apps/rivendell.png
	mkdir -p $(DESTDIR)@prefix@/share/X11/fvwm2/pixmaps
	cp rivendell-16x16.xpm $(DESTDIR)@prefix@/share/X11/fvwm2/pixmaps/mini.rivendell.xpm
	cp rivendell-32x32.xpm $(DESTDIR)@prefix@/share/X11/fvwm2/pixmaps/rivendell.xpm

uninstall-local:	
	rm -f $(DESTDIR)@libexecdir@/greencheckmark.png
	rm -f $(DESTDIR)@libexecdir@/redx.png
	rm -f $(DESTDIR)@libexecdir@/greenball.png
	rm -f $(DESTDIR)@libexecdir@/redball.png
	rm -f $(DESTDIR)@libexecdir@/whiteball.png
	rm -f $(DESTDIR)@libexecdir@/progressbar.gif
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/rivendell.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/rivendell.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/rivendell.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/rivendell.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/64x64/apps/rivendell.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/128x128/apps/rivendell-128x128.png

EXTRA_DIST = rivendell.res play.xpm macro.xpm\
             fillstart.xpm fillstop.xpm\
             note.xpm note2.xpm\
             post.xpm post2.xpm post3.xpm\
             record.xpm record2.xpm record3.xpm\
             rml.xpm rml2.xpm rml3.xpm rml5.xpm\
             switch.xpm switch2.xpm switch3.xpm\
             marker.xpm chain.xpm rivendell.ico\
             download.xpm upload.xpm track.xpm\
             checkmark.xpm ex.xpm track_cart.xpm\
             track_cart.xpm rivendell-16x16.xpm\
             greencheckmark.xpm redcheckmark.xpm\
             redx.xpm whiteball.xpm blueball.xpm\
             redball.xpm greenball.xpm mic16.xpm\
             music.xpm traffic.xpm notemarker.xpm\
             rivendell-22x22.xpm rivendell-32x32.xpm\
             rivendell-48x48.xpm rivendell-64x64.xpm\
             rivendell-128x128.xpm greencheckmark.png redx.png\
             greenball.png redball.png whiteball.png\
             user.xpm admin.xpm\
             progressbar.xcf progressbar.gif\
             rivendell-16x16.png\
             rivendell-22x22.png\
             rivendell-32x32.png\
             rivendell-48x48.png\
             rivendell-64x64.png\
             rivendell-128x128.png

CLEANFILES = *~
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4

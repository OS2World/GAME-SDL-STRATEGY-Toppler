datafiles = toppler.dat \
boing.wav bubbles.wav hit.wav honk.wav splash.wav swoosh.wav tap.wav tick.wav water.wav \
fanfare.wav rumble.wav score.wav alarm.wav \
m1.ttm m2.ttm ball1.ttm ball2.ttm abc.ttm \
toppler.xpm
headerfiles = archi.h bonus.h config.h decl.h elevators.h game.h keyb.h level.h menu.h \
points.h robots.h screen.h snowball.h sound.h sprites.h stars.h toppler.h leveledit.h txtsys.h \
configuration.h

scoredir = $(localstatedir)/@PACKAGE@
scorefile = $(scoredir)/@PACKAGE@.hsc
CPPFLAGS = @CPPFLAGS@ -Wall -DTOP_DATADIR=\"$(pkgdatadir)\" -DSCOREFILE=\"$(scorefile)\" -DVERSION=\"@TOPPLER_VERSION@\"

bin_PROGRAMS = toppler
toppler_SOURCES = archi.cc bonus.cc decl.cc elevators.cc game.cc keyb.cc level.cc menu.cc \
main.cc points.cc robots.cc screen.cc snowball.cc sound.cc sprites.cc stars.cc \
toppler.cc leveledit.cc txtsys.cc configuration.cc
pkgdata_DATA = $(datafiles)
man_MANS = toppler.6
EXTRA_DIST = $(man_MANS) $(datafiles) $(headerfiles) VERSION toppler.spec.in toppler.spec toppler.desktop.in \
 config.sub config.guess toppler.xpm

global_highscore:
	-(chgrp games $(DESTDIR)$(bindir)/$$p; chmod g+s $(DESTDIR)$(bindir)/$$p)
	install -d $(DESTDIR)$(scoredir)
	touch $(DESTDIR)$(scorefile)
	-(chgrp games $(DESTDIR)$(scorefile); chmod g+rw $(DESTDIR)$(scorefile))



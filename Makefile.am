datafiles = toppler.dat \
boing.wav bubbles.wav hit.wav honk.wav splash.wav swoosh.wav tap.wav tick.wav water.wav \
fanfare.wav rumble.wav score.wav alarm.wav \
m1.ttm m2.ttm \
toppler.6
headerfiles = archi.h bonus.h config.h decl.h elevators.h game.h keyb.h level.h menu.h \
palette.h points.h robots.h screen.h snowball.h sound.h sprites.h stars.h toppler.h leveledit.h

scoredir = $(localstatedir)/@PACKAGE@
scorefile = $(scoredir)/@PACKAGE@.hsc
CPPFLAGS = @CPPFLAGS@ -DTOP_DATADIR=\"$(pkgdatadir)\" -DSCOREFILE=\"$(scorefile)\"
bin_PROGRAMS = toppler
toppler_SOURCES = archi.cc bonus.cc decl.cc elevators.cc game.cc keyb.cc level.cc menu.cc \
main.cc palette.cc points.cc robots.cc screen.cc snowball.cc sound.cc sprites.cc stars.cc \
toppler.cc leveledit.cc
pkgdata_DATA = $(datafiles)
EXTRA_DIST = $(datafiles) $(headerfiles) VERSION

global_highscore:
	-(chgrp games $(DESTDIR)$(bindir)/$$p; chmod g+s $(DESTDIR)$(bindir)/$$p)
	install -d $(DESTDIR)$(scoredir)
	touch $(DESTDIR)$(scorefile)
	-(chgrp games $(DESTDIR)$(scorefile); chmod g+rw $(DESTDIR)$(scorefile))



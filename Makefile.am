datafiles = toppler.dat \
boing.wav bubbles.wav hit.wav honk.wav splash.wav swoosh.wav tap.wav tick.wav water.wav \
fanfare.wav rumble.wav score.wav alarm.wav \
layer1.tga layer2.tga layer3.tga \
m1.ttm m2.ttm \
toppler.6
headerfiles = archi.h bonus.h config.h decl.h elevators.h game.h keyb.h level.h menu.h \
palette.h points.h robots.h screen.h snowball.h sound.h sprites.h stars.h toppler.h leveledit.h

scoredir = $(localstatedir)/@PACKAGE@
scorefile = $(scoredir)/@PACKAGE@.hsc
CPPFLAGS = @CPPFLAGS@ -DDATADIR=\"$(pkgdatadir)\" -DSCOREFILE=\"$(scorefile)\"
bin_PROGRAMS = toppler
toppler_SOURCES = archi.cc bonus.cc decl.cc elevators.cc game.cc keyb.cc level.cc menu.cc \
main.cc palette.cc points.cc robots.cc screen.cc snowball.cc sound.cc sprites.cc stars.cc \
toppler.cc leveledit.cc
pkgdata_DATA = $(datafiles)
EXTRA_DIST = $(datafiles) $(headerfiles) VERSION

install-exec-hook:
	-(chgrp games $(bindir)/$$p; chmod g+s $(bindir)/$$p)
	install -d $(scoredir)
	touch $(scorefile)
	-(chgrp games $(scorefile); chmod g+rw $(scorefile))

datafiles = nebulous.dat boing.wav bubbles.wav hit.wav honk.wav splash.wav swosh.wav tap.wav tick.wav water.wav layer1.tga layer2.tga layer3.tga
headerfiles = archi.h bonus.h config.h decl.h elevators.h game.h keyb.h level.h menu.h palette.h points.h robots.h screen.h snowball.h sound.h sprites.h stars.h toppler.h

CPPFLAGS = @CPPFLAGS@ -DDATADIR=\"$(pkgdatadir)\"
bin_PROGRAMS = toppler
toppler_SOURCES = archi.cc bonus.cc decl.cc elevators.cc game.cc keyb.cc level.cc menu.cc nebulous.cc palette.cc points.cc robots.cc screen.cc snowball.cc sound.cc sprites.cc stars.cc toppler.cc
pkgdata_DATA = $(datafiles)
EXTRA_DIST = $(datafiles) $(headerfiles) VERSION

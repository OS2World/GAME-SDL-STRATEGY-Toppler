datafiles = toppler.dat \
m1.ttm m2.ttm ball1.ttm ball2.ttm ball3.ttm abc.ttm \
toppler.xpm
headerfiles = archi.h bonus.h config.h decl.h elevators.h game.h keyb.h level.h menu.h \
points.h robots.h screen.h snowball.h sound.h sprites.h stars.h toppler.h leveledit.h txtsys.h \
configuration.h highscore.h menusys.h

scoredir = $(localstatedir)/@PACKAGE@
scorefile = $(scoredir)/@PACKAGE@.hsc
CPPFLAGS = @CPPFLAGS@ -Wall -DTOP_DATADIR=\"$(pkgdatadir)\" -DHISCOREDIR=\"$(scoredir)\" -DVERSION=\"@TOPPLER_VERSION@\"

noinst_PROGRAMS = toppler

toppler_SOURCES =                  \
  archi.cc         archi.h         \
  bonus.cc         bonus.h         \
  configuration.cc configuration.h \
  decl.cc          decl.h          \
  elevators.cc     elevators.h     \
  game.cc          game.h          \
  highscore.cc     highscore.h     \
  keyb.cc          keyb.h          \
  level.cc         level.h         \
  leveledit.cc     leveledit.h     \
  menu.cc          menu.h          \
  menusys.cc       menusys.h       \
  main.cc          config.h        \
  points.cc        points.h        \
  robots.cc        robots.h        \
  screen.cc        screen.h        \
  snowball.cc      snowball.h      \
  sound.cc         sound.h         \
  soundsys.cc      soundsys.h      \
  sprites.cc       sprites.h       \
  stars.cc         stars.h         \
  toppler.cc       toppler.h       \
  txtsys.cc        txtsys.h

pkgdata_DATA = \
  toppler.dat \
  m1.ttm m2.ttm ball1.ttm ball2.ttm ball3.ttm abc.ttm \
  toppler.xpm

man_MANS = toppler.6
EXTRA_DIST = $(man_MANS) $(datafiles) $(headerfiles) VERSION toppler.spec.in toppler.spec toppler.desktop.in \
 config.sub config.guess toppler.xpm

install_exec_local:
	-(chgrp games $(DESTDIR)$(bindir)/toppler; chmod g+s $(DESTDIR)$(bindir)/toppler)
	$(INSTALL) -d -g games -m 664 $(DESTDIR)$(scoredir)
	touch $(DESTDIR)$(scorefile)
	-(chgrp games $(DESTDIR)$(scorefile); chmod g+rw $(DESTDIR)$(scorefile))
	-(chgrp games $(DESTDIR)$(scoredir); chmod g+rw $(DESTDIR)$(scoredir))


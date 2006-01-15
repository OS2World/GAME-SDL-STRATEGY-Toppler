include $(top_srcdir)/Makefile.tools

AM_CXXFLAGS = -Wall -DTOP_DATADIR=\"$(pkgdatadir)\" -DHISCOREDIR=\"$(pkglocalstatedir)\" -DLOCALEDIR=\"$(datadir)\"
SUBDIRS = po m4
ACLOCAL_AMFLAGS = -I m4

PACKAGE = toppler

bin_PROGRAMS = toppler

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
  txtsys.cc        txtsys.h        \
  qnxicon.c

dist_pixmaps_DATA = $(PACKAGE).xpm
applications_DATA = $(PACKAGE).desktop
dist_man_MANS = toppler.6
dist_pkgdoc_DATA = AUTHORS COPYING ChangeLog NEWS README
dist_pkgdata_DATA = \
  toppler.dat \
  m1.ttm m2.ttm ball1.ttm ball2.ttm ball3.ttm abc.ttm pasi2.ttm

dist_pkglocalstate_DATA = $(PACKAGE).hsc

EXTRA_DIST = \
  config.rpath mkinstalldirs Makefile.tools \
  $(PACKAGE).spec \
  VERSION levelnames.txt \
  toppler.qpg

install-exec-hook:
	-chgrp games $(DESTDIR)$(bindir)/toppler
	-chmod 2755 $(DESTDIR)$(bindir)/toppler

install-data-hook:
	-chgrp games $(DESTDIR)$(pkglocalstatedir)/$(PACKAGE).hsc
	-chmod 0664 $(DESTDIR)$(pkglocalstatedir)/$(PACKAGE).hsc

# Tower Toppler
#
# Tower Toppler is the legal property of its developers, whose
# names are listed in the AUTHORS file, which is included
# within the source distribution.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA

PREFIX = /usr
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share
STATEDIR = $(PREFIX)/var/toppler
LOCALEDIR = $(PREFIX)/locale
DESTDIR =
CROSS =

CONVERT = convert
CXX = $(CROSS)$(CXX_NATIVE)
CXX_NATIVE = g++
CXXFLAGS = -Wall -Wextra -g -O2 -std=c++17
EXEEXT = $(if $(filter %-w64-mingw32.static-,$(CROSS)),.exe,)
INSTALL = install
LDFLAGS =
MSGFMT = msgfmt
MSGMERGE = msgmerge
PKG_CONFIG = $(CROSS)$(PKG_CONFIG_NATIVE)
PKG_CONFIG_NATIVE = pkg-config
POVRAY = povray
XGETTEXT = xgettext
GIMP = gimp

MSGID_BUGS_ADDRESS := roever@users.sf.net

.DELETE_ON_ERROR:

.PHONY: default
default: all

VERSION := $(shell cat src/version)
ALL_SOURCES := $(wildcard src/*)

PKGS += SDL_mixer
PKGS += libpng
PKGS += sdl
PKGS += zlib

PKGS_NATIVE += zlib
PKGS_NATIVE += sdl
PKGS_NATIVE += libpng
PKGS_NATIVE += SDL_image

PKG_CFLAGS_NATIVE = $$($(PKG_CONFIG_NATIVE) --cflags $(PKGS_NATIVE))
PKG_LIBS_NATIVE = $$($(PKG_CONFIG_NATIVE) --libs $(PKGS_NATIVE))

DEFS += -DVERSION='"$(VERSION)"'
DEFS += -DTOP_DATADIR='"$(DATADIR)"'
DEFS += -DHISCOREDIR='"$(STATEDIR)"'
DEFS += -DENABLE_NLS=1
DEFS += -DLOCALEDIR='"$(LOCALEDIR)"'

FILES_H := $(wildcard src/*.h)
FILES_CPP := $(wildcard src/*.cc)
FILES_O := $(patsubst src/%,_build/$(CROSS)/%.o,$(FILES_CPP))
.SECONDARY: $(FILES_O)
_build/$(CROSS)/%.o: src/% src/version $(FILES_H)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $$($(PKG_CONFIG) --cflags $(PKGS)) $(DEFS) -c -o $@ $<

FILES_BINDIR += $(CROSS)toppler$(EXEEXT)
$(CROSS)toppler$(EXEEXT): $(FILES_O)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(FILES_O) $$($(PKG_CONFIG) --libs $(PKGS)) $(LIBS)

# the rules for the datafile

SOUNDS = datafile/sounds/alarm.wav datafile/sounds/boing.wav datafile/sounds/bubbles.wav datafile/sounds/fanfare.wav \
         datafile/sounds/hit.wav datafile/sounds/honk.wav datafile/sounds/rumble.wav datafile/sounds/score.wav \
         datafile/sounds/sonar.wav datafile/sounds/splash.wav datafile/sounds/swoosh.wav datafile/sounds/tap.wav \
         datafile/sounds/tick.wav datafile/sounds/torpedo.wav datafile/sounds/water.wav

DATFILES += $(SOUNDS)

#-------------------------------------------------------#
# rules to create the data files necesary for the cross #
#-------------------------------------------------------#
DATFILES += _build/cross.dat
.SECONDARY: _build/cross.dat
_build/cross.dat: _build/tools/cross _build/tools/assembler _build/tools/colorreduction datafile/cross_pov/cross.pov datafile/cross_pov/cross.ini datafile/sprites_pov/environment.pov
	( cd _build && mkdir -p cross_pov )
	( cd _build/cross_pov && $(POVRAY) ../../datafile/cross_pov/cross.ini +L../../datafile/cross_pov -D )
	( cd _build && ./tools/assembler vm cross_rgb cross_pov/*.png )
	( cd _build && ./tools/colorreduction cross_rgb_mask.png 256 cross_mask.png )
	( cd _build && ./tools/colorreduction cross_rgb_colors.png 256 cross_colors.png )
	( cd _build && ./tools/cross )

#------------------------------------------------------#
# rules to create the data files necesary for the font #
#------------------------------------------------------#
DATFILES += _build/font.dat
.SECONDARY: _build/font.dat
_build/font.dat: _build/tools/font datafile/font.xcf _build/tools/colorreduction
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/font.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"font_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/font.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"font_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && ./tools/colorreduction font_colors_rgb.png 256 font_colors.png )
	( cd _build && ./tools/colorreduction font_mask_rgb.png 256 font_mask.png )
	( cd _build && ./tools/font )

#----------------------------------------------------------#
# rules to create the data files necesary for the graphics #
#----------------------------------------------------------#
DATFILES += _build/graphics.dat
.SECONDARY: _build/graphics.dat
_build/graphics.dat: _build/tools/graphics datafile/graphics_brick.xcf
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/graphics_brick.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"graphics_brick.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/graphics_pinacle.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"graphics_pinacle.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && ./tools/graphics )

#-------------------------------------------------------#
# rules to create the data files necesary for the mennu #
#-------------------------------------------------------#
DATFILES += _build/menu.dat
MENULEVELS_DIR=datafile/levels/mission1
MENULEVELS=$(wildcard ${MENULEVELS_DIR}/*)
.SECONDARY: _build/menu.dat
_build/menu.dat: _build/tools/menu _build/tools/tower2inc _build/tools/colorreduction ${MENULEVELS}
	( cd _build && mkdir -p menu_pov )
	( cd _build/menu_pov && ../tools/tower2inc "turm%i.inc" ../.. $(MENULEVELS) )
	( cd _build/menu_pov && $(POVRAY) ../../datafile/menu_pov/menu.ini +L../../datafile/menu_pov -D )
	( cd _build && ./tools/colorreduction menu_pov/menu_rgb.png 256 menu.png )
	( cd _build && ./tools/menu )

#----------------------------------------------------------#
# rules to create the data files necesary for the scroller #
#----------------------------------------------------------#
DATFILES += _build/scroller.dat
.SECONDARY: _build/scroller.dat
_build/scroller.dat: _build/tools/scroller datafile/scroller.xcf _build/tools/colorreduction
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"scroller1_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"scroller2_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 2) \"scroller3_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 3) \"scroller1_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 4) \"scroller2_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/scroller.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 5) \"scroller3_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && ./tools/colorreduction scroller3_mask_rgb.png 256 scroller3_mask.png )
	( cd _build && ./tools/colorreduction scroller2_mask_rgb.png 256 scroller2_mask.png )
	( cd _build && ./tools/colorreduction scroller1_mask_rgb.png 256 scroller1_mask.png )
	( cd _build && ./tools/colorreduction scroller3_colors_rgb.png 256 scroller3_colors.png )
	( cd _build && ./tools/colorreduction scroller2_colors_rgb.png 256 scroller2_colors.png )
	( cd _build && ./tools/colorreduction scroller1_colors_rgb.png 256 scroller1_colors.png )
	( cd _build && ./tools/scroller 3 2 1/1 scroller3_colors.png scroller3_mask.png 0/0/640 1/2 \
                       scroller2_colors.png scroller2_mask.png 0/0/640 1/1 \
                       scroller1_colors.png scroller1_mask.png 0/0/640 2/1 )

#---------------------------------------------------------#
# rules to create the data files necesary for the sprites #
#---------------------------------------------------------#
DATFILES += _build/sprites.dat
.SECONDARY: _build/sprites.dat
_build/sprites.dat: _build/tools/sprites _build/tools/assembler _build/tools/colorreduction \
             datafile/sprites_pov/box/obj.pov datafile/sprites_pov/box/obj.ini \
             datafile/sprites_pov/balls/obj.pov datafile/sprites_pov/balls/obj.ini \
             datafile/sprites_pov/snowball/obj.pov datafile/sprites_pov/snowball/obj.ini \
             datafile/sprites_pov/robot0/obj.pov datafile/sprites_pov/robot0/obj.ini \
             datafile/sprites_pov/robot1/obj.pov datafile/sprites_pov/robot1/obj.ini \
             datafile/sprites_pov/robot2/obj.pov datafile/sprites_pov/robot2/obj.ini \
             datafile/sprites_pov/robot3/obj.pov datafile/sprites_pov/robot3/obj.ini \
             datafile/sprites_pov/robot4/obj.pov datafile/sprites_pov/robot4/obj.ini \
             datafile/sprites_pov/robot5/obj.pov datafile/sprites_pov/robot5/obj.ini \
             datafile/sprites_pov/robot6/obj.pov datafile/sprites_pov/robot6/obj.ini \
             datafile/sprites_pov/robot7/obj.pov datafile/sprites_pov/robot7/obj.ini \
             datafile/sprites_pov/environment.pov \
             $(wildcard datafile/fish/render/*.tga) \
             $(wildcard datafile/submarine/render/*.tga) \
	     datafile/sprites_torpedo_colors.png datafile/sprites_torpedo_mask.png
	( cd _build && mkdir -p sprites_pov  )
	( cd _build/sprites_pov && mkdir -p robot7 && cd robot7 && $(POVRAY) ../../../datafile/sprites_pov/robot7/obj.ini +L../../../datafile/sprites_pov/robot7 -D )
	( cd _build/sprites_pov && mkdir -p robot6 && cd robot6 && $(POVRAY) ../../../datafile/sprites_pov/robot6/obj.ini +L../../../datafile/sprites_pov/robot6 -D )
	( cd _build/sprites_pov && mkdir -p robot5 && cd robot5 && $(POVRAY) ../../../datafile/sprites_pov/robot5/obj.ini +L../../../datafile/sprites_pov/robot5 -D )
	( cd _build/sprites_pov && mkdir -p robot4 && cd robot4 && $(POVRAY) ../../../datafile/sprites_pov/robot4/obj.ini +L../../../datafile/sprites_pov/robot4 -D )
	( cd _build/sprites_pov && mkdir -p robot3 && cd robot3 && $(POVRAY) ../../../datafile/sprites_pov/robot3/obj.ini +L../../../datafile/sprites_pov/robot3 -D )
	( cd _build/sprites_pov && mkdir -p robot2 && cd robot2 && $(POVRAY) ../../../datafile/sprites_pov/robot2/obj.ini +L../../../datafile/sprites_pov/robot2 -D )
	( cd _build/sprites_pov && mkdir -p robot1 && cd robot1 && $(POVRAY) ../../../datafile/sprites_pov/robot1/obj.ini +L../../../datafile/sprites_pov/robot1 -D )
	( cd _build/sprites_pov && mkdir -p robot0 && cd robot0 && $(POVRAY) ../../../datafile/sprites_pov/robot0/obj.ini +L../../../datafile/sprites_pov/robot0 -D )
	( cd _build && ./tools/assembler hm sprites_pov/robot7_rgb sprites_pov/robot7/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot6_rgb sprites_pov/robot6/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot5_rgb sprites_pov/robot5/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot4_rgb sprites_pov/robot4/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot3_rgb sprites_pov/robot3/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot2_rgb sprites_pov/robot2/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot1_rgb sprites_pov/robot1/*.png )
	( cd _build && ./tools/assembler hm sprites_pov/robot0_rgb sprites_pov/robot0/*.png )
	( cd _build && ./tools/assembler v sprites_robots_rgb sprites_pov/robot*_rgb_colors.png sprites_pov/robot*_rgb_mask.png )
	( cd _build && mv sprites_robots_rgb_colors.png sprites_robots_colors_rgb.png )
	( cd _build && mv sprites_robots_rgb_mask.png sprites_robots_mask_rgb.png )
	( cd _build/sprites_pov && mkdir -p snowball && cd snowball && $(POVRAY) ../../../datafile/sprites_pov/snowball/obj.ini  +L../../../datafile/sprites_pov/snowball -D )
	( cd _build/sprites_pov && mkdir -p balls && cd balls && $(POVRAY) ../../../datafile/sprites_pov/snowball/obj.ini  +L../../../datafile/sprites_pov/balls -D )
	( cd _build/sprites_pov && mkdir -p box && cd box && $(POVRAY) ../../../datafile/sprites_pov/snowball/obj.ini +L../../../datafile/sprites_pov/box -D )
	( cd _build && ./tools/assembler hm sprites_balls_rgb sprites_pov/balls/obj*.png )
	( cd _build && ./tools/assembler hm sprites_box_rgb sprites_pov/box/obj*.png )
	( cd _build && ./tools/colorreduction sprites_pov/snowball/obj1.png 256 sprites_snowball_mask.png )
	( cd _build && ./tools/colorreduction sprites_pov/snowball/obj0.png 256 sprites_snowball_colors.png )
	( cd _build && ./tools/colorreduction sprites_box_rgb_mask.png 256 sprites_box_mask.png )
	( cd _build && ./tools/colorreduction sprites_box_rgb_colors.png 256 sprites_box_colors.png )
	( cd _build && ./tools/colorreduction sprites_balls_rgb_mask.png 256 sprites_balls_mask.png )
	( cd _build && ./tools/colorreduction sprites_balls_rgb_colors.png 256 sprites_balls_colors.png )
	( cd _build && ./tools/colorreduction sprites_robots_mask_rgb.png 256 sprites_robots_mask.png )
	( cd _build && ./tools/colorreduction sprites_robots_colors_rgb.png 256 sprites_robots_colors.png )
	( mkdir -p _build/fish && ln -sf ../../datafile/fish/render _build/fish/render )
	( mkdir -p _build/submarine && ln -sf ../../datafile/submarine/render _build/submarine/render )
	( cp datafile/sprites_torpedo_* _build )
	( cd _build && ./tools/sprites )

#-------------------------------------------------------#
# rules to create the data files necesary for the title #
#-------------------------------------------------------#
DATFILES += _build/titles.dat
.SECONDARY: _build/titles.dat
_build/titles.dat: _build/tools/titles datafile/titles.xcf _build/tools/colorreduction
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/titles.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"titles_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/titles.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"titles_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && ./tools/colorreduction titles_colors_rgb.png 256 titles_colors.png )
	( cd _build && ./tools/colorreduction titles_mask_rgb.png 256 titles_mask.png )
	( cd _build && ./tools/titles )

#-------------------------------------------------------#
# rules to create the data files necesary for the dude  #
#-------------------------------------------------------#
DATFILES += _build/dude.dat
.SECONDARY: _build/dude.dat
_build/dude.dat: _build/tools/dude datafile/dude.xcf _build/tools/colorreduction
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/dude.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 1) \"dude_colors_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && $(GIMP) -i -b "(let* ((image (car(gimp-xcf-load 1 \"../datafile/dude.xcf\" \"ttt\")))(layers (cadr(gimp-image-get-layers image))))(file-png-save-defaults 1 image (aref layers 0) \"dude_mask_rgb.png\" \"ttt\")(gimp-quit 1))" )
	( cd _build && ./tools/colorreduction dude_colors_rgb.png 256 dude_colors.png )
	( cd _build && ./tools/colorreduction dude_mask_rgb.png 256 dude_mask.png )
	( cd _build && ./tools/dude )

#-------------------------------------------------------#
# rules to create the missions                          #
#-------------------------------------------------------#

# RULE FOR MISSION 1

DATFILES += _build/m1.ttm
.SECONDARY: _build/m1.ttm
_build/m1.ttm: _build/tools/cremission \
	datafile/levels/mission1/m1t1 datafile/levels/mission1/m1t2 datafile/levels/mission1/m1t3 datafile/levels/mission1/m1t4 datafile/levels/mission1/m1t5 datafile/levels/mission1/m1t6 datafile/levels/mission1/m1t7 datafile/levels/mission1/m1t8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Mission 1" ../m1 10 \
		../../datafile/levels/mission1/m1t1 \
		../../datafile/levels/mission1/m1t2 \
		../../datafile/levels/mission1/m1t3 \
		../../datafile/levels/mission1/m1t4 \
		../../datafile/levels/mission1/m1t5 \
		../../datafile/levels/mission1/m1t6 \
		../../datafile/levels/mission1/m1t7 \
		../../datafile/levels/mission1/m1t8 )

# RULE FOR MISSION 2

DATFILES += _build/m2.ttm
.SECONDARY: _build/m2.ttm
_build/m2.ttm: _build/tools/cremission \
	datafile/levels/mission2/m2t1 datafile/levels/mission2/m2t2 datafile/levels/mission2/m2t3 datafile/levels/mission2/m2t4 datafile/levels/mission2/m2t5 datafile/levels/mission2/m2t6 datafile/levels/mission2/m2t7 datafile/levels/mission2/m2t8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Mission 2" ../m2 11 \
		../../datafile/levels/mission2/m2t1 \
		../../datafile/levels/mission2/m2t2 \
		../../datafile/levels/mission2/m2t3 \
		../../datafile/levels/mission2/m2t4 \
		../../datafile/levels/mission2/m2t5 \
		../../datafile/levels/mission2/m2t6 \
		../../datafile/levels/mission2/m2t7 \
		../../datafile/levels/mission2/m2t8 )

# RULE FOR CLARENCE MISSION 1

DATFILES += _build/ball1.ttm
.SECONDARY: _build/ball1.ttm
_build/ball1.ttm: _build/tools/cremission \
	datafile/levels/ball1/lev1 datafile/levels/ball1/lev2 datafile/levels/ball1/lev3 datafile/levels/ball1/lev4 datafile/levels/ball1/lev5 datafile/levels/ball1/lev6 datafile/levels/ball1/lev7 datafile/levels/ball1/lev8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Ball 1" ../ball1 13 \
		../../datafile/levels/ball1/lev1 \
		../../datafile/levels/ball1/lev2 \
		../../datafile/levels/ball1/lev3 \
		../../datafile/levels/ball1/lev4 \
		../../datafile/levels/ball1/lev5 \
		../../datafile/levels/ball1/lev6 \
		../../datafile/levels/ball1/lev7 \
		../../datafile/levels/ball1/lev8 )

# RULE FOR CLARENCE MISSION 2

DATFILES += _build/ball2.ttm
.SECONDARY: _build/ball2.ttm
_build/ball2.ttm: _build/tools/cremission \
	datafile/levels/ball2/lev1 datafile/levels/ball2/lev2 datafile/levels/ball2/lev3 datafile/levels/ball2/lev4 datafile/levels/ball2/lev5 datafile/levels/ball2/lev6 datafile/levels/ball2/lev7 datafile/levels/ball2/lev8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Ball 2" ../ball2 14 \
		../../datafile/levels/ball2/lev1 \
		../../datafile/levels/ball2/lev2 \
		../../datafile/levels/ball2/lev3 \
		../../datafile/levels/ball2/lev4 \
		../../datafile/levels/ball2/lev5 \
		../../datafile/levels/ball2/lev6 \
		../../datafile/levels/ball2/lev7 \
		../../datafile/levels/ball2/lev8 )

# RULE FOR CLARENCE MISSION 3

DATFILES += _build/ball3.ttm
.SECONDARY: _build/ball3.ttm
_build/ball3.ttm: _build/tools/cremission \
	datafile/levels/ball3/lev1 datafile/levels/ball3/lev2 datafile/levels/ball3/lev3 datafile/levels/ball3/lev4 datafile/levels/ball3/lev5 datafile/levels/ball3/lev6 datafile/levels/ball3/lev7 datafile/levels/ball3/lev8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Ball 3" ../ball3 15 \
		../../datafile/levels/ball3/lev1 \
		../../datafile/levels/ball3/lev2 \
		../../datafile/levels/ball3/lev3 \
		../../datafile/levels/ball3/lev4 \
		../../datafile/levels/ball3/lev5 \
		../../datafile/levels/ball3/lev6 \
		../../datafile/levels/ball3/lev7 \
		../../datafile/levels/ball3/lev8 )

# RULE FOR PASIS MISSION 1

DATFILES += _build/abc.ttm
.SECONDARY: _build/abc.ttm
_build/abc.ttm: _build/tools/cremission \
	datafile/levels/kallinen1/t1 datafile/levels/kallinen1/t2 datafile/levels/kallinen1/t3 datafile/levels/kallinen1/t4 datafile/levels/kallinen1/t5 datafile/levels/kallinen1/t6 datafile/levels/kallinen1/t7 datafile/levels/kallinen1/t8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "ABC Towers" ../abc 12 \
		../../datafile/levels/kallinen1/t1 \
		../../datafile/levels/kallinen1/t2 \
		../../datafile/levels/kallinen1/t3 \
		../../datafile/levels/kallinen1/t4 \
		../../datafile/levels/kallinen1/t5 \
		../../datafile/levels/kallinen1/t6 \
		../../datafile/levels/kallinen1/t7 \
		../../datafile/levels/kallinen1/t8 )

# RULE FOR PASIS MISSION 2

DATFILES += _build/pasi2.ttm
.SECONDARY: _build/pasi2.ttm
_build/pasi2.ttm: _build/tools/cremission \
	datafile/levels/kallinen2/t1 datafile/levels/kallinen2/t2
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Pasis 2 Towers" ../pasi2 12 \
		../../datafile/levels/kallinen2/t1 \
		../../datafile/levels/kallinen2/t2 )

# RULE FOR DAVIDS MISSION 1

DATFILES += _build/david1.ttm
.SECONDARY: _build/david1.ttm
_build/david1.ttm: _build/tools/cremission \
	datafile/levels/david1/lev1 datafile/levels/david1/lev2 datafile/levels/david1/lev3 datafile/levels/david1/lev4 datafile/levels/david1/lev5 datafile/levels/david1/lev6 datafile/levels/david1/lev7 datafile/levels/david1/lev8
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Challenge 1" ../david1 12 \
		../../datafile/levels/david1/lev1 \
		../../datafile/levels/david1/lev2 \
		../../datafile/levels/david1/lev3 \
		../../datafile/levels/david1/lev4 \
		../../datafile/levels/david1/lev5 \
		../../datafile/levels/david1/lev6 \
		../../datafile/levels/david1/lev7 \
		../../datafile/levels/david1/lev8 )

# RULE FOR DAVIDS MISSION 2

DATFILES += _build/david2.ttm
.SECONDARY: _build/david2.ttm
_build/david2.ttm: _build/tools/cremission \
	datafile/levels/david2/l1 datafile/levels/david2/l2 datafile/levels/david2/l3 datafile/levels/david2/l4
	( cd _build && mkdir -p levels && cd levels && ../tools/cremission "Challenge 2" ../david2 12 \
		../../datafile/levels/david2/l1 \
		../../datafile/levels/david2/l2 \
		../../datafile/levels/david2/l3 \
		../../datafile/levels/david2/l4 )

#-------------------------------------------------------#
# rules to create the tool programs                     #
#-------------------------------------------------------#
.SECONDARY: _build/tools/colorreduction
_build/tools/colorreduction: datafile/colorreduction.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/assembler
_build/tools/assembler: datafile/assembler.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/crearc
_build/tools/crearc: datafile/crearc.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/dude
_build/tools/dude: datafile/dude.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/titles
_build/tools/titles: datafile/titles.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/sprites
_build/tools/sprites: datafile/sprites.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/scroller
_build/tools/scroller: datafile/scroller.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/font
_build/tools/font: datafile/font.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/cross
_build/tools/cross: datafile/cross.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/graphics
_build/tools/graphics: datafile/graphics.c datafile/colorreduction.h datafile/pngsaver.h
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/menu
_build/tools/menu: datafile/menu.c
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/tower2inc
_build/tools/tower2inc: datafile/menu_pov/tower2inc.cc
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -g -Isrc -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

.SECONDARY: _build/tools/cremission
_build/tools/cremission: datafile/levels/cremission.cc
	@mkdir -p $(dir $@)
	$(CXX_NATIVE) $(CFLAGS) -g -Isrc -o $@ $< $(PKG_CFLAGS_NATIVE) $(PKG_LIBS_NATIVE)

#---------------------------------------------------------------------------------------#
# global make, this creates the final data file packing all things togethers using zlib #
#---------------------------------------------------------------------------------------#
FILES_BINDIR += toppler.dat
toppler.dat: _build/tools/crearc $(DATFILES)
	./_build/tools/crearc toppler.dat $(DATFILES) $(MISSIONS)


.PHONY: all
all: $(FILES_BINDIR)


# Tower Toppler
#
# Domino-Chain is the legal property of its developers, whose
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

DEFS += -DVERSION='"$(VERSION)"'
DEFS += -DTOP_DATADIR='"$(DATADIR)"'
DEFS += -DHISCOREDIR='"$(STATEDIR)"'

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

.PHONY: all
all: $(FILES_BINDIR)


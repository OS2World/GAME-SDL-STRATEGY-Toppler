#!/bin/sh

libtoolize --automake
aclocal
autoheader
automake --gnu --add-missing
autoconf
./configure $@

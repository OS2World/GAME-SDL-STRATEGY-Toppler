#!/bin/sh

for i in config.sub config.guess; do
	if test ! -e "$i"; then ln -s "/usr/share/automake/$i" .; fi
done

aclocal
autoheader
automake --gnu --add-missing
autoconf
./configure

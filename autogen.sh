#!/bin/bash

echo "$0: running autopoint ..."
autopoint

echo "$0: running post-treatments for autopoint ..."
(
	extra_dist="`cd m4/ && ls -1 *.m4 | tr '\n' ' '`"
	linguas="`cd po/ && ls -1 *.po | cut -d. -f1`"
	all_linguas="`echo "$linguas" | tr '\n' ' '`"
	tempfile="`mktemp -p.`"

	echo "Creating m4/Makefile.am"
	echo "EXTRA_DIST = $extra_dist" >m4/Makefile.am

	echo "Creating po/ChangeLog"
	>po/ChangeLog

	echo "Changing configure.ac"
	sed 's/\(ALL_LINGUAS="\)[^"]*/\1'"$all_linguas"'/' <configure.ac >"$tempfile" \
	&& cat <"$tempfile" >configure.ac

	rm -f "$tempfile"
)

echo "$0: running aclocal ..."
aclocal

echo "$0: running autoconf ..."
autoconf

echo "$0: running autoheader ..."
autoheader

echo "$0: running automake ..."
automake --gnu --add-missing

echo "$0: running configure ..."
./configure $@

#!/bin/bash

echo "$0: running autopoint ..."
autopoint || exit 1

echo "$0: running post-treatments for autopoint ..."
(
	extra_dist="`cd m4/ && ls -1 *.m4 | tr '\n' ' '`"
	linguas="`cd po/ && ls -1 *.po | cut -d. -f1`"
	all_linguas="`echo -n "$linguas" | tr '\n' ' '`"

	echo "Creating m4/Makefile.am"
	echo "EXTRA_DIST = $extra_dist" >m4/Makefile.am

	echo "Creating po/ChangeLog"
	>po/ChangeLog

	echo "Creating po/LINGUAS"
	echo -n "$linguas" >po/LINGUAS

	echo "Updating configure.ac (backup is in configure.ac~)"
	cp configure.ac configure.ac~
	sed 's/\(ALL_LINGUAS="\)[^"]*/\1'"$all_linguas"'/' <configure.ac~ >configure.ac
)

echo "$0: running aclocal ..."
aclocal || exit 1

echo "$0: running autoconf ..."
autoconf || exit 1

echo "$0: running autoheader ..."
autoheader || exit 1

echo "$0: running automake ..."
automake --gnu --add-missing || exit 1

echo "$0: running configure ..."
./configure $@ || exit 1

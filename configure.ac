#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.13)
AC_INIT
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([toppler],[`cd "$srcdir" && cat VERSION`])
AC_CONFIG_SRCDIR([toppler.cc])
AC_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

# To be removed in the future
AC_DEFINE_UNQUOTED(SYS_LINUX,0,Linux)
AC_DEFINE_UNQUOTED(SYS_WINDOWS,1,Windows)
AC_DEFINE_UNQUOTED(SYS_BEOS,2,BeOs)
AC_DEFINE_UNQUOTED(SYS_MACOSX,3,MacOSX)
AC_DEFINE_UNQUOTED(SYSTEM,SYS_LINUX,System)

# General information.
AC_SUBST(FULLNAME,"Tower Toppler")
AC_SUBST(URL,"http://toppler.sourceforge.net/")
AC_SUBST(DESCRIPTION,"Reimplementation of the old game (aka Nebulous). In the game you have to climb a tower with lots of strange inhabitants that try to push you down. Your only defence is a snowball you can throw and your skill to avoid these beings.")
AC_DEFINE_UNQUOTED([FULLNAME],["$FULLNAME"],[Define to the full name of this package.])
AC_DEFINE_UNQUOTED([URL],["$URL"],[Define to the URL of this package's homepage.])
AC_DEFINE_UNQUOTED([DESCRIPTION],["$DESCRIPTION"],[Define to the description of this package.])

# Compiler options.
CPPFLAGS="-Wall"

# Additional variables.
AC_SUBST(DLL_FILES,"")
AC_SUBST(DLL_FILES_CYGWIN,"/bin/cygwin1.dll")

# I18n.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.11.5)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
	AC_DEFINE_UNQUOTED(HAVE_SDL,[1],[Define to 1 if you have SDL.]),
	AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"
[ $HAVE_SDL ] && DLL_FILES="$DLL_FILES /bin/SDL.dll"

AC_CHECK_LIB(SDL_mixer, main, ,
	AC_MSG_WARN([Can't find SDL_mixer. Toppler will have no sound.]))
[ $HAVE_LIBSDL_MIXER ] && DLL_FILES="$DLL_FILES /bin/SDL_mixer.dll"

AC_CHECK_LIB(z, uncompress, ,
	AC_MSG_ERROR([Can't find libz. Toppler needs it to uncompress the data tile!]))
[ $HAVE_LIBZ ] && DLL_FILES="$DLL_FILES /bin/mgwz.dll"

AC_CHECK_LIB(msvcp60, mbrtowc)

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h langinfo.h libintl.h stdlib.h string.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit memmove memset mkdir setlocale strcasecmp strchr strstr])

# Output.
AC_CONFIG_FILES([Makefile toppler.spec toppler.nsi toppler.desktop po/Makefile.in m4/Makefile debian/control debian/copyright debian/menu])
AC_OUTPUT

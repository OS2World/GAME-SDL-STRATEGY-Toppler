#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.13)
AC_INIT
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([toppler],[`cat VERSION`])
AC_CONFIG_SRCDIR([toppler.cc])
AC_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE
CPPFLAGS="-Wall"

# To be removed in the future
AC_DEFINE_UNQUOTED(SYS_LINUX,0,Linux)
AC_DEFINE_UNQUOTED(SYS_WINDOWS,1,Windows)
AC_DEFINE_UNQUOTED(SYS_BEOS,2,BeOs)
AC_DEFINE_UNQUOTED(SYS_MACOSX,3,MacOSX)
AC_DEFINE_UNQUOTED(SYSTEM,SYS_LINUX,System)

# I18n.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.11.5)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :,
  AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_CHECK_LIB(SDL_mixer, main, ,
  AC_MSG_WARN([Can't find SDL_mixer. Toppler will have no sound.]))

AC_CHECK_LIB(z, uncompress, ,
  AC_MSG_ERROR([Can't find libz. Toppler needs it to uncompress the data tile!]))

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
AC_CONFIG_FILES([Makefile toppler.spec toppler.desktop po/Makefile.in m4/Makefile])
AC_OUTPUT

#!/bin/sh
# $Id: autogen.sh,v 1.1 2003-08-07 16:28:21 roebel Exp $
#
# $Log: not supported by cvs2svn $

rm -f aclocal.m4 config.* install-sh libtool ltconfig ltmain.sh missing mkinstalldirs
set -x

aclocal -I m4
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
autoconf
automake --foreign --add-missing --copy

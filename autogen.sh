#!/bin/sh
# $Id: autogen.sh,v 1.6 2003-08-07 16:28:45 roebel Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.5  2003/08/07 16:19:29  roebel
# Java include directories are determined automatically using m4 functions. perl include directory are determined with improved flexibility.
#
# Revision 1.4  2003/04/30 11:36:06  tisseran
# Started autoconfiscation of swig directory
#
# Revision 1.3  2003/04/03 12:39:45  schwarz
# Added conditional to prevent endless loop when directory SDIF doesn't exist.
#

rm -f aclocal.m4 config.* install-sh libtool ltconfig ltmain.sh missing mkinstalldirs
set -x

#aclocal -I m4
aclocal 
# On Mac OS X, libtoolize is glibtoolize
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
autoconf
automake --foreign --add-missing --copy
(
    cd SDIF  &&  ./autogen.sh
)
(
    cd swig &&  ./autogen.sh
)

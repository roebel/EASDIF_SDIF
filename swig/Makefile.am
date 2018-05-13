# $Id: Makefile.am,v 1.11 2006-12-12 16:29:11 roebel Exp $
#
# eaSDIF - swig/Makefile.am
#
# Main makefile for swig wrapper generation for eaSDIF C++ library.
#
# $Log: not supported by cvs2svn $
# Revision 1.10  2006/12/10 17:34:46  roebel
# Added swig sources into distribution.
#
# Revision 1.9  2006/11/26 22:49:24  roebel
# Fixed to use local m4 directory.
#
# Revision 1.8  2006/11/26 20:38:30  roebel
# Reordered sub directories according to stability and cross platform support.
#
# Revision 1.7  2006/11/26 16:52:20  roebel
# fixed typo in variable name.
#
# Revision 1.6  2006/11/26 16:51:29  roebel
# Fixed configuration where java compiler is not available.
#
# Revision 1.5  2006/11/26 16:12:18  roebel
# Disable java subdir if not java compiler is not available.
#
# Revision 1.4  2003/06/04 20:40:43  schwarz
# Added targets to make just one language.
# Moved java to back because it doesn't work at the moment.
#

ACLOCAL_AMFLAGS = -I m4


if MAKE_JAVA
JAVADIR = java
else
JAVADIR =
endif

SUBDIRS = python perl5 $(JAVADIR)  
DIST_SUBDIRS = python perl5 java   m4
EXTRA_DIST = CMakeLists.txt    easdif-common.i.in  easdif-common-init.i  easdif-defines.i  README   sdif-common.i.in


$(SUBDIRS):
	$(MAKE) -C $@

# make only one language
perl:
	$(MAKE) -C perl5


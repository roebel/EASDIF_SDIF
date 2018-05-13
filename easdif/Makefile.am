# $Id: Makefile.am,v 1.17 2006-12-10 17:54:40 roebel Exp $
#
# ./Easdif/src/Makefile.am	Feb. 2003 Axel Roebel
#
# Source file for GNU automake, generates Makefile.in.
#

#  This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
# 
#  Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
# 
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
# 
#  See file COPYING for further informations on licensing terms.
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# 
#  The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
# 


# $Log: not supported by cvs2svn $
# Revision 1.16  2006/04/22 08:57:24  roebel
# Renamed some files to prevent name clash of object files on macosx
#
# Revision 1.15  2005/05/30 21:43:01  roebel
# Changed all include files from .h into .hpp to prevent name clash between
# sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
# case insensitive.
#
# Revision 1.14  2005/05/20 21:31:49  roebel
# Added proper dependency on libsdif_cpp.la.
#
# Revision 1.13  2004/03/19 01:03:26  roebel
# Finalized support for --enable-ddebug
#
# Revision 1.12  2003/08/07 11:59:42  tisseran
# Fixed content of EXTRA_DIST variable.
#
# Revision 1.11  2003/07/09 21:19:49  roebel
# Provide configure switch to add gccversion to library name to distinguish
# gcc3.2 from gcc2.96 compiled libraries.
#
# Revision 1.10  2003/05/22 21:22:53  roebel
# Fixed support for configuring and compiling in directories other than the source tree.
#
# Revision 1.9  2003/05/19 13:55:47  roebel
# Added new header easdif_config.h.in .
#
# Revision 1.8  2003/04/09 11:48:40  roebel
# Added missing easdifintro.h to dsitributed files.
#
# Revision 1.7  2003/04/08 18:29:39  roebel
# Fixed version numbering in shared library.
#
# Revision 1.6  2003/04/06 16:31:08  roebel
# Added license info
#
# Revision 1.5  2003/03/18 15:37:11  roebel
# Use libtool for conveniance library
#
# Revision 1.4  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.3  2003/03/05 15:33:13  roebel
# Fixed installation of include files.
#
# Revision 1.2  2003/03/04 00:55:02  roebel
# Removed version determination
#
# Revision 1.1  2003/03/03 19:00:16  roebel
# Moved src directory to new name easdif
#
# Revision 1.1  2003/03/03 10:03:50  roebel
# Adapted for autoconf/automake...
#

# EXTRA_LIBRARIES to provide knowledge about possible substitutions
# in lib_LTLIBRARIES
EXTRA_LTLIBRARIES = libEasdif.la  libEasdif_gcc3_2.la libEasdif-debug.la

# lib_LIBRARIES = @SELLIB@
lib_LTLIBRARIES = @SELLIB@

EXTRA_DIST = easdif_version.hpp.in easdifintro.hpp.in easdif_config.hpp.in CMakeLists.txt

EASDIF_H = easdif_version.hpp easdif.h  sdifentity.hpp \
	sdifexception.hpp sdif_frame.hpp sdifinit.hpp sdif_matrix.hpp sdifmatrixdata.hpp \
	sdifmatrixdatainterface.hpp sdifnamevaluetable.hpp

easdifincdir = $(prefix)/include/easdif
easdifinc_HEADERS = $(EASDIF_H)

libEasdif_la_SOURCES =  sdifentity.cpp sdifexception.cpp sdif_frame.cpp \
			sdifinit.cpp sdif_matrix.cpp sdifnamevaluetable.cpp

libEasdif_la_LIBADD  = @SDIFCPPLIB@
libEasdif_la_LDFLAGS = -no-undefined  -release @VERSION@ -rpath $(libdir)
libEasdif_la_DEPENDENCIES = @SDIFCPPLIB@


libEasdif_debug_la_SOURCES =  sdifentity.cpp sdifexception.cpp sdif_frame.cpp \
			sdifinit.cpp sdif_matrix.cpp sdifnamevaluetable.cpp

libEasdif_debug_la_LIBADD  = @SDIFCPPLIB@
libEasdif_debug_la_LDFLAGS = -no-undefined  -release @VERSION@ -rpath $(libdir)
libEasdif_debug_la_DEPENDENCIES = @SDIFCPPLIB@

libEasdif_gcc3_2_la_SOURCES =  sdifentity.cpp sdifexception.cpp sdif_frame.cpp \
                        sdifinit.cpp sdif_matrix.cpp sdifnamevaluetable.cpp

libEasdif_gcc3_2_la_LIBADD  = @SDIFCPPLIB@
libEasdif_gcc3_2_la_LDFLAGS = -no-undefined  -release @VERSION@ -rpath $(libdir)
libEasdif_gcc3_2_la_DEPENDENCIES = @SDIFCPPLIB@


INCLUDES = -I.. -I$(top_srcdir) -I../SDIF/include -I$(top_srcdir)/SDIF/include -I../SDIF/sdif -I$(top_srcdir)/SDIF/sdif   -DUSE_CONFIGURE


#
# EXTRA rules and dependencies to generate easdif_version.h
#
#$(top_srcdir)/src/easdif_version.h: $(top_srcdir)/Doxyfile
#	echo "/* Version automatically generated from Makefile/DoxyFile" >$@.h
#	echo " * do not edit */" >>$@.h
#	echo "#ifndef EASDIF_VERSION_MAJ"	 >>$@.h
#	echo "#define" EASDIF_VERSION_MAJ $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f1)) >>$@.h
#	echo "#define" EASDIF_VERSION_MIN $(strip $(shell grep ^PROJECT_NUMBER  $< | cut -d"=" -f2| cut -d. -f2)) >>$@.h
#	echo "#endif "	 >>$@.h



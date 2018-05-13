# $Id: Makefile.am,v 1.22 2006-12-10 17:53:54 roebel Exp $
#
# ./Makefile.am		Feb. 2003     Axel Roebel
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
# Revision 1.21  2006/11/26 16:45:07  roebel
# No automatic configuration in swug subdir.
#
# Revision 1.20  2005/05/03 16:28:30  roebel
# Fixed ircam doc install target.
#
# Revision 1.19  2005/05/03 16:24:03  roebel
# Fixed documentation installation target.
#
# Revision 1.18  2004/07/28 14:59:25  roebel
# Version 0.10.0:
# Matrix::SetCol/SetRow and unsigned char element access.
# New pkg-config for debug version of Easdif.
#
# Revision 1.17  2004/07/13 15:28:02  roebel
# Fixed cvstag target to be able to use it from working directories that are not the top
# source directory.
#
# Revision 1.16  2003/11/18 12:31:37  roebel
# Moved documentation install server to maelzel replacement.
#
# Revision 1.15  2003/05/02 13:39:25  roebel
# Reinvented missing ircam_install_doc target.
#
# Revision 1.14  2003/04/10 14:06:03  roebel
# Change name of doc directory in doc distribution to Easdif-VERSION.doc.
#
# Revision 1.13  2003/04/09 16:54:00  roebel
# Because Easdif has its own subdir in the download directory
# no need to map filenames.
#
# Revision 1.12  2003/04/09 16:33:44  roebel
# Changed publish directory to be special sub directory for Easdif.
#
# Revision 1.11  2003/04/09 16:19:42  roebel
# Directory was still wrong.
#
# Revision 1.10  2003/04/09 16:10:18  roebel
# Put final publish directory into place.
#
# Revision 1.9  2003/04/09 12:30:41  roebel
# Changed default configuration back to enable-shared but added
# README.MacOSX to announce problems with shared libraries to users.
#
# Revision 1.8  2003/04/09 11:55:29  roebel
# fixed PUBLISH_HOST
#
# Revision 1.7  2003/04/08 23:22:34  roebel
# Added determination of SDIF version
#
# Revision 1.6  2003/04/08 18:27:36  roebel
# Added doc tar to published files.
# Changed name of published tar to include SDIF with version.
# Configured PUBLISH_HOST
# Rename conflicting files that have common names in SDIF and Easdif
# to coexist in download directory.
#
# Revision 1.5  2003/04/06 16:47:51  roebel
# Added copyright notice.
#
# Revision 1.4  2003/04/03 13:15:40  schwarz
# Added doc and test targets.
#
# Revision 1.3  2003/03/07 17:38:41  roebel
# Updated for correct handling of distribution and new layout of c++ compilation in SDIF.
#
# Revision 1.2  2003/03/04 00:50:01  roebel
# Changed src difrectory to easdif, fixed distribution of SDIF
#
# Revision 1.1  2003/03/03 10:03:24  roebel
# Adapted for autoconf/automake...
#

SUBDIRS =  SDIF easdif  test 
DIST_SUBDIRS =  SDIF easdif  test swig
EXTRA_DIST = Doxyfile.in README.MacOSX CMakeLists.txt

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = Easdif@PKGDEBUG@.pc

doc:
	doxygen Doxyfile


# only useful for IRCAM
IRCAMDOCDIR = /var/www/html/analyse_synthese/documentation/easdif
IRCAMDOCSERVER = iii.ircam.fr

ircam_install_doc: doc
	chmod -R g+w doc/html
	ncftpput -u $(USER) $(IRCAMDOCSERVER) $(IRCAMDOCDIR)  doc/html/*
#	rsync -avpz --delete doc/html/* $(IRCAMDOCSERVER):$(IRCAMDOCDIR)
	chmod -R g-w doc/html

test:	check

grep:
	grep $a easdif/*.cpp easdif/*.h /dev/null


DOCTAR = $(PACKAGE)-$(VERSION).doc.tgz

pubdoc: doc
	cp -r doc   $(PACKAGE)-$(VERSION).doc
	tar -cvzf $(DOCTAR) $(PACKAGE)-$(VERSION).doc
	rm -rf $(PACKAGE)-$(VERSION).doc
#
# DISTRIBUTION
#

# base name of distribution (override automake generated variable to make source distribution!)
SDIFVERSION=$(shell grep "\#define VERSION" SDIF/sdif/config.h | cut -d\" -f2)

distdir		  = $(PACKAGE)-$(VERSION)-SDIF-$(SDIFVERSION)-src
PUBLISH_FILE      = $(distdir).tar.gz

#
# PUBLISH (on server)
#

# use this to check if the distribution works, tag the cvs project, 
# make a source distribution and rpm, and copy it to the web server
publish:	distcheck cvstag dist pubdoc publish-ftp 

# more files to publish (on server)
EXTRA_PUBLISH     = $(DOCTAR)

# call this as "make PUBLISH_HOST=yourwebhost publish" to publish a new version
# this might work differently for other places
# todo: configure PUBLISH_HOST
PUBLISH_HOST      = www.ircam.fr
PUBLISH_USER      = wwwas
PUBLISH_DIR       = sdif/download/Easdif
PUBLISH_FILES     = $(PUBLISH_FILE) $(PUBLISH_RPM) $(EXTRA_PUBLISH) COPYING INSTALL README AUTHORS ChangeLog
#PUBLISH_FILES_MAP = COPYING INSTALL README AUTHORS ChangeLog

publish-ftp:	# (experimental)
	echo -e put $(PUBLISH_FILES)  | ncftp -u$(PUBLISH_USER) -p$(PW) ftp://$(PUBLISH_HOST)/$(PUBLISH_DIR)

#	for ii in  $(PUBLISH_FILES_MAP);do echo put -z $$ii $$ii.Easdif; done|ncftp -u$(PUBLISH_USER) -p$(PW) ftp://$(PUBLISH_HOST)/$(PUBLISH_DIR)



# tag the cvsrepository with the current version 
# (to exactly reconstitute a published version)
CVSTAG	      = $(shell echo $(PACKAGE)_lib_version_$(VERSION) | tr . _)
cvstag:
		 cd @top_srcdir@; cvs tag -F $(CVSTAG)


.PHONY:		grep doc ircam_install_doc test publish pubdoc publish-ftp  cvstag


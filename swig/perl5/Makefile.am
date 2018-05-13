# $Id: Makefile.am,v 1.7 2006-12-10 17:43:48 roebel Exp $
#
# $Log: not supported by cvs2svn $
# Revision 1.6  2006/11/26 20:35:08  roebel
# Use automatic directory names.
#
# Revision 1.5  2006/11/26 16:15:42  roebel
# Reorganized to work with swig 1.3.31!
#
# Revision 1.4  2006/10/17 10:36:45  roebel
# Fixed out of source directory configuration and compilation.
#
# Revision 1.3  2005/07/25 13:29:24  roebel
# Adapted for new include file names - h->hpp.
#
# Revision 1.2  2003/08/07 16:19:29  roebel
# Java include directories are determined automatically using m4 functions. perl include directory are determined with improved flexibility.
#
# Revision 1.1  2003/04/30 11:31:35  tisseran
# Updated file hierarchy to be conform with Diemo README.
# Start autoconfiscation for perl module
#
# Revision 1.11  2003/04/29 16:27:47  schwarz
# debug on
#
# Revision 1.10  2003/04/18 17:45:23  schwarz
# Renamed sdifsignature.i to sdiftypemap.i, because all types are in there.
# Include typemap for std::string from SWIG library
# Added typemap freearg SdifStringT *
#
# Revision 1.9  2003/04/18 16:00:22  schwarz
# Generate separate large module SDIF.pm with all
# low-level sdif lib wrappers.
#
# Revision 1.8  2003/04/18 15:41:18  schwarz
# Don't include all definitions from sdif.h, only the ones needed by
# easdif, defined in sdifdefine.i.  -> smaller wrapper size:
# .cxx	 880k -> 130k
# .o	1000k -> 210k
# .pm	 123k -> 8k
#
# Revision 1.7  2003/04/18 14:31:27  schwarz
# Added typemap for string to SdifSignature conversion (input arg):
# Setting/writing data works --> wrapper complete, but rudimentary.
#
# Revision 1.6  2003/04/17 12:20:39  schwarz
# Output (return value) mapping for SdifSignature works!
#
# Revision 1.5  2003/04/17 11:15:01  schwarz
# Swigging sdif.h works (with some warnings), but that's not yet it...
#
# Revision 1.4  2003/04/17 11:03:52  schwarz
# Swigged all easdif classes!
# Now going on to map sdif basic types.
#
# Revision 1.3  2003/04/17 10:05:48  schwarz
# The same with shadow classes (= proper memory management and methods).
#
# Revision 1.2  2003/04/17 09:54:12  schwarz
# Test target works when path to easdif shared lib is given.
#
# Revision 1.1  2003/04/17 09:51:10  schwarz
# First working swig generated perl extension!
# Classes re-declared in easdif.i, simple method without shadow classes.
# Doesn't do much except counting frames.

EXTRA_DIST = CMakeLists.txt easdif-perl5.i test.pl test.pl.in sdiftypemap-perl5.i

PERL5_INCLUDE	= /usr/lib/perl5/$(shell perl -MConfig -e 'print $$Config{version}')/$(shell perl -MConfig -e 'print $$Config{archname}')/CORE/
INCL		= -I$(PERL5_INCLUDE) -I$(top_builddir)/.. -I$(top_srcdir)/.. -I$(top_builddir)/../SDIF/include -I$(top_srcdir)/../SDIF/include 
EASDIF_LIB_PATH	= $(top_builddir)/../easdif/.libs
dbflag = -g


modules		= eaSDIF SDIF
incfiles	= ../easdif-defines.i sdiftypemap-perl5.i
easdifincbase	= entity _frame _matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=$(top_srcdir)/../easdif/sdif%.hpp)
sdifincfiles	= $(top_srcdir)/../SDIF/include/sdif.h

define swig
	swig -c++ -perl5 -shadow $(INCL) -o $@ $<
endef


all:	eaSDIF.so #SDIF.so

eaSDIF_wrap.cxx:	easdif-perl5.i $(easdifincfiles) $(incfiles)
			$(swig)

SDIF_wrap.cxx:		sdif.i   $(sdifincfiles)   $(incfiles)
			$(swig)


%_wrap.o: %_wrap.cxx
	g++ -c $(dbflag) -fpic -Dexplicit= -fno-strict-aliasing $(INCL) $<

%.so: 	%_wrap.o
	g++ -shared $(EASDIF_LIB_PATH)/libEasdif.so $< -o $@

test:	eaSDIF.pm eaSDIF.so #SDIF.so
	LD_LIBRARY_PATH=$(EASDIF_LIB_PATH):$(LD_LIBRARY_PATH) \
	perl -I. test.pl

clean:
	-rm -f eaSDIF_wrap.* SDIF_wrap.* eaSDIF.pm SDIF.pm eaSDIF.so SDIF.so

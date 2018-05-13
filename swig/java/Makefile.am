EXTRA_DIST = CMakeLists.txt easdif-java.i main.java sdiftypemap-java.i main.java.in

TOP_SRCDIR=../..
TOP_BUILDDIR=$(TOP_SRCDIR)

JAVA_INCLUDE=#@JAVA_INCLUDE@

INCL		=  $(JAVA_INCLUDE) \
		   -I$(top_builddir)/.. -I$(top_srcdir)/.. -I$(top_builddir)/../SDIF/include -I$(top_srcdir)/../SDIF/include 
EASDIF_LIB_PATH	= $(top_builddir)/../easdif/.libs
# dbflag = -g


modules		= eaSDIF #SDIF
incfiles	= ../easdif-defines.i sdiftypemap-java.i
easdifincbase	= entity _frame _matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=$(top_srcdir)/../easdif/sdif%.hpp)
sdifincfiles	= $(top_srcdir)/../SDIF/include/sdif.h

define swig
	swig -c++ -java $(INCL) -o $@ $<
endef


all:	libeaSDIF.so #libSDIF.so

eaSDIF_wrap.cxx:	easdif-java.i $(easdifincfiles) $(incfiles)
			$(swig)

SDIF_wrap.cxx:		sdif.i   $(sdifincfiles)   $(incfiles)
			$(swig)


libeaSDIF.so: eaSDIF.so
	mv eaSDIF.so libeaSDIF.so

libSDIF.so: SDIF.so
	mv SDIF.so libSDIF.so


%_wrap.o: %_wrap.cxx
	g++ -c $(dbflag) $(INCL) $<

%.so: 	%_wrap.o
	g++ -shared $(EASDIF_LIB_PATH)/libEasdif.so $< -o $@

test:	libeaSDIF.so 
	@JAVAC@ *.java
	LD_LIBRARY_PATH=$(EASDIF_LIB_PATH):$(LD_LIBRARY_PATH):. \
	java main

clean:
	-mv main.java main.java.new
	-rm -f eaSDIF_wrap.* SDIF_wrap.* *.java *.class libeaSDIF.so
	-mv main.java.new main.java

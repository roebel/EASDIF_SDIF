
EXTRA_DIST = CMakeLists.txt test.py test.py.in easdif-python.i sdiftypemap-python.i example.py

INCL		= -I/usr/local/include -I$(PYTHON_INCLUDE) \
		   -I$(top_builddir)/.. -I$(top_srcdir)/.. -I$(top_builddir)/../SDIF/include -I$(top_srcdir)/../SDIF/include 

EASDIF_LIB_PATH	= $(top_builddir)/../easdif/.libs
# dbflag = -g
BUILT_SOURCES = eaSDIF_wrap.cpp # $(srcdir)/SDIF_wrap.cpp
SWIG_SOURCES = ../easdif-common.i ../easdif-defines.i sdiftypemap-python.i

pkgpython_PYTHON = 	eaSDIF.py
pkgpyexec_LTLIBRARIES = _eaSDIF.la
_eaSDIF_la_SOURCES = eaSDIF_wrap.cpp $(SWIG_SOURCES)
_eaSDIF_la_CPPFLAGS = $(SWIG_PYTHON_CPPFLAGS) $(INCL) 
_eaSDIF_la_LDFLAGS = -module
_eaSDIF_la_LIBADD = $(EASDIF_LIB_PATH)/libEasdif.la

incfiles	= ../easdif-common.i ../easdif-defines.i sdiftypemap-python.i


easdifincbase	= entity _frame _matrix namevaluetable
easdifincfiles	= $(easdifincbase:%=$(top_srcdir)/../easdif/sdif%.hpp)
sdifincfiles	= $(top_srcdir)/../SDIF/include/sdif.h

eaSDIF_wrap.cpp : easdif-python.i $(SWIG_SOURCES)  $(easdifincfiles) $(incfiles)
	$(SWIG) $(SWIG_PYTHON_OPT)  -I$(top_srcdir)/swig/python $(INCL) -o $@ $<

$(srcdir)/SDIF_wrap.cpp : sdif.i $(SWIG_SOURCES)  $(sdifincfiles) $(incfiles)
	$(SWIG) $(SWIG_PYTHON_OPT)  -I$(top_srcdir)/swig/python  $(INCL) -o $@ $<


test:	eaSDIF.py _eaSDIF.la # _SDIF.so
	PYTHONPATH=.libs;export PYTHONPATH; LD_LIBRARY_PATH=$(top_builddir)/../easdif/.libs:$(LD_LIBRARY_PATH) \
	$(PYTHON) test.py

CLEANFILES=eaSDIF.py SDIF.py eaSDIF.pyc SDIF.pyc eaSDIF_wrap.cpp

# clean:
# 	-rm -f eaSDIF_wrap.* SDIF_wrap.* eaSDIF.py SDIF.py eaSDIF.pyc SDIF.pyc _eaSDIF.so _SDIF.so

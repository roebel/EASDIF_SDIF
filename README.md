# Disclaimer

This github repos is a manually managed mirror  providing access to the EASDIF_SDIF directory of the SourceForge 
SDIF repos.

   Copyright (C) Ircam 2002-2018

Here below follows the original readme of the Easdif project:

# Introduction

Easdif is a  high level API to the functions of  the SDIF Library.  By
means  of encapsulating  all  low level  functions  into classes  with
intuitive  user interface  the  involved handling  of  the SDIF  files
becomes much  simpler. Nevertheless all the deeper  levels of handling
SDIF files  as for examples Name  Value Tables and  embedded types are
accessible with Easdif.

libEasdif contains all objects from  SDIF compiled with a c++ compiler
to  facilitate error  handling  by  means of  exceptions.  Due to  the
complete availability of  SDIF in Easdif it can be  used as a complete
SDIF replacement library.

Due to time constraints releases  of the Easdif library are rare.  The
sourceforge svn, however, is constantly maintained and should give you
a clean build.  To get the proper source layout from cvs check out the
cvs module EASDIF_SDIF as below :

svn co svn://svn.code.sf.net/p/sdif/code/trunk/Easdif  EASDIF_SDIF

This  creates a directory  EASDIF_SDIF with  a subdirectory  SDIF. The
directory ESDIF_SDIF should  be used as source root  directory for the
cmake configuration (see below) and the build.

# Installation and compilation

Since  version 1.3.0  of Easdif  the main  build  system configuration
system is based on cmake (see: www.cmake.org). cmake is similar to the
automake tools, however, it is much faster and supports a large number
of compilers and  operating systems.  (Currently Makefiles and
 gcc or icc compiler on Linux,  Xcode or Makefiles using gcc on Mac OS
X,  Visual C++  and Borland  C++ on  Windows and  other  build systems
should be directly supported.
 
To configure  and compile Easdif  using the cmake  build configuration
you should use cmake version  2.8.0. Earlier versions may work for you
but,  they are are  not tested.   After installing  cmake you  need to
extract the  Easdif distribution, and  create and cd into  a dedicated
build directory.   To configure and  compile using Unix  makefiles you
may then call

$> cmake path_to_top_level_easdif_dir
$> make 
$> make install

The install target will install  shared versions of the Easdif and the
SDIF  libraries  as well  as  the SDIF  command  line  tools that  are
included in the Easdif  distribution. The default install directory is
/usr/local.  The   install  directory   can  be  modified   using  the
CMAKE_INSTALL_PREFIX variable as follows

$> cmake -DCMAKE_INSTALL_PREFIX:STRING=/my/install/dir  path_to_top_level_easdif_dir 
$> make 
$> make install


## Mac OS X

For installation on  Mac OS X you can use the  same procedure as above
or you  can use Xcode.  To configure your  Xcode project you  create a
build directory BDIR cd into this directory and run

cmake -GXcode path_to_top_level_easdif_dir

which will create the Xcode project in the current directory BDIR. You
can then load  the project and compile with  Xcode.  The cmake project
generator supports Mac OS X with Xcode 1.5 until Xcode 2.4.

To  enable the  creation  of  universal binaries  you  should use  the
configure option DOUNIVERSAL.   Note, however, that universal binaries
are only supported  if the SDK MacOSX10.4u.sdk is  installed.  In this
case  and with DOUNIVERSAL  option given  the universal  binaries that
will be created will run on Mac OS X 10.4 or later.

The command line for configuration with universal binaries is then

cmake -GXcode path_to_top_level_easdif_dir -DDOUNIVERSAL:=1

Warning :  With some versions of  Xcode the build button  in the tools
menu does not  build the selected target. Better  right click on the
target you want to build and  select build from there.  Note, that you
should never  use the ALL  target because not  all of the  targets are
supported in Xcode.

## Windows 

To  configure and  compile  on window  you  can use  all the  compiler
environments supported  by cmake (mingw, cygwin, msvc  and many more).
You can run  cmake either from the  command line or as a  GUI.  In the
gui mode you  select the source directory and  build directory and run
the  cmake  configuration.   Then  you can  interactively  modify  the
compiler  options you  like,  and  run the  configure  pass until  all
settings  stabilize (no  red field  appears  in the  dialog) and  then
create the  selected project.   Note, that for  some IDEs  the default
build style  may be debug  and you may  need to explicitly  select the
release build.  Note,  as well that the ALL target  will fail for most
IDEs  because  not  all  of  the  targets  are  supported  in  an  IDE
environment. Therefore you better work with the individual library and
program targets. 

# Matlab/Octave mex interface

Since  1.4.0  of the  library  a matlab  mex  interface  to easdif  is
available.   Building  mex files  will  work  for  command line  build
generators (Makefile).  Since cmake version 2.8.5 and Easdif 1.4.16 it
should work correctly  as well for GUI based  IDEs (MSVC, Xcode).  You
need to make sure however, that the compiler used by the matlab/octave
mex/mkoctfile  scripts is  the  same  that is  used  to configure  the
library. To  change the compiler used  by cmake you  can either select
the  compiler form  the menu  of the  cmake gui  or  set environnement
variables prior  to the  first run of  the cmake command  line utility
(you have to set CC for the  c compiler and CXX for the c++ compiler).
Please  refer  to cmake  documentation  for  futher  details. In  some
situations you can configure the  compiler used by the mex script from
the  matlab environment  using  mex setup.  Architecture setup  (i386,
x86_64) has to be coherent  as well.  For more details on architecture
related problems see the discussion on mex interface compilation under
Mac OS 10.6 below.

To configure for comilation of the mex interface use

$> cmake -DEASDIF_BUILD_MEX:BOOL=on path_to_top_level_easdif_dir

See below for further info for configuring on Mac OS 10.6.

To build for use in  matlab do

$> make mex
$> make install

To build for use in  octave do

$> make oct
$> make install

This  installs  the mex  and  matlab m-files  as  well  as the  Easdif
library.  If  you want to copy  only the the matlab  support files you
can copy the  content of the m-files in the  matlab source root folder
and  the generated  mex  files in  the  directory bin/  in your  build
folder.
 
If the mex compile script is not found by cmake you can select it  
manually as follows

$> cmake -DMEX:STRING=/path_to_mex_script -DEASDIF_BUILD_MEX:BOOL=on path_to_top_level_easdif_dir 
$> make mex
$> make install

Example for the directory structure

build_EASDIF/
EASDIF_SDIF/

$> cd build_EASDIF
$> cmake -DMEX:STRING=/Applications/MATLAB_R2010a.app/bin/mex -DEASDIF_BUILD_MEX:BOOL=on ../EASDIF_SDIF
$> make mex
$> make install


ATTENTION: the mex script is often masked by the mex binary of the tex 
program suite. If cmake finds the mex of tex first in its path it will
not be able to run correctly. In this case you have to either change
your PATH or specify the mex script on the command line as shown above.

For the octave compile script mkoctfile  there exists a similar syntax

$> cmake -DMKOCT:STRING=/path_to_mkoctfile -DEASDIF_BUILD_MEX:BOOL=on path_to_top_level_easdif_dir 
$> make oct
$> make install

Mex wrappers and m-files  will be installed in 
${CMAKE_INSTALL_PREFIX}/lib/matlab.

## Configuring the Matlab Mex interface for Mac OS 10.6

You need to configure Easdif  according to the matlab version you use.
The choices that  exist are related to the  architecture, the compiler
and the deployment  target your matlab has been  compiled for. In most
cases matlab uses the gcc 4.0 compiler, and deployment target 10.5.

The corrsponding choices can be  made by means of comand line switches
of  the  cmake  command  line  as  follows. You  have  to  adjust  the
architecture according  to the  Matlab you are  using.  You  find your
matlab architecture by  issuing mexext on the matlab  comand line.  If
you get mexmaci64 you should use 
 -DCMAKE_OSX_ARCHITECTURES:STRING=x86_64
and in case you get mexmaci 
 -DCMAKE_OSX_ARCHITECTURES:STRING=i368

Here the the command line for 64 bit Matlab.

CC=gcc-4.0 CXX=g++-4.0 cmake ../EASDIF_SDIF -DCMAKE_OSX_ARCHITECTURES:STRING=x86_64 -DCMAKE_OSX_SYSROOT:STRING=/Developer/SDKs/MacOSX10.5.sdk -DCMAKE_MACOSX_DEPLOYMENT_TARGET=10.5 -DEASDIF_BUILD_MEX:BOOL=ON 

For  csh you have  to set  the environment  variables that  select the
compiler  before running  the cmake  command.  Please  note,  that you
cannot change the compiler for a configured build directory.

# SWIG Wrappers


Easdif provides a number of swig wrapper templates that can be used to
work  with  SDIF  files  from   python,  perl  or  java.  For  further
information look into swig/README.


# Documentation

The  programmer documentation  of  the Easdif  library  can either  be
downloaded from http://sourceforge.net/projects/sdif  or if doxygen is
installed may be generated from the local sources by means of calling

> make doc

in a configured source directory.


# License Agreement

Easdif and  SDIF are free  software, distributed under the  GNU Lesser
General Public License. See the file COPYING for further information.


# Contact

SDIF home page: http://sdif.sourceforge.net/
SDIF wiki http://sdif.wiki.sourceforge.net/
sourceforge project page: http://sourceforge.net/projects/sdif





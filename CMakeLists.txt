#
# cmake configuration file
#
# This file is part of the IRCAM EaSDIF Library (http://www.ircam.fr/sdif)
#
# Copyright (C) 2006 by IRCAM - Centre Pompidou, Paris, France.
#
# created by Axel Roebel                     7.12.2006
#
# This file computes/replaces some default values:
# CMAKE_BUILD_TYPE     : default value is "release" other selectable value is "debug"
# PKGCONFIGDIR         : /u/formes/share/lib/$ENV{SYS}/pkgconfig 
#
# The default values can be disabled by means of setting USECMAKEDEFAULTS:BOOL=ON on the command line
# if you choose this you are expected to provide reasonable values for all of the above
# variables or you are happy with the cmake defaults.
#
# On macosx and if the SDKs are available it replaces gcc/c++ by an script that sets
# an environment variable MACOSX_DEPLOYMENT_TARGET to 10.3 (for ppc architectures)
# and 10.4 (for intel architectures) 
#
# For partly replacing the default values you may run cmake twice, first to create
# a CMakeCache.txt with the above default values and second to override some of the default values
# by means of command line arguments.
#
# default values can be reestablished if the related variable is set to the value "default"
#
#  OPTIONS provided
#  USE_THREADS:BOOL=ON/OFF enable disable use of threads
#  BUILD_MINGW:BOOL=ON/OFF  if cygwin add -mno-cygwin to compiler commands

PROJECT(EaSDIF)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 OLD)
ENDIF(POLICY CMP0042)
IF(POLICY CMP0054)
  CMAKE_POLICY(SET CMP0054 OLD)
ENDIF(POLICY CMP0054)

SET(EASDIF_VERSION_MAJ 1)
SET(EASDIF_VERSION_MIN 4)
SET(EASDIF_VERSION_RELEASE 27)
SET(EASDIF_VERSION  "${EASDIF_VERSION_MAJ}.${EASDIF_VERSION_MIN}.${EASDIF_VERSION_RELEASE}")

MESSAGE(STATUS "===============================================================")
MESSAGE(STATUS "== Configuring Easdif   ${EASDIF_VERSION} ")
MESSAGE(STATUS "===============================================================")

SET(PACKAGE "\"Easdif\"")

IF(NOT EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/SDIF)
  MESSAGE(FATAL_ERROR "You are missing the SDIF source directory!!! Please check the way how you obtained the sources.\nIf you have obtained them via CVS you probably used the wrong module. Try again checking out the EASDIF_SDIF module that contains all the necessary sources for compilation. \n\nuse: cvs -z3 -d:pserver:anonymous@sdif.cvs.sourceforge.net:/cvsroot/sdif co -P EASDIF_SDIF\n")  
ENDIF(NOT EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/SDIF)

SET(CMAKE_VERBOSE_MAKEFILE false)
SET(CMAKE_SKIP_BUILD_RPATH OFF)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/cmModules)


# # prevent the use of the autoconf default values 
# # we impose our own handling
# EXTERNAL_CXXFLAGS=$CXXFLAGS

# AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

# AM_CONFIG_HEADER(easdif/easdif_config.hpp)

INCLUDE(GET_ARCH)

IF(APPLE)
  INCLUDE(REDIRECT_COMPILERS_TO_SYSROOT OPTIONAL)
ENDIF(APPLE)

IF(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  ADD_CUSTOM_TARGET(libs)
  ADD_CUSTOM_TARGET(doc) 
  ADD_CUSTOM_TARGET(install_doc) 
  ADD_CUSTOM_TARGET(check)
ENDIF(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)


IF (WIN32)
  IF(CMAKE_COMPILER_IS_CYGWIN)
    OPTION(BUILD_MINGW "Build MingW Version" ON)
    IF(BUILD_MINGW)
      ADD_DEFINITIONS (-mno-cygwin )
      #SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mno-cygwin")
      #SET(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -mno-cygwin")
      LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -mno-cygwin)
      LIST(APPEND CMAKE_REQUIRED_FLAGS -mno-cygwin)
      LINK_LIBRARIES(-mno-cygwin)
    ENDIF(BUILD_MINGW)
  ENDIF(CMAKE_COMPILER_IS_CYGWIN)
  IF(MSVC)
    # for visual c++ 2005, don't show deprecated function warnings
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE=1 -D_SCL_SECURE_NO_WARNINGS=1 -D_SECURE_SCL=0)
  ENDIF(MSVC)
ENDIF(WIN32)

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(NOT WIN32)
    INCLUDE( CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-fvisibility=hidden" GCC_HAS_VISIBILITY)
    IF(GCC_HAS_VISIBILITY)
      ADD_DEFINITIONS(-DGCC_HAS_VISIBILITY -fvisibility=hidden)
    ENDIF(GCC_HAS_VISIBILITY)
  ENDIF(NOT WIN32)
  # many gcc implementations enable large file support when the 
  # macro _FILE_OFFSET_BITS is compiled. We set it here, it will not harm if gcc does 
  # honor it. In that case we are using the default large file support of the compiler
  ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# set default values for build type and install prefix
INCLUDE(SET_LOCAL_DEFAULTS OPTIONAL)


IF   ( "$ENV{SYS}" STREQUAL "" )
  # if you set archdir then it needs to include a leading directory divider like "/i386"
  # otherwise the install_name tool on mac will not manage to fix the install_name of dynamique libraries
  SET(ARCHDIR)
  SET(INSTALL_LIB_DIR  lib)
  SET(INSTALL_BIN_DIR  bin)
ELSE ( "$ENV{SYS}" STREQUAL "" )
  # if you set archdir then it needs to include a leading directory divider like "/i386"
  # otherwise the install_name tool on mac will not manage to fix the install_name of dynamique libraries
  SET(ARCHDIR)
  SET(INSTALL_LIB_DIR lib/$ENV{SYS})
  SET(INSTALL_BIN_DIR bin/$ENV{SYS})
ENDIF( "$ENV{SYS}" STREQUAL "" )

SET(INSTALL_INC_DIR include)

SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})
SET(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})


IF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin${ARCHDIR})
ENDIF(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)

IF(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib${ARCHDIR})
ENDIF(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)

IF(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  IF(WIN32)
    # dlls are libraries and need to be present in the bin dir
    # so we merge library and bin output     
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  ELSE(WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  ENDIF(WIN32)
ENDIF(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)

############################################################
# Checks for programs.

FIND_PROGRAM(HAVEDOT_EXECUTABLE NAMES dot )
IF(HAVEDOT_EXECUTABLE)
  SET(HAVEDOT YES)
ELSE(HAVEDOT_EXECUTABLE)
  SET(HAVEDOT NO)
ENDIF(HAVEDOT_EXECUTABLE)
## Configure SDIF

INCLUDE(SET_COMPILER_FLAGS)

MESSAGE(STATUS "CHECK FOR HEADERS")
INCLUDE(CheckIncludeFile)

CHECK_INCLUDE_FILE(stdint.h    HAVE_STDINT_H)
CHECK_INCLUDE_FILE(stdlib.h    HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(stdarg.h    HAVE_STDARG_H)
CHECK_INCLUDE_FILE(dlfcn.h     HAVE_DLFCN_H)
CHECK_INCLUDE_FILE(string.h    HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h   HAVE_STRINGS_H)
CHECK_INCLUDE_FILE(inttypes.h  HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(memory.h    HAVE_MEMORY_H)
CHECK_INCLUDE_FILE(float.h     HAVE_FLOAT_H)
CHECK_INCLUDE_FILE(sys/stat.h  HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(errno.h     HAVE_ERRNO_H)
CHECK_INCLUDE_FILE(unistd.h    HAVE_UNISTD_H)

# only very rough test
IF(HAVE_STDLIB_H AND HAVE_STDARG_H AND HAVE_STRING_H AND HAVE_FLOAT_H)
SET(STDC_HEADERS 1)
ENDIF(HAVE_STDLIB_H AND HAVE_STDARG_H AND HAVE_STRING_H AND HAVE_FLOAT_H)

INCLUDE(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(getenv HAVE_GETENV)
CHECK_FUNCTION_EXISTS(ftruncate HAVE_FTRUNCATE)

INCLUDE(FindThreads)
OPTION(USE_THREADS "Enable threads" ON)

IF(USE_THREADS)
  IF(CMAKE_USE_PTHREADS_INIT)
    ADD_DEFINITIONS(-DHAVE_PTHREAD)
    SET(HAVE_PTHREAD 1)
    LINK_LIBRARIES (${CMAKE_THREAD_LIBS_INIT})
  ENDIF(CMAKE_USE_PTHREADS_INIT)
ENDIF(USE_THREADS)


############################################################
# Particular compiler characteristics
MESSAGE(STATUS "PARTICULAR COMPILER CHARACTERISTICS")

#dnl Check for Long 64 bits
INCLUDE(CHECK_SIZEOF)
CHECK_SIZEOF(long 4)
#AC_CHECK_SIZEOF(long)
	

# EaSDIF Configure
IF   (CMAKE_BUILD_TYPE MATCHES debug)
  SET(DEBUGEXT _debug)
ELSE (CMAKE_BUILD_TYPE MATCHES debug)
  SET(DEBUGEXT )
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)

INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(sstream HAVE_SSTREAM)
CHECK_INCLUDE_FILE_CXX(strstream HAVE_STRSTREAM)

SET(top_srcdir ${CMAKE_CURRENT_SOURCE_DIR})
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)


# include special traget to remove cache, system and compiler info
IF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
  INCLUDE(CLEAN_CMAKE)
  INCLUDE(CVS_Targets)
  INCLUDE(BuildTypeTargets)
ENDIF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")

INCLUDE(CREATE_CVSTAGS_MACRO)
# create cvstags for current version
CREATE_CVSTAG_TARGET("libEasdif_${EASDIF_VERSION_MAJ}_${EASDIF_VERSION_MIN}_${EASDIF_VERSION_RELEASE}")


ADD_SUBDIRECTORY(SDIF)


ADD_DEFINITIONS("-DHAVE_CONFIG_H")

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/SDIF/sdifconfig ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SDIF/include ${CMAKE_CURRENT_SOURCE_DIR}  )

OPTION(EASDIF_BUILD_MEX "build matlab mex bindings" off)
OPTION(EASDIF_DO_PYTHON OFF)
OPTION(EASDIF_DO_PERL OFF)
OPTION(EASDIF_DO_JAVA OFF)

IF(EASDIF_BUILD_MEX OR EASDIF_DO_PYTHON OR EASDIF_DO_PERL OR EASDIF_DO_JAVA)
  SET(EASDIF_BUILD_STATIC 1)
ELSE(EASDIF_BUILD_MEX OR EASDIF_DO_PYTHON OR EASDIF_DO_PERL OR EASDIF_DO_JAVA)
  IF(NOT EASDIF_BUILD_STATIC)
    OPTION(EASDIF_BUILD_STATIC "build static versions of the library" off)
  ELSE(NOT EASDIF_BUILD_STATIC)
    OPTION(EASDIF_BUILD_STATIC "build static versions of the library" ${EASDIF_BUILD_STATIC})
  ENDIF(NOT EASDIF_BUILD_STATIC)
ENDIF(EASDIF_BUILD_MEX OR EASDIF_DO_PYTHON OR EASDIF_DO_PERL OR EASDIF_DO_JAVA)

ADD_SUBDIRECTORY( easdif ) 
ADD_SUBDIRECTORY( test ) 
IF(EASDIF_BUILD_MEX)
  ADD_SUBDIRECTORY( matlab ) 
ENDIF(EASDIF_BUILD_MEX)

ADD_CUSTOM_TARGET(libeasdif_lib)
ADD_DEPENDENCIES(libeasdif_lib Easdif${DEBUGEXT})


########### install files ###############
FILE(TO_CMAKE_PATH "${PKGCONFIGDIR}" tmp)

IF(NOT "${tmp}" STREQUAL  "")
  IF("${tmp}" MATCHES "/.*")
    SET(PKGCONFIGPATH "${tmp}" CACHE PATH "install path for pkg config files" FORCE)
    MESSAGE(STATUS "pkgconfig  abs    : ${PKGCONFIGPATH}")
  ELSE("${tmp}" MATCHES "/.*")
    IF("${tmp}" STREQUAL "default" )
      SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig" CACHE PATH "install path for pkg config files" FORCE)
      MESSAGE(STATUS "pkgconfig  rel   : ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig")
    ELSE("${tmp}" STREQUAL "default" )
      SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/${tmp}" CACHE PATH "install path for pkg config files" FORCE)
    ENDIF("${tmp}" STREQUAL "default" )
    MESSAGE(STATUS "pkgconfig  rel   : ${PKGCONFIGPATH}")  
  ENDIF("${tmp}" MATCHES "/.*")
ENDIF(NOT "${tmp}" STREQUAL  "")

# if install path is empty set to default
IF("${PKGCONFIGPATH}" STREQUAL  "")
  SET(PKGCONFIGPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}/pkgconfig" CACHE PATH "install path for pkg config files" FORCE)
ENDIF("${PKGCONFIGPATH}" STREQUAL  "")



SET(prefix ${CMAKE_INSTALL_PREFIX})
SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
SET(libdir ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR})
SET(includedir ${CMAKE_INSTALL_PREFIX}/include)
SET(EASDIFBASENAME Easdif${DEBUGEXT})
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/Easdif.pc.in   ${PROJECT_BINARY_DIR}/Easdif${DEBUGEXT}.pc )

IF(EASDIF_DO_PYTHON OR EASDIF_DO_PERL OR EASDIF_DO_JAVA)
  IF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
    ADD_SUBDIRECTORY( swig EXCLUDE_FROM_ALL )
  ELSE(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
    ADD_SUBDIRECTORY( swig )
  ENDIF(CMAKE_GENERATOR MATCHES "^Unix Makefiles")
ENDIF(EASDIF_DO_PYTHON OR EASDIF_DO_PERL OR EASDIF_DO_JAVA)


INSTALL(FILES  ${PROJECT_BINARY_DIR}/Easdif${DEBUGEXT}.pc DESTINATION ${PKGCONFIGPATH} )


GET_DIRECTORY_PROPERTY(easdifDOCFILES DIRECTORY easdif DEFINITION srcDOCFILES)
SET(DOCFILES ${easdifDOCFILES} CACHE INTERNAL "files containing doc sources" FORCE)
#MESSAGE(STATUS "doc files : ${DOCFILES}")

# Documentation generation with Doxygen
ADD_CUSTOM_TARGET(libeasdif_doc COMMAND  doxygen  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile DEPENDS CMakeLists.txt;${DOCFILES})
IF(DEFINED DOCSERVER)
  FIND_PROGRAM(NCFTPPUT_EXECUTABLE ncftpput)
  IF(NCFTPPUT_EXECUTABLE)
    ADD_CUSTOM_TARGET(libeasdif_install_doc  
      COMMAND chmod -R  g+w doc/html 
      COMMAND ${NCFTPPUT_EXECUTABLE}  ${DOCSERVER} ${DOCDIR} doc/html/* 
      COMMAND chmod -R  g-w doc/html)
    ADD_DEPENDENCIES(libeasdif_install_doc libeasdif_doc)
  ELSE(NCFTPPUT_EXECUTABLE)
    MESSAGE(STATUS "++++++++++++++++++++++++")
    MESSAGE(STATUS "ncftpput not found! no install_doc target generated!")
    MESSAGE(STATUS "++++++++++++++++++++++++")
  ENDIF(NCFTPPUT_EXECUTABLE)
ENDIF(DEFINED DOCSERVER)

SET(DOCTAR Easdif-${EASDIF_VERSION}.doc.tgz)

ADD_CUSTOM_TARGET(pubdoc
COMMAND	cp -r doc   Easdif-${EASDIF_VERSION}.doc
COMMAND	tar -cvzf ${DOCTAR} Easdif-${EASDIF_VERSION}.doc
COMMAND	rm -rf Easdif-${EASDIF_VERSION}.doc)
ADD_DEPENDENCIES(pubdoc doc)


ADD_DEPENDENCIES(libs libeasdif_lib)
ADD_DEPENDENCIES(doc libeasdif_doc)
IF(NCFTPPUT_EXECUTABLE)
  ADD_DEPENDENCIES(install_doc libeasdif_install_doc)
ENDIF(NCFTPPUT_EXECUTABLE)



GET_FILENAME_COMPONENT(normalize_path ${CMAKE_CURRENT_SOURCE_DIR}/SDIF ABSOLUTE)
GET_DIRECTORY_PROPERTY(sdif_version DIRECTORY ${normalize_path} DEFINITION  SDIF_VERSION)

FILE(GLOB_RECURSE configure_dependencies_am ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.am) 
FILE(GLOB_RECURSE configure_dependencies_sh ${CMAKE_CURRENT_SOURCE_DIR}/autogen.sh) 
FILE(GLOB_RECURSE configure_dependencies_in ${CMAKE_CURRENT_SOURCE_DIR}/configure.in) 
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/configure COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/autogen.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${configure_dependencies_in} ${configure_dependencies_am} ${configure_dependencies_sh} )

IF(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  ADD_CUSTOM_TARGET(dist cpack --config ${CMAKE_CURRENT_BINARY_DIR}/CPackSourceConfig.cmake DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/configure )
ENDIF(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EaSDIF/SDIF libraries and tools for handling SDIF files")
SET(CPACK_PACKAGE_VENDOR "IRCAM")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_PACKAGE_VERSION_MAJOR "${EASDIF_VERSION_MAJ}")
SET(CPACK_PACKAGE_VERSION_MINOR "${EASDIF_VERSION_MIN}")
SET(CPACK_PACKAGE_VERSION_PATCH "${EASDIF_VERSION_RELEASE}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
    "EaSDIF-${EASDIF_VERSION}-SDIF-${sdif_version}-src")
SET(CPACK_PACKAGE_FILE_NAME)
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_SOURCE_GENERATOR "TGZ")

SET(CPACK_SOURCE_IGNORE_FILES "/CVS/" "/\\\\.svn/" "\\\\.swp$" "\\\\.#" "/#" "~$"  "/build.*/" "/autom4te.cache/"   "/SDIF/others/" "/all.ircam.sh$" "/all.install$" "/SDIF/sdif/ReadFromMemory.txt$"  "/test/fall.sdif"   "/Makefile$" "sdif.sln$" "/SDIF/test/prof/" "/SDIF/test/.*src$"   "/SDIF/test/.*dsp$" "/SDIF/test/SdifTypes.STYP" "/macosxbuild/[^MS]$"  "/macosxbuild/.*plist$" "/macosxbuild/.*MacOSX.binary-distrib$" "/macosxbuild/S.*/[^Mp]" "/macosxbuild/SDIF.xcodeproj/" ".cvsignore$" "/test/ent.*cpp$" "/test/t[tryo].*cpp$" "/SDIF/doc/pics/.*fig$" "/SDIF/doc/intern/"  "/SDIF/doc/standard/" "/SDIF/doc/index-sf.html" "/SDIF/doc/Makefile.doc"  "/SDIF/doc/sdif.src" "/SDIF/doc/.*txt" "/SDIF/types/" "/SDIF/tools/main.nib/" "/SDIF/tools/.*sdef" "/SDIF/tools/.*cp$" "/SDIF/tools/.*h$" "/SDIF/tools/sdifusage.*$" "/SDIF/tools/.*vcproj$" "/SDIF/doc/download/" "compileandinstall$" "/sdifcpp/.*cpp" "/sdifcpp/.*h$" "/test/Ne*\\\\.sdif$"   "/test/tut[mn].*\\\\.cpp$"   "/test/es.*cpp$" "/test/[cmvnrs].*cpp$" )
#MESSAGE("CPACK_SOURCE_IGNORE_FILES  ${CPACK_SOURCE_IGNORE_FILES}")
INCLUDE(CPack)


MESSAGE(STATUS "===================================================================")
MESSAGE(STATUS "Option summary:")
MESSAGE(STATUS "install prefix (CMAKE_INSTALL_PREFIX)         : ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "enable swig python interf. (EASDIF_DO_PYTHON) : ${EASDIF_DO_PYTHON}")
MESSAGE(STATUS "enable swig perl interface (EASDIF_DO_PERL)   : ${EASDIF_DO_PERL}")
MESSAGE(STATUS "enable swig java interface (EASDIF_DO_JAVA)   : ${EASDIF_DO_JAVA}")
MESSAGE(STATUS "build static Easdif lib (EASDIF_BUILD_STATIC) : ${EASDIF_BUILD_STATIC}")
MESSAGE(STATUS "build static sdif lib (SDIF_BUILD_STATIC)     : ${SDIF_BUILD_STATIC}")
MESSAGE(STATUS "enable thread support (USE_THREADS)           : ${USE_THREADS}")
MESSAGE(STATUS "build mex files (EASDIF_BUILD_MEX)            : ${EASDIF_BUILD_MEX}")
IF(APPLE)
  MESSAGE(STATUS "enable universal (DOUNIVERSAL)                : ${DOUNIVERSAL}" )
  IF(CMAKE_OSX_SYSROOT)
    IF(DOUNIVERSAL)
      MESSAGE(STATUS "Architectures                                 : ${CMAKE_OSX_ARCHITECTURES} ")
    ENDIF(DOUNIVERSAL)
  ENDIF(CMAKE_OSX_SYSROOT)
ENDIF(APPLE)

IF(CMAKE_COMPILER_IS_CYGWIN)
  MESSAGE(STATUS "Use mingw (BUILD_MINGW)                       : ${BUILD_MINGW}")
ENDIF(CMAKE_COMPILER_IS_CYGWIN)

MESSAGE(STATUS "Easdif installation summary:")
MESSAGE(STATUS "bin            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_BIN_DIR}")
MESSAGE(STATUS "lib            : ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")
MESSAGE(STATUS "includes       : ${CMAKE_INSTALL_PREFIX}/${INSTALL_INC_DIR}")
MESSAGE(STATUS "pkgconfig      : ${PKGCONFIGPATH}")
MESSAGE(STATUS "===================================================================")

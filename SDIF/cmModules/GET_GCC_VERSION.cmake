# Determine gcc version
IF(CMAKE_COMPILER_IS_GNUCC)
  IF("${GCCVERSIONMAJ}" STREQUAL "" OR "${GCCVERSIONMIN}" STREQUAL "")
    
    EXEC_PROGRAM(${CMAKE_C_COMPILER} ARGS -dumpversion OUTPUT_VARIABLE TMP)
    MESSAGE("Compiler version: ${TMP}")
    STRING(REGEX REPLACE "^\([0-9]*\)[.][0-9]*[.][0-9]*" "\\1" TMP1 ${TMP} )
    STRING(REGEX REPLACE "^[0-9]*[.]\([0-9]*\)[.][0-9]*" "\\1" TMP2 ${TMP} )
    SET(GCCVERSIONMAJ ${TMP1} CACHE INTERNAL "gcc major version")
    SET(GCCVERSIONMIN ${TMP2} CACHE INTERNAL "gcc minor version")

  ENDIF("${GCCVERSIONMAJ}" STREQUAL "" OR  "${GCCVERSIONMIN}" STREQUAL "")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
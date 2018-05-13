SET(EASDIF_MATLAB_VERSION_MAJ 1)
SET(EASDIF_MATLAB_VERSION_MIN 2)
SET(EASDIF_MATLAB_VERSION_REL 1)

MESSAGE(STATUS "-------------------------------------------------")
MESSAGE(STATUS "Configuring mex wrappers: version ${EASDIF_MATLAB_VERSION_MAJ}.${EASDIF_MATLAB_VERSION_MIN}.${EASDIF_MATLAB_VERSION_REL}" )

FIND_PROGRAM(MEX mex)
FIND_PROGRAM(MKOCT mkoctfile)

IF(NOT MEX AND APPLE)
  FILE(GLOB MATLAB_PATH /Applications/M*)
  IF(MATLAB_PATH)
    FOREACH(DIR ${MATLAB_PATH})
      IF(NOT MEX)
        IF(EXISTS  ${DIR}/bin/mex)
          SET(MEX ${DIR}/bin/mex)
        ENDIF(EXISTS  ${DIR}/bin/mex )
      ENDIF(NOT MEX)
    ENDFOREACH(DIR ${MATLAB_PATH})
  ENDIF(MATLAB_PATH)
ENDIF(NOT MEX AND APPLE)


IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER "2.8.4")
  SET(USE_GEN_EXPRESSIONS ON)
ELSE(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER "2.8.4")
  SET(USE_GEN_EXPRESSIONS ON)
ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER "2.8.4")


IF(MEX) 
  MESSAGE(STATUS "matlab mex wrapper :: using mex -> ${MEX}")
  MESSAGE(STATUS "generator expression supported -> ${USE_GEN_EXPRESSIONS}")
  # Attention the line breaks are required for mac ppc
  SET(SOURCE "#include<mex.h> 
void mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs []){
}")
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/SDIF/cmModules/trycompile.c.in  ${CMAKE_CURRENT_BINARY_DIR}/cmake_tmp.cxx)
  EXECUTE_PROCESS(COMMAND ${MEX}  ${CMAKE_CURRENT_BINARY_DIR}/cmake_tmp.cxx -v RESULT_VARIABLE res OUTPUT_VARIABLE out ERROR_VARIABLE err)
  
  IF(res)
    MESSAGE(STATUS "mex command ${MEX} cannot be run, error ${res} '${err}' skipping mex\n    make sure this is not the mex program from the tex distribution.")
  ELSE (res)
    IF(NOT MEXEXT) 
      STRING(REGEX REPLACE ".*cmake_tmp.mex\([a-z0-9]*).*" "\\1" MEXEXTSUF ${out})
      SET(MEXEXT ".mex${MEXEXTSUF}")
      IF(DEBUG)
        MESSAGE("MEXEXT SUF<${MEXEXTSUF}>")
      ENDIF(DEBUG)
      #      STRING(REGEX REPLACE ".*LDEXTENSION[ \t]*=[ \t]*\([^ \t\n\r]*).*" "\\1" MEXEXT ${out})
    ENDIF(NOT MEXEXT)

    MESSAGE(STATUS "Use MEXEXT: <${MEXEXT}>")

    IF( "${out}" MATCHES "-fexception" )
      SET(EXCEPTIONFLAG)
    ELSE( "${out}" MATCHES "-fexception" )
      SET(EXCEPTIONFLAG "-fno-exceptions" )
    ENDIF( "${out}" MATCHES "-fexception" )
    
    
    # GNU C++ Compiler
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET(SEL_CXXABI_FLAG -fabi-version=1 CACHE STRING "select C++ ABI")
      IF (GCCVERSIONMAJ EQUAL 2)
        ADD_DEFINITIONS( -D_ISOC99_SOURCE)
      ENDIF (GCCVERSIONMAJ EQUAL 2)        
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    
    SET(CMAKE_SHARED_LIBRARY_PREFIX)
    IF("${ARCH}" STREQUAL "x86_64" )
      SET(CMAKE_SHARED_LIBRARY_SUFFIX mexa64)
    ELSE ("${ARCH}" STREQUAL "x86_64" )
      SET(CMAKE_SHARED_LIBRARY_SUFFIX mexglx)
    ENDIF("${ARCH}" STREQUAL "x86_64" )
    
    IF(${CMAKE_BUILD_TYPE} MATCHES "debug")
      SET(MEXFLAGS -g)
      SET(COMPFLAGS CMAKE_CXX_FLAGS_DEBUG)
      IF(WIN32)
        SET(MEXFLAGBUILDTYPE DEBUGFLAGS)
      ELSE(WIN32)
        SET(MEXFLAGBUILDTYPE CXXDEBUGFLAGS)
      ENDIF(WIN32)
    ELSE(${CMAKE_BUILD_TYPE} MATCHES "debug")
      SET(MEXFLAGS -O)
      SET(COMPFLAGS CMAKE_CXX_FLAGS_RELEASE)
      IF(WIN32)
        SET(MEXFLAGBUILDTYPE OPTIMFLAGS)
      ELSE(WIN32)
        SET(MEXFLAGBUILDTYPE CXXOPTIMFLAGS)
      ENDIF(WIN32)
    ENDIF(${CMAKE_BUILD_TYPE} MATCHES "debug")

   IF(CMAKE_MEXOPTS_FILE)
     SET(MEXFLAGS ${MEXFLAGS} -f ${CMAKE_MEXOPTS_FILE})
   ENDIF(CMAKE_MEXOPTS_FILE)
   MESSAGE("MEXFLAGS ${MEXFLAGS}")
    IF(WIN32)
      SET(COMPILER_SELECTOR_VAR "COMPILER")
    ELSE(WIN32)
      SET(COMPILER_SELECTOR_VAR "CXX")
    ENDIF(WIN32)

    IF("${out}" MATCHES ".*->[ \t]*MATLAB[ \t]*=[ \t]*\([^\n\r]*\).*")
      STRING(REGEX REPLACE ".*->[ \t]*MATLAB[ \t]*=[ \t]*\([^\n\r]*\).*" "\\1" MATLAB_COMMAND ${out})
      MESSAGE(STATUS "Configuring for ${MATLAB_COMMAND}")
    ENDIF("${out}" MATCHES ".*->[ \t]*MATLAB[ \t]*=[ \t]*\([^\n\r]*\).*")
    IF("${out}" MATCHES ".*[^L][^D]${MEXFLAGBUILDTYPE}[ \t]*=[ \t]*\([^\n\r]*\).*")
      STRING(REGEX REPLACE ".*[^L][^D]${MEXFLAGBUILDTYPE}[ \t]*=[ \t]*\([^\n\r]*\).*" "\\1" MEXORIFLAGS ${out})
    ELSE("${out}" MATCHES ".*[^L][^D]${MEXFLAGBUILDTYPE}[ \t]*=[ \t]*\([^\n\r]*\).*")
      MESSAGE(warning "cannot determine original mex compiler flags for ${MEXFLAGBUILDTYPE}, will try none")
      SET(MEXORIFLAGS)
    ENDIF("${out}" MATCHES ".*[^L][^D]${MEXFLAGBUILDTYPE}[ \t]*=[ \t]*\([^\n\r]*\).*")

    IF(${CMAKE_BUILD_TYPE} MATCHES "debug")

      SET(MEX_CXX_FLAGS "${MEXFLAGBUILDTYPE}=${MEXORIFLAGS} ${CMAKE_CXX_ADD_FLAGS_RELEASE} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
    ELSE(${CMAKE_BUILD_TYPE} MATCHES "debug")
      IF(CMAKE_COMPILER_IS_GNUCXX)
        SET(MEX_CXX_FLAGS   "${MEXFLAGBUILDTYPE}=${MEXORIFLAGS} -O0 -fno-inline -fno-default-inline  ${CMAKE_CXX_ADD_FLAGS_DEBUG} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
      ELSE(CMAKE_COMPILER_IS_GNUCXX)
        SET(MEX_CXX_FLAGS   "${MEXFLAGBUILDTYPE}=${MEXORIFLAGS}  ${CMAKE_CXX_ADD_FLAGS_DEBUG} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
      ENDIF(CMAKE_COMPILER_IS_GNUCXX)
    ENDIF(${CMAKE_BUILD_TYPE} MATCHES "debug")

    IF(DEBUG)
      MESSAGE(STATUS "CMAKE_SHARED_LIBRARY_C_FLAGS ${CMAKE_SHARED_LIBRARY_C_FLAGS}")
      MESSAGE(STATUS "CMAKE_SHARED_LIBRARY_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
      MESSAGE(STATUS "ORI: ${MEXORIFLAGS}")
    ENDIF(DEBUG)
    SET(CMAKE_SHARED_LIBRARY_C_FLAGS)
    SET(CMAKE_SHARED_LIBRARY_CXX_FLAGS)


    SET(ADDCFLAGS "-DEASDIF_MATLAB_VERSION=${EASDIF_MATLAB_VERSION_MAJ}.${EASDIF_MATLAB_VERSION_MIN}.${EASDIF_MATLAB_VERSION_REL} -DEASDIF_IS_STATIC")

    IF(CMAKE_COMPILER_IS_GNUCXX AND GCC_HAS_VISIBILITY)
      SET(ADDCFLAGS "${ADDCFLAGS} -DGCC_HAS_VISIBILITY -fvisibility=hidden")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX AND GCC_HAS_VISIBILITY)



    ADD_CUSTOM_TARGET(mex)      
    SET(INSTALL_MEX_FILES )
    MACRO(MEXCOMPILE_FILES )
      FOREACH(_current_FILE ${ARGN})
        GET_FILENAME_COMPONENT(_tmp_FILE ${_current_FILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(_abs_PATH ${_tmp_FILE} PATH)
        GET_FILENAME_COMPONENT(_basename ${_tmp_FILE} NAME_WE)
      
        SET(_o_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_basename}${DEBUGEXT}${MEXEXT})
        # MESSAGE(STATUS "_current_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} _o_FILE ${_o_FILE}")
        SET(INSTALL_MEX_FILES ${INSTALL_MEX_FILES} ${_o_FILE})
        IF(DEBUG)
          MESSAGE("ADD CUSTOM COMMAND ::${MEX} ${MEXFLAGS} -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} \"${COMPILER_SELECTOR_VAR}=${CMAKE_CXX_COMPILER}\" \"${MEX_CXX_FLAGS} ${ADDCFLAGS}\"  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. -output ${_o_FILE} -L${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} -lEasdif_static_pic${DEBUGEXT}::")
        ENDIF(DEBUG)
        IF(USE_GEN_EXPRESSIONS)
          ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
            COMMENT  "Processing: ${_current_FILE}"
            COMMAND
            ${MEX} ${MEXFLAGS} -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} "${COMPILER_SELECTOR_VAR}=${CMAKE_CXX_COMPILER}" "${MEX_CXX_FLAGS} ${ADDCFLAGS}"  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. -output ${_o_FILE} -L$<TARGET_FILE_DIR:Easdif_static_pic${DEBUGEXT}> -lEasdif_static_pic${DEBUGEXT}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT})
        ELSE(USE_GEN_EXPRESSIONS)
          ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
            COMMENT "Processing: ${_current_FILE}"
            COMMAND
            ${MEX} ${MEXFLAGS} -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} "${COMPILER_SELECTOR_VAR}=${CMAKE_CXX_COMPILER}" "${MEX_CXX_FLAGS} ${ADDCFLAGS}"  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. -output ${_o_FILE} -L${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} -lEasdif_static_pic${DEBUGEXT} 
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT})         
        ENDIF(USE_GEN_EXPRESSIONS)
        ADD_CUSTOM_TARGET(${_basename}${DEBUGEXT}  DEPENDS  ${_o_FILE})      
        #ADD_DEPENDENCIES(${_basename}  ${_o_FILE} )
        ADD_DEPENDENCIES(mex  ${_basename}${DEBUGEXT} )
      ENDFOREACH(_current_FILE)
    ENDMACRO(MEXCOMPILE_FILES )
    
    SET(SOURCES Fsdif_read_handler.cpp Fsdif_write_handler.cpp)
    
    
    MEXCOMPILE_FILES(${SOURCES})
    
    SET(MFILES ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclose.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifopen.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifwrite.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifread.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifexist.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclearall.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifrewind.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifloadfile.m)
    
    INSTALL(FILES ${INSTALL_MEX_FILES} ${MFILES} DESTINATION ${INSTALL_LIB_DIR}/matlab/ )
  ENDIF(res)

ENDIF(MEX)


IF(MKOCT) 

  MESSAGE(STATUS "octave mex wrapper :: using mkoctfile -> ${MKOCT}")
  MESSAGE(STATUS "generator expression supported -> ${USE_GEN_EXPRESSIONS}")
  EXECUTE_PROCESS(COMMAND ${MKOCT} -v RESULT_VARIABLE res OUTPUT_VARIABLE out ERROR_VARIABLE err)

  IF(res)
    MESSAGE(STATUS "mkoctfile command ${MKOCT} cannot be run, error '${err}' skipping oct")
  ELSE (res)

    SET(OCTEXT .mex)
    
    ADD_CUSTOM_TARGET(oct)      
    SET(INSTALL_OCT_FILES )
    MACRO(OCTCOMPILE_FILES )
      FOREACH(_current_FILE ${ARGN})
        GET_FILENAME_COMPONENT(_tmp_FILE ${_current_FILE} ABSOLUTE)
        GET_FILENAME_COMPONENT(_abs_PATH ${_tmp_FILE} PATH)
        GET_FILENAME_COMPONENT(_basename ${_tmp_FILE} NAME_WE)
        
        SET(_o_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_basename}${DEBUGEXT}${OCTEXT})
        #          MESSAGE(STATUS "_current_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE} _o_FILE ${_o_FILE}")
        SET(INSTALL_OCT_FILES ${INSTALL_OCT_FILES} ${_o_FILE})
        IF(USE_GEN_EXPRESSIONS)
          ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
            COMMENT  "Processing: ${_current_FILE}"
            COMMAND
            ${MKOCT} --mex -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. --output ${_o_FILE} -L$<TARGET_FILE_DIR:Easdif_static_pic${DEBUGEXT}> -lEasdif_static_pic${DEBUGEXT}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT})
        ELSE(USE_GEN_EXPRESSIONS)
          ADD_CUSTOM_COMMAND(OUTPUT ${_o_FILE} 
            COMMENT "Processing: ${_current_FILE}"
            COMMAND
            ${MKOCT} --mex -v ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  -I${CMAKE_CURRENT_BINARY_DIR}/../SDIF/sdifconfig -I${CMAKE_CURRENT_BINARY_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR}/../SDIF/include -I${CMAKE_CURRENT_SOURCE_DIR}/.. --output ${_o_FILE} -L${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} -lEasdif_static_pic${DEBUGEXT}            
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_current_FILE}  ${DEP_INCLUDES} Easdif_static_pic${DEBUGEXT}) 
        ENDIF(USE_GEN_EXPRESSIONS)
        
        ADD_CUSTOM_TARGET(${_basename}_oct${DEBUGEXT}  DEPENDS  ${_o_FILE})      
        #ADD_DEPENDENCIES(${_basename}  ${_o_FILE} )
        ADD_DEPENDENCIES(oct  ${_basename}_oct${DEBUGEXT} )
      ENDFOREACH(_current_FILE)
    ENDMACRO(OCTCOMPILE_FILES)
    
    SET(SOURCES Fsdif_read_handler.cpp Fsdif_write_handler.cpp)
    
    OCTCOMPILE_FILES(${SOURCES})
    
    SET(MFILES_OCT ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclose.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifopen.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifwrite.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifread.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifexist.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifclearall.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifrewind.m 
      ${CMAKE_CURRENT_SOURCE_DIR}/Fsdifloadfile.m)
    
    INSTALL(FILES ${INSTALL_OCT_FILES} ${MFILES_OCT} DESTINATION ${INSTALL_LIB_DIR}/octave/ )
  ENDIF(res)  
ENDIF(MKOCT)

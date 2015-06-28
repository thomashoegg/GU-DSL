# - this module looks for Matlab
# Defines:
#  MATLAB_INCLUDE_DIR: include path for mex.h, engine.h
#  MATLAB_LIBRARIES:   required libraries: libmex, etc
#  MATLAB_MEX_LIBRARY: path to libmex.lib
#  MATLAB_MX_LIBRARY:  path to libmx.lib
#  MATLAB_ENG_LIBRARY: path to libeng.lib

#=============================================================================
# Copyright 2015 Thomas Hoegg
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

SET(MATLAB_FOUND 0)
IF(WIN32)
  FIND_PATH(MATLAB_INCLUDE_DIR "mex.h"
    HINTS ENV MATLAB_DIR
    PATH_SUFFIXES extern/include
    PATHS ENV MATLAB_DIR
  )

  FIND_LIBRARY(MATLAB_MEX_LIBRARY libmex
    HINTS ENV MATLAB_DIR
    PATH_SUFFIXES extern/lib/win64/microsoft
    PATHS ENV MATLAB_DIR
  )
  
  FIND_LIBRARY(MATLAB_MX_LIBRARY libmx
    HINTS ENV MATLAB_DIR
    PATH_SUFFIXES extern/lib/win64/microsoft
    PATHS ENV MATLAB_DIR
  )
  
  FIND_LIBRARY(MATLAB_ENG_LIBRARY libeng
    HINTS ENV MATLAB_DIR
    PATH_SUFFIXES extern/lib/win64/microsoft
    PATHS ENV MATLAB_DIR
  )
  
  FIND_LIBRARY(MATLAB_MAT_LIBRARY libmat
    HINTS ENV MATLAB_DIR
    PATH_SUFFIXES extern/lib/win64/microsoft
    PATHS ENV MATLAB_DIR
  )

ENDIF(WIN32)

# This is common to UNIX and Win32:
SET(MATLAB_LIBRARIES
  ${MATLAB_MEX_LIBRARY}
  ${MATLAB_MX_LIBRARY}
  ${MATLAB_ENG_LIBRARY}
  ${MATLAB_MAT_LIBRARY}
)

SET(MATLAB_FOUND "NO")

IF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
  SET(MATLAB_FOUND "YES")
ENDIF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

MARK_AS_ADVANCED(
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
  MATLAB_MAT_LIBRARY
)


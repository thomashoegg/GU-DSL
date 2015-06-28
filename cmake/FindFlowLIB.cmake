# - Try to find Flow SDK
# Once done this will define
#  
#  FLOWSDK_FOUND        -  system has or not FLOW
#  FLOWLIB_INCLUDE_DIR  - the FLOW include directory
#  FLOW_LIBRARY         - path to flow.lib
#  IUCORE_LIBRARY       - path to iucore.lib
#  IUIO_LIBRARY         - path to iuio.lib
#  FLOWLIB_LIBRARIES    - link these to use the FLOW  
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
SET(FLOWSDK_FOUND 0)

IF (WIN32)
  FIND_PATH(FLOWLIB_INCLUDE_DIR1 flowlib.h
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES include/fl
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_PATH(FLOWLIB_INCLUDE_DIR2 fl
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES include
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_PATH(FLOWLIB_INCLUDE_DIR2 fl
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES include
    PATHS ENV FLOWLIB_DIR
  )

  FIND_PATH(FLOWLIB_INCLUDE_DIR3 iucore.h
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES include/iu
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_PATH(FLOWLIB_INCLUDE_DIR4 rof.h
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES include/rof
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_LIBRARY(FLOW_LIBRARY flow.lib
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES lib
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_LIBRARY(IUCORE_LIBRARY iucore.lib
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES lib
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_LIBRARY(IUIO_LIBRARY iuio.lib
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES lib
    PATHS ENV FLOWLIB_DIR
  )
  
  FIND_LIBRARY(ROF_LIBRARY rof.lib
    HINTS ENV FLOWLIB_DIR
    PATH_SUFFIXES lib
    PATHS ENV FLOWLIB_DIR
  )
  
ELSE (WIN32)
  MESSAGE(FATAL_ERROR "Non-Windows Platforms currently not supported!")  
ENDIF (WIN32)

SET(FLOWLIB_LIBRARIES
  ${FLOW_LIBRARY}
  ${IUCORE_LIBRARY}
  ${IUIO_LIBRARY}
  ${ROF_LIBRARY}
)

IF(FLOWLIB_INCLUDE_DIR1 AND FLOWLIB_LIBRARIES)

	SET(FLOWLIB_INCLUDE_DIR ${FLOWLIB_INCLUDE_DIR2} ${FLOWLIB_INCLUDE_DIR2} ${FLOWLIB_INCLUDE_DIR3} ${FLOWLIB_INCLUDE_DIR4})
    SET(FLOWSDK_FOUND 1)

ELSE(FLOWLIB_INCLUDE_DIR1 AND FLOWLIB_LIBRARIES)

  MESSAGE(WARNING "Flow Library not found")  

ENDIF(FLOWLIB_INCLUDE_DIR1 AND FLOWLIB_LIBRARIES)

MARK_AS_ADVANCED(
  FLOWSDK_FOUND
  FLOWLIB_INCLUDE_DIR
  FLOW_LIBRARY
  IUCORE_LIBRARY
  IUIO_LIBRARY
)

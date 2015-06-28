# - Try to find PVI SDK
# Once done this will define
#  
#  PVI_FOUND        - system has PVI
#  PVI_INCLUDE_DIR  - the PVI include directory
#  PVI_LIBRARIES    - Link these to use PVI
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

IF (WIN32)
  FIND_PATH(PVI_INCLUDE_DIR BrPviDriver.h
    HINTS ENV PVI_DIR
    PATH_SUFFIXES include
    PATHS ENV PVI_DIR
  )

  FIND_LIBRARY(PVI_LIBRARY_DEBUG PVIDriver_Win32_Qt5d
    HINTS ENV PVI_DIR
    PATH_SUFFIXES bin/QT5
    PATHS ENV PVI_DIR
  )
  
  FIND_LIBRARY(PVI_LIBRARY_RELEASE PVIDriver_Win32_Qt5
    HINTS ENV PVI_DIR
    PATH_SUFFIXES bin/QT5
    PATHS ENV PVI_DIR
  )
  
  SET(PVI_LIBRARY optimized ${PVI_LIBRARY_RELEASE} debug ${PVI_LIBRARY_DEBUG})
  
ELSE (WIN32)
  MESSAGE(FATAL_ERROR "Non-Windows Platforms currently not supported!")  
ENDIF (WIN32)

SET(PVI_FOUND "NO")

IF(PVI_LIBRARY)

    SET(PVI_LIBRARIES ${PVI_LIBRARY})
    SET(PVI_FOUND "YES")

ENDIF(PVI_LIBRARY)
# - Try to find easylogging++
# Once done this will define
#  
#  EASYLOGGINGPP_FOUND        - system has easylogging++
#  EASYLOGGINGPP_INCLUDE_DIR  - the easylogging++ include directory
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

FIND_PATH(EASYLOGGINGPP_INCLUDE_DIR easylogging++.h
  HINTS ENV TOOLKITS_ROOT
  PATH_SUFFIXES easyloggingpp/src
  PATHS ENV PVI_DIR
)

SET(EASYLOGGINGPP_FOUND "NO")

IF(EASYLOGGINGPP_INCLUDE_DIR)
  SET(EASYLOGGINGPP_FOUND "YES")
ENDIF(EASYLOGGINGPP_INCLUDE_DIR)

# Locate glew
# This module defines
# GLEW_LIBRARY
# GLEW_FOUND, if false, do not try to link to ice
# GLEW_INCLUDE_DIR, where to find the headers
#
# $GLEWDIR is an environment variable that points to the
# root directory of the ice distribution.


# Header files are presumed to be included like
#include <GL/glew.h>

INCLUDE("${CMAKE_SOURCE_DIR}/cmake/Findglew_functions.cmake")
GLEW_FIND_PATH   (GLEW GL/glew.h)
GLEW_FIND_LIBRARY(GLEW GLEW)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

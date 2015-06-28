# Try to find EIGEN library installation
#
# The follwoing variables are optionally searched for defaults
#  EIGEN_ROOT:             Base directory of EIGEN tree to use.
#  $ENV{EIGEN_ROOT:}:      Base directory of EIGEN tree to use.
#
# The following are set after configuration is done: 
#  EIGEN_FOUND
#  EIGEN_INCLUDE_DIR
#  
#  
#
#
#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

SET(EIGEN_FOUND false)

if(EXISTS "$ENV{PCL_ROOT}")
	
	find_path(EIGEN_INCLUDE_DIR_TEMP "Core" PATHS "$ENV{PCL_ROOT}" PATH_SUFFIXES "3rdParty/Eigen/include/Eigen" DOC "")
	
	if(EIGEN_INCLUDE_DIR_TEMP)
		
		SET(EIGEN_INCLUDE_DIR ${EIGEN_INCLUDE_DIR_TEMP})
		SET(EIGEN_FOUND true)
		
	endif(EIGEN_INCLUDE_DIR_TEMP)
	
	mark_as_advanced(EIGEN_INCLUDE_DIR_TEMP)
	
else(EXISTS "$ENV{PCL_ROOT}")

	set(ERR_MSG "Please specify EIGEN directory using EIGEN_ROOT env. variable. Eigen library is needed for PCL")

endif(EXISTS "$ENV{PCL_ROOT}")


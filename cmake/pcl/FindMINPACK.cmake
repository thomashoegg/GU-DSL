# Try to find MINPACK library installation
#
# The follwoing variables are optionally searched for defaults
#  MINPACK_ROOT:             Base directory of MINPACK tree to use.
#  $ENV{MINPACK_ROOT}:       Base directory of MINPACK tree to use.
#
# The following are set after configuration is done: 
#  MINPACK_FOUND
#  MINPACK_INCLUDE_DIR
#  MINPACK_LIBRARIES
#  MINPACK_BIN_DIR
#
#
#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

SET(MINPACK_FOUND 0)

if(EXISTS "$ENV{MINPACK_ROOT}")
	
	set(MINPACK_FOUND_TMP true)
	
	find_path(MINPACK_INCLUDE_DIR "minpack.h" PATHS "$ENV{MINPACK_ROOT}" DOC "")
    
	set(MINPACK_LIBRARIES "")
	set(MINPACK_LIB_COMPONENTS cminpack)
	## Loop over each components
	foreach(__MINPACKLIB ${MINPACK_LIB_COMPONENTS})

			#message(STATUS "MINPACK Lib Name: ${__MINPACKLIB}")
			
			find_library(MINPACK_${__MINPACKLIB}_LIBRARY_DEBUG NAMES "${__MINPACKLIB}" PATHS "$ENV{MINPACK_ROOT}/Debug" NO_DEFAULT_PATH)
			find_library(MINPACK_${__MINPACKLIB}_LIBRARY_RELEASE NAMES "${__MINPACKLIB}" PATHS "$ENV{MINPACK_ROOT}/Release" NO_DEFAULT_PATH)
			
			#Remove the cache value
			set(MINPACK_${__MINPACKLIB}_LIBRARY "" CACHE STRING "" FORCE)
	
			#both debug/release
			if(MINPACK_${__MINPACKLIB}_LIBRARY_DEBUG AND MINPACK_${__MINPACKLIB}_LIBRARY_RELEASE)
					set(MINPACK_${__MINPACKLIB}_LIBRARY debug ${MINPACK_${__MINPACKLIB}_LIBRARY_DEBUG} optimized ${MINPACK_${__MINPACKLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
					set(MINPACK_BIN_DIR debug "${__MINPACKLIB}/Debug" optimized "${__MINPACKLIB}/Release")
			#only debug
			elseif(MINPACK_${__MINPACKLIB}_LIBRARY_DEBUG)
					set(MINPACK_${__MINPACKLIB}_LIBRARY ${MINPACK_${__MINPACKLIB}_LIBRARY_DEBUG}  CACHE STRING "" FORCE)
					set(MINPACK_BIN_DIR "${__MINPACKLIB}/Debug")
			#only release
			elseif(MINPACK_${__MINPACKLIB}_LIBRARY_RELEASE)
					set(MINPACK_${__MINPACKLIB}_LIBRARY ${MINPACK_${__MINPACKLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
					set(MINPACK_BIN_DIR "${__MINPACKLIB}/Release")
			#no library found
			else()
					set(MINPACK_FOUND_TMP false)
			endif()
			
			#Add to the general list
			if(MINPACK_${__MINPACKLIB}_LIBRARY)
					set(MINPACK_LIBRARIES ${MINPACK_LIBRARIES} ${MINPACK_${__MINPACKLIB}_LIBRARY})
			endif(MINPACK_${__MINPACKLIB}_LIBRARY)
			
	endforeach(__MINPACKLIB)
	
	set(MINPACK_FOUND ${MINPACK_FOUND_TMP} CACHE BOOL "" FORCE)
	
	
else(EXISTS "$ENV{MINPACK_ROOT}")

	set(ERR_MSG "Please specify MINPACK directory using MINPACK_ROOT env. variable")

endif(EXISTS "$ENV{MINPACK_ROOT}")


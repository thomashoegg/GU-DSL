# Try to find FLANN library installation
#
# The follwoing variables are optionally searched for defaults
#  FLANN_ROOT:             Base directory of FLANN tree to use.
#  $ENV{FLANN_ROOT}:       Base directory of FLANN tree to use.
#
# The following are set after configuration is done: 
#  FLANN_FOUND
#  FLANN_INCLUDE_DIR
#  FLANN_LIBRARIES
#  FLANN_BIN_DIR
#
#
#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

SET(FLANN_FOUND 0)

if(EXISTS "$ENV{FLANN_ROOT}")
	
	set(FLANN_FOUND_TMP true)
	
	find_path(FLANN_INCLUDE_DIR_TEMP "flann.hpp" PATHS "$ENV{FLANN_ROOT}" PATH_SUFFIXES "src/cpp/flann" DOC "")
	
	if(FLANN_INCLUDE_DIR_TEMP)
		
		find_path(FLANN_INCLUDE_DIR "FLANN" PATHS "$ENV{FLANN_ROOT}" PATH_SUFFIXES "src/cpp/" DOC "")
		
	else(FLANN_INCLUDE_DIR_TEMP)
	
		set(FLANN_FOUND_TMP false)
		
	endif(FLANN_INCLUDE_DIR_TEMP)
	
    
	set(FLANN_LIBRARIES "")
	set(FLANN_LIB_COMPONENTS flann flann_s flann_cpp flann_cpp_s)
	## Loop over each components
	foreach(__FLANNLIB ${FLANN_LIB_COMPONENTS})

			#message(STATUS "FLANN Lib Name: ${__FLANNLIB}")
			
			find_library(FLANN_${__FLANNLIB}_LIBRARY_DEBUG NAMES "${__FLANNLIB}" PATHS "$ENV{FLANN_ROOT}/lib/Debug" NO_DEFAULT_PATH)
			find_library(FLANN_${__FLANNLIB}_LIBRARY_RELEASE NAMES "${__FLANNLIB}" PATHS "$ENV{FLANN_ROOT}/lib/Release" NO_DEFAULT_PATH)
			
			#Remove the cache value
			set(FLANN_${__FLANNLIB}_LIBRARY "" CACHE STRING "" FORCE)
	
			#both debug/release
			if(FLANN_${__FLANNLIB}_LIBRARY_DEBUG AND FLANN_${__FLANNLIB}_LIBRARY_RELEASE)
					set(FLANN_${__FLANNLIB}_LIBRARY debug ${FLANN_${__FLANNLIB}_LIBRARY_DEBUG} optimized ${FLANN_${__FLANNLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
					set(FLANN_BIN_DIR debug "${__FLANNLIB}/lib/Debug" optimized "${__FLANNLIB}/lib/Release")
			#only debug
			elseif(FLANN_${__FLANNLIB}_LIBRARY_DEBUG)
					set(FLANN_${__FLANNLIB}_LIBRARY ${FLANN_${__FLANNLIB}_LIBRARY_DEBUG}  CACHE STRING "" FORCE)
					set(FLANN_BIN_DIR "${__FLANNLIB}/lib/Debug")
			#only release
			elseif(FLANN_${__FLANNLIB}_LIBRARY_RELEASE)
					set(FLANN_${__FLANNLIB}_LIBRARY ${FLANN_${__FLANNLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
					set(FLANN_BIN_DIR "${__FLANNLIB}/lib/Release")
			#no library found
			else()
					set(FLANN_FOUND_TMP false)
			endif()
			
			#Add to the general list
			if(FLANN_${__FLANNLIB}_LIBRARY)
					set(FLANN_LIBRARIES ${FLANN_LIBRARIES} ${FLANN_${__FLANNLIB}_LIBRARY})
			endif(FLANN_${__FLANNLIB}_LIBRARY)
			
	endforeach(__FLANNLIB)
	
	set(FLANN_FOUND ${FLANN_FOUND_TMP} CACHE BOOL "" FORCE)
	
	
else(EXISTS "$ENV{FLANN_ROOT}")

	set(ERR_MSG "Please specify FLANN directory using FLANN_ROOT env. variable")

endif(EXISTS "$ENV{FLANN_ROOT}")


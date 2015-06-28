# - Try to find CUDA DATA PARALLEL PRIMITIVES
# Once done this will define
#  
#  CUDPP_FOUND        	-  system has or not CUDPP
#  CUDPP_INCLUDE_DIR  	- the CUDPP include directory
#  CUDPP_LIBRARIES  	- the CUDPP libraries
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
SET(CUDPP_FOUND 0)

IF (WIN32)
  FIND_PATH(CUDPP_INCLUDE_DIR cudpp.h
    HINTS ENV CUDPP_PATH
    PATH_SUFFIXES "include"
    PATHS ENV CUDPP_PATH
  )
ELSE (WIN32)
  MESSAGE(FATAL_ERROR "Non-Windows Platforms currently not supported!")  
ENDIF (WIN32)

IF(CUDPP_INCLUDE_DIR)

    SET(CUDPP_FOUND_TMP 1)
	
	set(CUDPP_LIBRARIES "")
	set(CUDPP_LIB_COMPONENTS cudpp cudpp_hash)
	## Loop over each components
	foreach(__CUDPPLIB ${CUDPP_LIB_COMPONENTS})
	
		#message(STATUS "CUDPP Lib Name: ${__CUDPPLIB}")
			
		find_library(CUDPP_${__CUDPPLIB}_LIBRARY_DEBUG NAMES "${__CUDPPLIB}32d" PATHS "$ENV{CUDPP_PATH}/lib" NO_DEFAULT_PATH)
		find_library(CUDPP_${__CUDPPLIB}_LIBRARY_RELEASE NAMES "${__CUDPPLIB}32" PATHS "$ENV{CUDPP_PATH}/lib" NO_DEFAULT_PATH)
		
		#Remove the cache value
		set(CUDPP_${__CUDPPLIB}_LIBRARY "" CACHE STRING "" FORCE)

		#both debug/release
		if(CUDPP_${__CUDPPLIB}_LIBRARY_DEBUG AND CUDPP_${__CUDPPLIB}_LIBRARY_RELEASE)
				set(CUDPP_${__CUDPPLIB}_LIBRARY debug ${CUDPP_${__CUDPPLIB}_LIBRARY_DEBUG} optimized ${CUDPP_${__CUDPPLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
		#only debug
		elseif(CUDPP_${__CUDPPLIB}_LIBRARY_DEBUG)
				set(CUDPP_${__CUDPPLIB}_LIBRARY ${CUDPP_${__CUDPPLIB}_LIBRARY_DEBUG}  CACHE STRING "" FORCE)
		#only release
		elseif(CUDPP_${__CUDPPLIB}_LIBRARY_RELEASE)
				set(CUDPP_${__CUDPPLIB}_LIBRARY ${CUDPP_${__CUDPPLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
		#no library found
		else()
				set(CUDPP_FOUND_TMP false)
		endif()
		
		#message(STATUS "CUDPP_LIBRARIES: ${CUDPP_LIBRARIES}")
		
		#Add to the general list
		if(CUDPP_${__CUDPPLIB}_LIBRARY)
				set(CUDPP_LIBRARIES ${CUDPP_LIBRARIES} ${CUDPP_${__CUDPPLIB}_LIBRARY})
		endif(CUDPP_${__CUDPPLIB}_LIBRARY)
			
	endforeach(__CUDPPLIB)
	
	set(CUDPP_FOUND CUDPP_FOUND_TMP)

ELSE(CUDPP_INCLUDE_DIR)

  MESSAGE(FATAL_ERROR "Cuda Data Parallel Primitives library not found")  

ENDIF(CUDPP_INCLUDE_DIR)


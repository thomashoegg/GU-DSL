# Try to find Thrift library installation
#
# The follwoing variables are optionally searched for defaults
#  THRIFT_INCLUDE_DIRS:    Base directory of Thrift
#  $ENV{THRIFT_DIR}:       Base directory of Thrift  to use.
#
# The following are set after configuration is done: 
#  THRIFT_FOUND
#  THRIFT_INCLUDE_DIRS		All thrift include directories
#  THRIFT_LIBRARIES			All thrift libraries
#  THRIFT_COMPILER			The thrift compiler
#  
#
#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

SET(THRIFT_FOUND 0)

if(EXISTS "$ENV{THRIFT_DIR}")
	
	find_path(THRIFT_INCLUDE_DIR "Thrift.h" PATHS "$ENV{THRIFT_DIR}" PATH_SUFFIXES "lib/cpp/src/thrift" DOC "")
	
	find_file(THRIFT_COMPILER "thrift-0.9.1.exe" PATH "$ENV{THRIFT_DIR}")
	
	if(THRIFT_INCLUDE_DIR)
	
		set(THRIFT_FOUND_TMP true)
		
	else(THRIFT_INCLUDE_DIR)
	
		set(THRIFT_FOUND_TMP false)
	
	endif(THRIFT_INCLUDE_DIR)
  
	set(THRIFT_LIBRARIES "")
	set(THRIFT_LIB_COMPONENTS libthrift libthriftnb)
	
	set(THRIFT_INCLUDE_DIRS ${THRIFT_INCLUDE_DIR} "$ENV{THRIFT_DIR}/lib/cpp/src")
	
	message(STATUS "Thrift Dirs: ${THRIFT_INCLUDE_DIRS}")
	
	## Loop over each components
	foreach(__THRIFTLIB ${THRIFT_LIB_COMPONENTS})
	
		if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
			message(STATUS "Detecting Thrift x86 libraries")
			find_library(THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG NAMES "${__THRIFTLIB}" PATHS "$ENV{THRIFT_DIR}/lib/cpp/x86/Debug"  NO_DEFAULT_PATH)
			find_library(THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE NAMES "${__THRIFTLIB}" PATHS "$ENV{THRIFT_DIR}/lib/cpp/x86/Release" NO_DEFAULT_PATH)
		else()
			message(STATUS "Detecting Thrift x64 libraries")
			find_library(THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG NAMES "${__THRIFTLIB}" PATHS "$ENV{THRIFT_DIR}/lib/cpp/x64/Debug"  NO_DEFAULT_PATH)
			find_library(THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE NAMES "${__THRIFTLIB}" PATHS "$ENV{THRIFT_DIR}/lib/cpp/x64/Release" NO_DEFAULT_PATH)
		endif()

	
		set(THRIFT_${__THRIFTLIB}_LIBRARY "" CACHE STRING "" FORCE)

		#both debug/release
		if(THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG AND THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE)
			set(THRIFT_${__THRIFTLIB}_LIBRARY debug ${THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG} optimized ${THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
			
			LIST(APPEND THRIFT_LIBRARIES ${THRIFT_${__THRIFTLIB}_LIBRARY})
		#only debug
		elseif(THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG)
			set(THRIFT_${__THRIFTLIB}_LIBRARY ${THRIFT_${__THRIFTLIB}_LIBRARY_DEBUG}  CACHE STRING "" FORCE)
			
			LIST(APPEND THRIFT_LIBRARIES ${THRIFT_${__THRIFTLIB}_LIBRARY})
		#only release
		elseif(THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE)
			set(THRIFT_${__THRIFTLIB}_LIBRARY ${THRIFT_${__THRIFTLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
			
			LIST(APPEND THRIFT_LIBRARIES ${THRIFT_${__THRIFTLIB}_LIBRARY})
		#no library found
		else()
			set(THRIFT_FOUND_TMP false)
		endif()
			
	endforeach(__THRIFTLIB)
	
	set(THRIFT_FOUND ${THRIFT_FOUND_TMP} CACHE BOOL "" FORCE)
	
	message(STATUS "Thrift Libraries: ${THRIFT_LIBRARIES}")
	
	
else(EXISTS "$ENV{THRIFT_DIR}")

	set(ERR_MSG "Did you install properly Thrift? THRIFT_DIR env. variable should be defined")

endif(EXISTS "$ENV{THRIFT_DIR}")


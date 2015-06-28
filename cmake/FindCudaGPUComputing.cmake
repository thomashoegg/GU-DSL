# - Try to find CUDA GPU COMPUTING SDK
# Once done this will define
#  
#  GPUCOMPUTINGSDK_FOUND        -  system has or not GPUCOMPUTING
#  GPUCOMPUTING_INCLUDE_DIR  - the GPUCOMPUTING include directory
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
SET(GPUCOMPUTINGSDK_FOUND 0)

IF (WIN32)
  FIND_PATH(GPUCOMPUTING_INCLUDE_DIR npp.h
    HINTS ENV NVSDKCOMPUTE_ROOT
    PATH_SUFFIXES include
    PATHS ENV NVSDKCOMPUTE_ROOT
  )
  
  FIND_PATH(GPUCOMPUTING_SAMPLES_DIR helper_functions.h
    HINTS ENV NVCUDASAMPLES_ROOT
    PATH_SUFFIXES common/inc
    PATHS ENV NVCUDASAMPLES_ROOT
  )
  
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_cuda_nv_lib_dir "$ENV{NVSDKCOMPUTE_ROOT}/lib/x64" "$ENV{NVSDKCOMPUTE_ROOT}/lib64" "$ENV{NVSDKCOMPUTE_ROOT}/libx64" )
	else()
		set(_cuda_nv_lib_dir "$ENV{NVSDKCOMPUTE_ROOT}/lib/Win32" "$ENV{NVSDKCOMPUTE_ROOT}/libWin32" )
	endif()
	
	
	## Loop over each path
	foreach(libdir ${_cuda_nv_lib_dir})
		message(STATUS "Libdir to test: ${libdir}")
		if(IS_DIRECTORY "${libdir}")
			message(STATUS "Libdir: ${libdir}")
			SET(GPUCOMPUTING_LIBRARY_DIR ${libdir})
		endif()
	endforeach(libdir)
  
	
ELSE (WIN32)
  MESSAGE(FATAL_ERROR "Non-Windows Platforms currently not supported!")  
ENDIF (WIN32)

IF(GPUCOMPUTING_INCLUDE_DIR AND GPUCOMPUTING_SAMPLES_DIR)

    SET(GPUCOMPUTINGSDK_FOUND 1)

ELSE(GPUCOMPUTING_INCLUDE_DIR AND GPUCOMPUTING_SAMPLES_DIR)

  MESSAGE(FATAL_ERROR "Cuda GPU Computing SDK is needed for the Flow library")  

ENDIF(GPUCOMPUTING_INCLUDE_DIR AND GPUCOMPUTING_SAMPLES_DIR)


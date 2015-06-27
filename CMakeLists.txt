cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()

# add projects here
PROJECT(cvis)

# The version number
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 4)
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set (COPYRIGHT "Copyright (C) 2013-2015")

# clear setup files
file(REMOVE "${CMAKE_SOURCE_DIR}/_Setup/setupDefines.wxi")
file(REMOVE "${CMAKE_SOURCE_DIR}/_Setup/setupInclude.wxi")
file(GLOB setupFiles "_Setup/include/" "*.wxi")
file(REMOVE setupFiles)
file(REMOVE "${CMAKE_SOURCE_DIR}/_Setup/CVisSourceDir.wxi")

#create setup source inlcude file
file(WRITE "${CMAKE_SOURCE_DIR}/_Setup/setupDefines.wxi" "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Include>\n")
file(WRITE "${CMAKE_SOURCE_DIR}/_Setup/setupInclude.wxi" "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Include>\n")
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/CVisSourceDir.wxi.in"
  "${CMAKE_SOURCE_DIR}/_Setup/CVisSourceDir.wxi"
  )


IF($ENV{CUDA_DEVICE_VERSION})

	MESSAGE("Cuda device version is set = $ENV{CUDA_DEVICE_VERSION}")
	SET(CUDA_DEVICE_VERSION $ENV{CUDA_DEVICE_VERSION} CACHE STRING "CUDA Device Version")

ELSE($ENV{CUDA_DEVICE_VERSION})

	#if cuda device version is not define, put 11 (surface will not be activated)
	SET(CUDA_DEVICE_VERSION 11 CACHE STRING "CUDA Device Version")
	MESSAGE("CUDA_DEVICE_VERSION is not an environment variable; Cuda architecture sm_11 will be used")

ENDIF($ENV{CUDA_DEVICE_VERSION})


MESSAGE("CUDA_DEVICE_VERSION = ${CUDA_DEVICE_VERSION}")

#surface needs 2.0 cuda capability
IF(${CUDA_DEVICE_VERSION} GREATER 19)

	IF($ENV{CUDA_DEVICE_USE_SURFACE})
		SET(SURFACE_COMPILER 1)
	ENDIF($ENV{CUDA_DEVICE_USE_SURFACE})

ENDIF(${CUDA_DEVICE_VERSION} GREATER 19)


SET(CMAKE_include_directories_BEFORE ON)
SET(CMAKE_DEBUG_POSTFIX d)
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG DEBUG)

IF(WIN32)
  IF(MSVC)

    ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)
	ADD_DEFINITIONS(-D_VARIADIC_MAX=10)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Zi /MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zi -MP /bigobj")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "{CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Zi -MP /bigobj -D_RELWDEBUG -DRELWDEBUG")
  ENDIF(MSVC)
ENDIF(WIN32)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

#Runtime
ADD_SUBDIRECTORY(Container ${CMAKE_SOURCE_DIR}/Container)
ADD_SUBDIRECTORY(Core ${CMAKE_SOURCE_DIR}/Core)
ADD_SUBDIRECTORY(Geometry ${CMAKE_SOURCE_DIR}/Geometry)
ADD_SUBDIRECTORY(Viewer ${CMAKE_SOURCE_DIR}/Viewer)
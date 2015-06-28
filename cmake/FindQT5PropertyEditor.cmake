# - Try to find PMD SDK
# Once done this will define
#  
#  PMDSDK_FOUND        - system has PMDSDK
#  PMDSDK_INCLUDE_DIR  - the PMDSDK include directory
#  PMDSDK_LIBRARIES    - Link these to use PMDSDK and GLU
#   

#=============================================================================
# Copyright 2015 Thomas Hoegg
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

IF (WIN32)

#D:\Toolkits\Qt\5.1.0_vs2012_x64\qttools\include\;
#D:\Toolkits\Qt\5.1.0_vs2012_x64\qttools\include\QtDesigner;
#D:\Toolkits\Qt\5.1.0_vs2012_x64\qttools\src\designer\src\components\propertyeditor;
#D:\Toolkits\Qt\5.1.0_vs2012_x64\qttools\src\designer\src\lib\shared;
#D:\Toolkits\Qt\5.1.0_vs2012_x64\qttools\src\designer\src\lib\components;
#D:\Toolkits\Qt\5.2.1_vs2012_x64\qttools\src\shared\qtpropertybrowser
#D:\Toolkits\Qt\5.3.2_vs2012_x86\5.3\Src\qtbase\src\widgets\itemviews

SET(QT_ROOT_DIR $ENV{QT5}/../qttools)

SET(QT5_PROPERTY_EDITOR_FOUND "YES")

if(IS_DIRECTORY "${QT_ROOT_DIR}/include")
	LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/include")
	if(IS_DIRECTORY "${QT_ROOT_DIR}/include/QtDesigner")
		LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/include/QtDesigner")
		if(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/components/propertyeditor")
			LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/src/designer/src/components/propertyeditor")
			
			if(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/shared")
				LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/src/designer/src/lib/shared")
				
				if(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/components")
					LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/src/designer/src/lib/components")
				else(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/components")
					SET(QT5_PROPERTY_EDITOR_FOUND "NO")
				endif(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/components")
			else(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/shared")
				SET(QT5_PROPERTY_EDITOR_FOUND "NO")
			endif(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/lib/shared")
		
		else(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/components/propertyeditor")
			SET(QT5_PROPERTY_EDITOR_FOUND "NO")
		endif(IS_DIRECTORY "${QT_ROOT_DIR}/src/designer/src/components/propertyeditor")

	else (IS_DIRECTORY "${QT_ROOT_DIR}/include/QtDesigner")
		SET(QT5_PROPERTY_EDITOR_FOUND "NO")
	endif(IS_DIRECTORY "${QT_ROOT_DIR}/include/QtDesigner")

	if(IS_DIRECTORY "${QT_ROOT_DIR}/src/shared/qtpropertybrowser")
		LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "${QT_ROOT_DIR}/src/shared/qtpropertybrowser")
	endif(IS_DIRECTORY "${QT_ROOT_DIR}/src/shared/qtpropertybrowser")
else(IS_DIRECTORY "${QT_ROOT_DIR}/include")
	SET(QT5_PROPERTY_EDITOR_FOUND "NO")
endif(IS_DIRECTORY "${QT_ROOT_DIR}/include")

if(IS_DIRECTORY "$ENV{QT5}/src/widgets/itemviews")
	LIST(APPEND QT_PROPERTY_EDITOR_INCLUDE_DIRS "$ENV{QT5}/src/widgets/itemviews")
else(IS_DIRECTORY "$ENV{QT5}/src/widgets/itemviews")
	SET(QT5_PROPERTY_EDITOR_FOUND "NO")
endif(IS_DIRECTORY "$ENV{QT5}/src/widgets/itemviews")


set(QT_PROPERTYEDITOR_FOUND_TMP true)

set(QT_PROPERTYEDITOR_LIBRARIES "")
set(QT_PROPERTYEDITOR_LIB_COMPONENTS Qt5Designer Qt5DesignerComponents)

## Loop over each components
foreach(__QTLIB ${QT_PROPERTYEDITOR_LIB_COMPONENTS})

	find_library(QT_${__QTLIB}_LIBRARY_DEBUG NAMES "${__QTLIB}d" PATHS "$ENV{QT5}/lib" "$ENV{QT_PROPERTY_EDITIOR_LIB_DIR}")
	find_library(QT_${__QTLIB}_LIBRARY_RELEASE NAMES "${__QTLIB}" PATHS "$ENV{QT5}/lib" "$ENV{QT_PROPERTY_EDITIOR_LIB_DIR}")
	
	set(QT_${__QTLIB}_LIBRARY "" CACHE STRING "" FORCE)

	#both debug/release
	if(QT_${__QTLIB}_LIBRARY_DEBUG AND QT_${__QTLIB}_LIBRARY_RELEASE)
		set(QT_${__QTLIB}_LIBRARY debug ${QT_${__QTLIB}_LIBRARY_DEBUG} optimized ${QT_${__QTLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
		
		LIST(APPEND QT_PROPERTYEDITOR_LIBRARIES ${QT_${__QTLIB}_LIBRARY})
	#only debug
	elseif(QT_${__QTLIB}_LIBRARY_DEBUG)
		set(QT_${__QTLIB}_LIBRARY ${QT_${__QTLIB}_LIBRARY_DEBUG}  CACHE STRING "" FORCE)
		
		LIST(APPEND QT_PROPERTYEDITOR_LIBRARIES ${QT_${__QTLIB}_LIBRARY})
	#only release
	elseif(QT_${__QTLIB}_LIBRARY_RELEASE)
		set(QT_${__QTLIB}_LIBRARY ${QT_${__QTLIB}_LIBRARY_RELEASE}  CACHE STRING "" FORCE)
		
		LIST(APPEND QT_PROPERTYEDITOR_LIBRARIES ${QT_${__QTLIB}_LIBRARY})
	#no library found
	else()
		set(QT_PROPERTYEDITOR_FOUND_TMP false)
	endif()
		
endforeach(__QTLIB)

set(QT_PROPERTY_EDITOR_FOUND ${QT_PROPERTYEDITOR_FOUND_TMP} CACHE BOOL "" FORCE)

message(STATUS "QT Property Editor Libraries: ${QT_PROPERTYEDITOR_LIBRARIES}")


#  FIND_LIBRARY(PMDSDK_LIBRARY pmdaccess2
#    HINTS ENV PMDSDK_DIR
#    PATH_SUFFIXES lib
 #   PATHS ENV PMDSDK_DIR
#  )
ELSE (WIN32)
  MESSAGE(FATAL_ERROR "Non-Windows Platforms currently not supported!")  
ENDIF (WIN32)


#IF(PMDSDK_LIBRARY)
#
    #SET(PMDSDK_LIBRARIES ${PMDSDK_LIBRARY})
    SET(QT5_PROPERTY_EDITOR_FOUND "YES")

#ENDIF(PMDSDK_LIBRARY)
cmake_minimum_required(VERSION 2.8)

SET(TARGETNAME "Core")

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()

SET(Boost_DETAILED_FAILURE_MSG	 ON)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
SET(Boost_DEBUG					OFF)


FIND_PACKAGE(Qt5Core REQUIRED) 
FIND_PACKAGE(Qt5Widgets REQUIRED) 
FIND_PACKAGE(Qt5Xml REQUIRED) 
FIND_PACKAGE(Qt5Network REQUIRED) 
FIND_PACKAGE(Qt5OpenGL REQUIRED) 
FIND_PACKAGE(PCL 1.7 REQUIRED)
FIND_PACKAGE(Boost COMPONENTS thread exception system date_time program_options REQUIRED) 

#INCLUDE(FindCUDA)
INCLUDE(FindOpenGL)
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/FindGLEW.cmake")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ListMacros.cmake")
#INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindCudaGPUComputing.cmake")
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindLibEvent.cmake")
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindOpenCV.cmake")
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindEasyloggingpp.cmake")

SET (DEPENDENCIES
	Geometry
	Viewer
	Container
)

LINK_DIRECTORIES(
  ${GLEW_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${PCL_LIBRARY_DIRS}
)

SET (INC_DIRECTORY
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  #${CUDA_INCLUDE_DIRS}
  #${GPUCOMPUTING_INCLUDE_DIR}
  #${GPUCOMPUTING_SAMPLES_DIR}
  #${CMAKE_SOURCE_DIR}/GUI/inc
  ${CMAKE_SOURCE_DIR}/commmon_inc/interfaces
  ${LIBEVENT_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${EASYLOGGINGPP_INCLUDE_DIR}
)



SET (LIBRARY_FILES  
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${Boost_LIBRARIES}
  ${PCL_LIBRARIES}
  ${LIBEVENT_LIB}
  ${OpenCV_LIBS}
)

# fetch input files
FILE(GLOB_RECURSE H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} inc/*.h inc/*.inl inc/*.hpp)
FILE(GLOB_RECURSE CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
FILE(GLOB_RECURSE MOC_INPUT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} inc/*.qt.h inc/*.qt.hpp)
FILE(GLOB_RECURSE UI_INPUT  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
FILE(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} res/*.*)
FILE(GLOB_RECURSE GLSL_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} dat/*.glsl)
FILE(GLOB_RECURSE UI_RESOURCES  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.qrc)
FILE(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} res/*.*)

#SET(remove_skel ".+\\.skeleton.+")
	
Message("MOC FILES = ${MOC_INPUT}")

# add header and libs
FOREACH(DEP ${DEPENDENCIES})
  LIST(APPEND MOC_INC " -I${CMAKE_SOURCE_DIR}/${DEP}/inc")
  LIST(APPEND TMP ${CMAKE_SOURCE_DIR}/${DEP})
  LIST(APPEND TMP ${CMAKE_SOURCE_DIR}/${DEP}/inc)
  LIST(APPEND LIBRARY_FILES ${DEP})
ENDFOREACH(DEP)

LIST(APPEND MOC_INC " -I${CMAKE_CURRENT_SOURCE_DIR}/inc")

# generate Qt files
QT5_WRAP_CPP(MOC_FILES ${MOC_INPUT} OPTIONS ${MOC_INC})
QT5_WRAP_UI (UI_FILES  ${UI_INPUT})
QT5_ADD_RESOURCES(UI_RESOURCES ${UI_RESOURCES})

#CUDA_INCLUDE_DIRECTORIES( ${GPUCOMPUTING_INCLUDE_DIR} ${GPUCOMPUTING_SAMPLES_DIR})

# define project
ADD_LIBRARY(${TARGETNAME} SHARED ${H_FILES} ${CPP_FILES} ${MOC_FILES} ${UI_FILES} ${UI_INPUT} ${GLSL_FILES} ${UI_RESOURCES} ${RESOURCES})

message(STATUS "TMP=${TMP}")
INCLUDE_DIRECTORIES(
  inc
  ${TMP}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${INC_DIRECTORY}
  ${Boost_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)

IF (LIBRARY_FILES) 
  TARGET_LINK_LIBRARIES(${TARGETNAME} ${LIBRARY_FILES})
ENDIF (LIBRARY_FILES) 

IF (DEPENDENCIES) 
  ADD_DEPENDENCIES(${TARGETNAME} ${DEPENDENCIES}) 
ENDIF(DEPENDENCIES)

# define lib export symbol
STRING(TOUPPER ${TARGETNAME} TMP)
SET_TARGET_PROPERTIES(${TARGETNAME} PROPERTIES DEFINE_SYMBOL ${TMP}_EXPORT)

qt5_use_modules(${TARGETNAME} Core Widgets Xml Network OpenGL)

add_definitions(-DUNICODE -D_UNICODE -DUSE_BOOST_THREAD)

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION "inc/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing" REGULAR_EXPRESSION "Processing/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Object" REGULAR_EXPRESSION "Processing/Object/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Object\\Impl" REGULAR_EXPRESSION "Processing/Object/Impl/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Component" REGULAR_EXPRESSION "Processing/Component/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Misc" REGULAR_EXPRESSION "Processing/Misc/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Ports" REGULAR_EXPRESSION "Processing/Ports/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Project" REGULAR_EXPRESSION "Processing/Project/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Processing\\Interfaces" REGULAR_EXPRESSION "Processing/Interfaces/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Images" REGULAR_EXPRESSION "Images/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\GLOBAL_INC" REGULAR_EXPRESSION "GLOBAL_INC/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Misc" REGULAR_EXPRESSION "Misc/[^/]+\\.(h|inl)")
#SOURCE_GROUP("Header Files\\Rendering" REGULAR_EXPRESSION "Rendering/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Communication" REGULAR_EXPRESSION "Communication/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Communication\\Interfaces" REGULAR_EXPRESSION "Communication/Interfaces/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Buffer" REGULAR_EXPRESSION "Buffer/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\DebugOutput" REGULAR_EXPRESSION "DebugOutput/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Calibration" REGULAR_EXPRESSION "Calibration/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Project" REGULAR_EXPRESSION "Project/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Camera" REGULAR_EXPRESSION "Camera/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Mediation" REGULAR_EXPRESSION "Mediation/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Mediation\\Interfaces" REGULAR_EXPRESSION "Mediation/Interfaces/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Mediation\\Events" REGULAR_EXPRESSION "Mediation/Events/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Mediation\\EventListeners" REGULAR_EXPRESSION "Mediation/EventListeners/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Utilities" REGULAR_EXPRESSION "Utilities/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\OpenGL" REGULAR_EXPRESSION "OpenGL/[^/]+\\.(h|inl)")
SOURCE_GROUP("Header Files\\Gui" REGULAR_EXPRESSION "Gui/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Gui\\Interfaces" REGULAR_EXPRESSION "Gui/Interfaces/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Plugin" REGULAR_EXPRESSION "Plugin/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Plugin\\Interfaces" REGULAR_EXPRESSION "Plugin/Interfaces/[^/]+\\.(h|inl|hpp)")
SOURCE_GROUP("Header Files\\Plugin\\Factory" REGULAR_EXPRESSION "Plugin/Factory/[^/]+\\.(h|inl|hpp)")

SOURCE_GROUP("Source Files" REGULAR_EXPRESSION "src/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing" REGULAR_EXPRESSION "Processing/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Object" REGULAR_EXPRESSION "Processing/Object/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Component" REGULAR_EXPRESSION "Processing/Component/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Misc" REGULAR_EXPRESSION "Processing/Misc/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Ports" REGULAR_EXPRESSION "Processing/Ports/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Project" REGULAR_EXPRESSION "Processing/Project/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Processing\\Interfaces" REGULAR_EXPRESSION "Processing/Interfaces/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Images" REGULAR_EXPRESSION "Images/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Misc" REGULAR_EXPRESSION "Misc/[^/]+\\.cpp")
#SOURCE_GROUP("Source Files\\Rendering" REGULAR_EXPRESSION "Rendering/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Communication" REGULAR_EXPRESSION "Communication/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Communication\\Interfaces" REGULAR_EXPRESSION "Communication/Interfaces/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\DebugOutput" REGULAR_EXPRESSION "DebugOutput/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Calibration" REGULAR_EXPRESSION "Calibration/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Project" REGULAR_EXPRESSION "Project/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Camera" REGULAR_EXPRESSION "Camera/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Mediation" REGULAR_EXPRESSION "Mediation/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Mediation\\Interfaces" REGULAR_EXPRESSION "Mediation/Interfaces/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Mediation\\Events" REGULAR_EXPRESSION "Mediation/Events/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Mediation\\EventListeners" REGULAR_EXPRESSION "Mediation/EventListeners/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Utilities" REGULAR_EXPRESSION "Utilities/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\OpenGL" REGULAR_EXPRESSION "OpenGL/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Gui" REGULAR_EXPRESSION "Gui/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Gui\\Interfaces" REGULAR_EXPRESSION "Gui/Interfaces/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Plugin" REGULAR_EXPRESSION "Plugin/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Plugin\\Interfaces" REGULAR_EXPRESSION "Plugin/Interfaces/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Plugin\\Factory" REGULAR_EXPRESSION "Plugin/Factory/[^/]+\\.cpp")

SOURCE_GROUP("Shader" REGULAR_EXPRESSION  "dat/[^/]+\\.glsl")

SOURCE_GROUP("Generated Files" REGULAR_EXPRESSION "ui_|moc_")
SOURCE_GROUP("Resource Files" REGULAR_EXPRESSION "res/[^/]+")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#INSTALL(TARGETS ${TARGETNAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)

#ADD_CUSTOM_COMMAND(TARGET ${TARGETNAME} POST_BUILD 
#  COMMAND cd \"${CMAKE_SOURCE_DIR}/bin\"
#  COMMAND IF NOT EXIST Debug md Debug
#  COMMAND IF NOT EXIST Release md Release
#  COMMAND cd \"${CMAKE_CURRENT_SOURCE_DIR}/dat\"
#  COMMAND xcopy *.glsl  \"${CMAKE_SOURCE_DIR}/bin/Debug/GLSL\"  /i /y
#  COMMAND xcopy *.glsl  \"${CMAKE_SOURCE_DIR}/bin/Release/GLSL\"  /i /y
#)

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/${TARGETNAME}.wxi.in"
  "${CMAKE_SOURCE_DIR}/_Setup/include/${TARGETNAME}.wxi"
  )

 file(APPEND "${CMAKE_SOURCE_DIR}/_Setup/setupInclude.wxi" "<?include include/${TARGETNAME}.wxi ?> \n")
 file(APPEND "${CMAKE_SOURCE_DIR}/_Setup/setupDefines.wxi" "<?define ${TARGETNAME} ?> \n")
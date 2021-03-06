cmake_minimum_required(VERSION 2.8)

SET(TARGETNAME "Geometry")

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
FIND_PACKAGE(Boost COMPONENTS thread exception system date_time REQUIRED) 

#INCLUDE(FindCUDA)
INCLUDE(FindOpenGL)
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindGLEW.cmake")
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/ListMacros.cmake")
#INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindCudaGPUComputing.cmake")
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/FindEasyloggingpp.cmake")


SET (DEPENDENCIES
)

LINK_DIRECTORIES(
  ${GLEW_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${PCL_LIBRARY_DIRS}
)

SET (INC_DIRECTORY
  ${OPENGL_INCLUDE_DIR}  
 # ${CUDA_INCLUDE_DIRS}
  ${GPUCOMPUTING_INCLUDE_DIR}
  ${GPUCOMPUTING_SAMPLES_DIR}
  ${GLEW_INCLUDE_DIR}
  ${EASYLOGGINGPP_INCLUDE_DIR}
)

SET (LIBRARY_FILES  
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES}
  ${GLEW_LIBRARY}
  ${Boost_LIBRARIES}
  ${PCL_LIBRARIES}
)

# fetch input files
FILE(GLOB_RECURSE H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} inc/*.h inc/*.inl inc/*.hpp)
FILE(GLOB_RECURSE CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
FILE(GLOB_RECURSE MOC_INPUT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} inc/*.qt.h)
FILE(GLOB_RECURSE UI_INPUT  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
FILE(GLOB_RECURSE GLSL_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} dat/*.glsl)
FILE(GLOB_RECURSE UI_RESOURCES  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.qrc)
FILE(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} res/*.*)


# add header and libs
FOREACH(DEP ${DEPENDENCIES})
  LIST(APPEND MOC_INC " -I${CMAKE_SOURCE_DIR}/${DEP}/inc")
  LIST(APPEND TMP ${CMAKE_SOURCE_DIR}/${DEP}/inc)
  LIST(APPEND LIBRARY_FILES ${DEP})
ENDFOREACH(DEP)

# generate Qt files
QT5_WRAP_CPP(MOC_FILES ${MOC_INPUT} OPTIONS ${MOC_INC})
QT5_WRAP_UI (UI_FILES  ${UI_INPUT})
QT5_ADD_RESOURCES(UI_RESOURCES ${UI_RESOURCES})

#CUDA_INCLUDE_DIRECTORIES( ${GPUCOMPUTING_INCLUDE_DIR} ${GPUCOMPUTING_SAMPLES_DIR})

# define project
ADD_LIBRARY(${TARGETNAME} SHARED ${H_FILES} ${CPP_FILES} ${MOC_FILES} ${UI_FILES} ${UI_INPUT} ${GLSL_FILES} ${UI_RESOURCES} ${RESOURCES})

# add header and libs
FOREACH(DEP ${DEPENDENCIES})
  LIST(APPEND TMP ${CMAKE_SOURCE_DIR}/${DEP})
  LIST(APPEND TMP ${CMAKE_SOURCE_DIR}/${DEP}/inc)
  LIST(APPEND LIBRARY_FILES ${DEP})
ENDFOREACH(DEP)


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

add_definitions(-DUNICODE -D_UNICODE)

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION "inc/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\AMP" REGULAR_EXPRESSION "inc/AMP/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Filter" REGULAR_EXPRESSION "inc/Filter/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Misc" REGULAR_EXPRESSION "inc/Misc/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Pipeline" REGULAR_EXPRESSION "inc/Pipeline/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Plane" REGULAR_EXPRESSION "inc/Plane/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Primitives" REGULAR_EXPRESSION "inc/Primitives/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\BoundingBoxes" REGULAR_EXPRESSION "inc/BoundingBoxes/[^/]+\\.(hpp|h|inl)")
SOURCE_GROUP("Header Files\\Camera" REGULAR_EXPRESSION "inc/Camera/[^/]+\\.(hpp|h|inl)")

SOURCE_GROUP("Source Files" REGULAR_EXPRESSION "src/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\AMP" REGULAR_EXPRESSION "AMP/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Filter" REGULAR_EXPRESSION "Filter/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Misc" REGULAR_EXPRESSION "Misc/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Pipeline" REGULAR_EXPRESSION "Pipeline/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Plane" REGULAR_EXPRESSION "Plane/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\Primitives" REGULAR_EXPRESSION "Primitives/[^/]+\\.cpp")
SOURCE_GROUP("Source Files\\BoundingBoxes" REGULAR_EXPRESSION "BoundingBoxes/[^/]+\\.cpp")
SOURCE_GROUP("Shader" REGULAR_EXPRESSION  "dat/[^/]+\\.glsl")

SOURCE_GROUP("Generated Files" REGULAR_EXPRESSION "ui_|moc_")
SOURCE_GROUP("Resource Files" REGULAR_EXPRESSION "res/[^/]+")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#INSTALL(TARGETS ${TARGETNAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)

#ADD_CUSTOM_COMMAND(TARGET ${TARGETNAME} POST_BUILD 
  #COMMAND cd \"${CMAKE_SOURCE_DIR}/bin\"
  #COMMAND IF NOT EXIST Debug md Debug
  #COMMAND IF NOT EXIST Release md Release
  #COMMAND cd \"${CMAKE_CURRENT_SOURCE_DIR}/dat\"
  #COMMAND xcopy *.glsl  \"${CMAKE_SOURCE_DIR}/bin/Debug/GLSL\"  /i /y
  #COMMAND xcopy *.glsl  \"${CMAKE_SOURCE_DIR}/bin/Release/GLSL\"  /i /y
#)

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/${TARGETNAME}.wxi.in"
  "${CMAKE_SOURCE_DIR}/_Setup/include/${TARGETNAME}.wxi"
  )

 file(APPEND "${CMAKE_SOURCE_DIR}/_Setup/setupInclude.wxi" "<?include include/${TARGETNAME}.wxi ?> \n")
 file(APPEND "${CMAKE_SOURCE_DIR}/_Setup/setupDefines.wxi" "<?define ${TARGETNAME} ?> \n")
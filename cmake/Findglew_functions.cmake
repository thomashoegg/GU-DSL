#
# This CMake file contains two macros to assist with searching for GLEW
# libraries. This file is originally privided by cmake/osg community and has been adapted
# to svt/glew by the SVT Group. 
#
# NOTE: the HINTS pathes seem not being searched in CMake 2.6 (-SVT Group)
# Therefore we use effectively just the paths in PATHS
#
function(GLEW_FIND_PATH module header)
   string(TOUPPER ${module} module_uc)
   
   # Try the user's environment request before anything else.
   find_path(${module_uc}_INCLUDE_DIR ${header}
       HINTS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
       PATH_SUFFIXES include
       PATHS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
            ~/Library/Frameworks
            /Library/Frameworks
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
            [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GLEWDIR]
   )
   
endfunction(GLEW_FIND_PATH module header)

function(GLEW_FIND_LIBRARY module library)
   string(TOUPPER ${module} module_uc)

  IF (WIN32)
  
    find_library(${module_uc}_LIBRARY
        NAMES ${library}32
        HINTS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
        PATH_SUFFIXES lib64 lib
        PATHS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
            [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GLEWDIR]
    )
    
   ELSE (WIN32)
   
    find_library(${module_uc}_LIBRARY
        NAMES ${library}
        HINTS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
        PATH_SUFFIXES lib64 lib
        PATHS
            ENV ${module_uc}_DIR
            ENV GLEW_DIR
            ENV GLEWDIR
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
    )
            
   ENDIF (WIN32)
   
   #NOTE: no debug lib for win available

endfunction(GLEW_FIND_LIBRARY module library)

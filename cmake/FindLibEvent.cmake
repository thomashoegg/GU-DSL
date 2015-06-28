set(LibEvent_EXTRA_PREFIXES "$ENV{LIBEVENT_DIR}")

foreach(prefix ${LibEvent_EXTRA_PREFIXES})
    list(APPEND LibEvent_INCLUDE_PATHS "${prefix}/include")
   # list(APPEND LibEvent_LIB_PATHS "${prefix}/lib")
	list(APPEND LibEvent_LIB_PATHS "${prefix}/lib/Release")
endforeach()

message(STATUS "LIBEVENT Include Search Path: ${LibEvent_INCLUDE_PATHS}") 
message(STATUS "LIBEVENT Library Search Path: ${LibEvent_LIB_PATHS}") 
find_path(LIBEVENT_INCLUDE_DIR event.h PATHS ${LibEvent_INCLUDE_PATHS})
find_library(LIBEVENT_LIB NAMES event PATHS ${LibEvent_LIB_PATHS})
 
if (LIBEVENT_LIB AND LIBEVENT_INCLUDE_DIR)
  set(LibEvent_FOUND TRUE)
  set(LIBEVENT_LIB ${LIBEVENT_LIB})
else ()
  set(LibEvent_FOUND FALSE)
endif ()
 
if (LibEvent_FOUND)
  if (NOT LibEvent_FIND_QUIETLY)
    message(STATUS "Found libevent: ${LIBEVENT_LIB}")
  endif ()
else ()
    if (LibEvent_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find libevent.")
    endif ()
    message(STATUS "libevent NOT found.")
endif ()
 
mark_as_advanced(
    LIBEVENT_LIB
    LIBEVENT_INCLUDE_DIR
  )
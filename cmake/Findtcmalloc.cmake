# - Find tcmalloc
# Find the native tcmalloc library
#
#  tcmalloc_LIBRARIES   - List of libraries when using tcmalloc.
#  tcmalloc_FOUND       - True if tcmalloc found.

# if (USE_TCMALLOC)
#   set(tcmalloc_NAMES tcmalloc)
# else ()
#   set(tcmalloc_NAMES tcmalloc_minimal tcmalloc)
# endif ()
set(tcmalloc_NAMES
    tcmalloc_minimal
    libtcmalloc_minimal
    tcmalloc
    tcmalloc_minimal4
    libtcmalloc_minimal.so.4)

find_library(tcmalloc_LIBRARY
  NAMES ${tcmalloc_NAMES}
  PATHS ${HT_DEPENDENCY_LIB_DIR} /lib /usr/lib /usr/local/lib /opt/local/lib
)

if (tcmalloc_LIBRARY)
  set(tcmalloc_FOUND TRUE)
  set( tcmalloc_LIBRARIES ${tcmalloc_LIBRARY} )
else ()
  set(tcmalloc_FOUND FALSE)
  set( tcmalloc_LIBRARIES )
endif ()

if (tcmalloc_FOUND)
  message(STATUS "Found tcmalloc: ${tcmalloc_LIBRARY}")
else ()
  message(STATUS "Not Found tcmalloc: ${tcmalloc_LIBRARY}")
  if (tcmalloc_FIND_REQUIRED)
    message(STATUS "Looked for tcmalloc libraries named ${tcmalloc_NAMES}.")
    message(FATAL_ERROR "Could NOT find tcmalloc library")
  endif ()
endif ()

mark_as_advanced(
  tcmalloc_LIBRARY
  )

set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue)

# add_subdirectory(readerwriterqueue)

install(FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue/atomicops.h 
                ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue/readerwriterqueue.h 
                ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue/readerwritercircularbuffer.h 
        DESTINATION
                ${CMAKE_INSTALL_INCLUDEDIR}
)
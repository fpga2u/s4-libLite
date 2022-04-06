set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue)

# add_subdirectory(concurrentqueue)
# set_property(GLOBAL APPEND PROPERTY GLOBAL_LIBS_LIST concurrentqueue)


install(
        FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue/blockingconcurrentqueue.h 
                ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue/concurrentqueue.h 
                ${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue/lightweightsemaphore.h 
        DESTINATION 
                ${CMAKE_INSTALL_INCLUDEDIR}
)

set(SPDLOG_INSTALL ON CACHE BOOL "Generate the install target")

add_subdirectory(spdlog)
set_property(GLOBAL APPEND PROPERTY GLOBAL_LIBS_LIST spdlog_header_only)

# include_directories(SYSTEM 
#   spdlog/include
# )
# Module folder
# set_target_properties(spdlog PROPERTIES FOLDER thirdparty/spdlog)

# set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)
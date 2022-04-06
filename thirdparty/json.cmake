set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install ON CACHE BOOL "Install CMake targets during install step.")
add_subdirectory(json)

# include_directories(SYSTEM 
#   json/single_include
# )
# Module folder
# set_target_properties(json PROPERTIES FOLDER thirdparty/json)

# set_property(GLOBAL APPEND PROPERTY GLOBAL_INCLUDE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include)
set_property(GLOBAL APPEND PROPERTY GLOBAL_LIBS_LIST nlohmann_json::nlohmann_json)

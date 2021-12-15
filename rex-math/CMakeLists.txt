add_library(rex-math INTERFACE)

target_include_directories(rex-math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(rex-math INTERFACE rex-options)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/regex/. regex_src)
add_library(regex ${regex_src})
include_directories(${PROJECT_SOURCE_DIR}/src/common)
target_link_libraries(regex )
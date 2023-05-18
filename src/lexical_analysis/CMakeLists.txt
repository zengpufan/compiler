aux_source_directory(${PROJECT_SOURCE_DIR}/src/lexical_analysis/. lexical_analysis_src)
add_library(lexical_analysis ${lexical_analysis_src})
include_directories(${PROJECT_SOURCE_DIR}/src/regex ${PROJECT_SOURCE_DIR}/src/common)
target_link_libraries(lexical_analysis regex common)
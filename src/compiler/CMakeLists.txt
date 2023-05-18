aux_source_directory(${PROJECT_SOURCE_DIR}/src/compiler/. compiler_src)
add_library(compiler ${compiler_src})

include_directories(
	${PROJECT_SOURCE_DIR}/src/regex
	${PROJECT_SOURCE_DIR}/src/lexical_analysis
	${PROJECT_SOURCE_DIR}/src/common
	${PROJECT_SOURCE_DIR}/src/LL1
)
target_link_libraries(compiler lexical_analysis regex LL1 common ) 
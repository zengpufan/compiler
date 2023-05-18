aux_source_directory(${PROJECT_SOURCE_DIR}/src/LL1/. LL1_src)
add_library(LL1 ${LL1_src})
include_directories(
    ${PROJECT_SOURCE_DIR}/src/common
)
target_link_libraries(LL1 common)
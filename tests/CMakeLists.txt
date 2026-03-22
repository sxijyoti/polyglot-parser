include(FetchContent)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)

add_library(unity_lib STATIC ${unity_SOURCE_DIR}/src/unity.c)
target_include_directories(unity_lib PUBLIC ${unity_SOURCE_DIR}/src)

set(TEST_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/adapters
  ${CMAKE_SOURCE_DIR}/ir
  ${CMAKE_SOURCE_DIR}/graph
  ${CMAKE_SOURCE_DIR}/exporter
)

function(add_poly_test test_name test_source)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name} PRIVATE polygot_parser_core unity_lib)
  target_include_directories(${test_name} PRIVATE ${TEST_INCLUDE_DIRS})
  target_compile_definitions(${test_name} PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    PARSER_BIN_PATH="$<TARGET_FILE:polygot_parser>"
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_poly_test(test_ir unit/test_ir.c)
add_poly_test(test_adapters unit/test_adapters.c)
add_poly_test(test_parser unit/test_parser.c)
add_poly_test(test_graph unit/test_graph.c)
add_poly_test(test_exporter unit/test_exporter.c)
add_poly_test(test_cli integration/test_cli.c)

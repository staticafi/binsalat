set(THIS_TARGET_NAME utils_tests)

set(SOURCE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/dynamic_bitset_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/sparse_map_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/sparse_set_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/type_list_compilation_test.cpp

  # view subfolder
  ${CMAKE_CURRENT_LIST_DIR}/view/flattened_cfg_view_test.cpp
)

add_executable(${THIS_TARGET_NAME} ${SOURCE_FILES})
target_compile_features(${THIS_TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${THIS_TARGET_NAME}
  PRIVATE
    optimizer_tests_common
    Catch2::Catch2WithMain
)

catch_discover_tests(${THIS_TARGET_NAME}
  ADD_TAGS_AS_LABELS
)
if(DEFINED BINSALAT_INSTALL_DIR)
    install(TARGETS ${THIS_TARGET_NAME} DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer")
endif(DEFINED BINSALAT_INSTALL_DIR)

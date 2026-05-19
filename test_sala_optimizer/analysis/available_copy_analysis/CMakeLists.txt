set(THIS_TARGET_NAME available_copy_analysis_chain_tests)

set(SOURCE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/available_copy_analysis_chain_test.cpp
)

set(AVAILABLE_COPY_PROGRAMS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/available_copy_golden
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
  install(TARGETS ${THIS_TARGET_NAME}
    RUNTIME DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer"
  )

  install(DIRECTORY "${AVAILABLE_COPY_PROGRAMS_SRC_DIR}/"
    DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer/available_copy_golden"
  )
endif()

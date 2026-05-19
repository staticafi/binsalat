set(THIS_TARGET_NAME liveness_analysis_chain_tests)

set(SOURCE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/liveness_analysis_chain_test.cpp
)

set(LIVENESS_PROGRAMS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/liveness_golden
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

add_custom_command(TARGET ${THIS_TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${LIVENESS_PROGRAMS_SRC_DIR}"
          "$<TARGET_FILE_DIR:${THIS_TARGET_NAME}>/liveness_golden"
  COMMENT "Copying liveness golden files next to ${THIS_TARGET_NAME}"
  VERBATIM
)

if(DEFINED BINSALAT_INSTALL_DIR)
  install(TARGETS ${THIS_TARGET_NAME}
    RUNTIME DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer"
  )

  install(DIRECTORY "${LIVENESS_PROGRAMS_SRC_DIR}/"
    DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer/liveness_golden"
  )
endif()

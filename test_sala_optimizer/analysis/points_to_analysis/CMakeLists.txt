set(THIS_TARGET_NAME points_to_analysis_chain_test)

set(SOURCE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/points_to_analysis_chain_test.cpp
)

set(POINTS_TO_PROGRAMS_SRC_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/programs
)

add_executable(${THIS_TARGET_NAME} ${SOURCE_FILES})

target_compile_features(${THIS_TARGET_NAME} PRIVATE cxx_std_20)

if(DEFINED BINSALAT_INSTALL_DIR)
  set(POINTS_TO_PROGRAMS_RUNTIME_DIR
    "${CMAKE_INSTALL_PREFIX}/${BINSALAT_INSTALL_DIR}/otpimizer/points_to_programs"
  )
else()
  set(POINTS_TO_PROGRAMS_RUNTIME_DIR
    "${POINTS_TO_PROGRAMS_SRC_DIR}"
  )
endif()

target_compile_definitions(${THIS_TARGET_NAME}
  PRIVATE
    POINTS_TO_ANALYSIS_PROGRAMS_DIR="${POINTS_TO_PROGRAMS_RUNTIME_DIR}"
)

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

  install(DIRECTORY "${POINTS_TO_PROGRAMS_SRC_DIR}/"
    DESTINATION "${BINSALAT_INSTALL_DIR}/optimizer/points_to_programs"
  )
endif()

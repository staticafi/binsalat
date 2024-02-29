set(THIS_TARGET_NAME test_builder_for_input_flow)

file(GLOB "${THIS_TARGET_NAME}_H" "./*.h")
file(GLOB "${THIS_TARGET_NAME}_C" "./*.c")

add_executable(${THIS_TARGET_NAME}
    "${${THIS_TARGET_NAME}_H}"
    "${${THIS_TARGET_NAME}_C}"
    )

target_link_libraries(${THIS_TARGET_NAME}
    )

if(DEFINED BINSALAT_INSTALL_DIR)
    install(TARGETS ${THIS_TARGET_NAME} DESTINATION "${BINSALAT_INSTALL_DIR}")
endif(DEFINED BINSALAT_INSTALL_DIR)


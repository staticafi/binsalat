add_library(optimizer_tests_common INTERFACE)

message("Searching for Catch2 library ...")
find_package(Catch2 3 CONFIG REQUIRED)
message(STATUS "Catch2 version: ${Catch2_VERSION}")

include(Catch)
include(CTest)

target_include_directories(optimizer_tests_common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../salac/optimizer/include
)

target_link_libraries(optimizer_tests_common INTERFACE
    ${BOOST_LIST_OF_LIBRARIES_TO_LINK_WITH}
    optimizer
    sala
    utility
)

# Install Catch2 runtime artifacts
install(IMPORTED_RUNTIME_ARTIFACTS
    Catch2::Catch2
    Catch2::Catch2WithMain
    DESTINATION .
)

if (VCPKG_INSTALLED_DIR)
    if (WIN32)
        set(VCPKG_RUNTIME_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<IF:$<CONFIG:Debug>,debug/bin,bin>")

        set(CATCH2_RUNTIME_DEPENDENCIES
            "$<IF:$<CONFIG:Debug>,Catch2d.dll,Catch2.dll>"
            "$<IF:$<CONFIG:Debug>,Catch2Maind.dll,Catch2Main.dll>"
        )
    elseif (UNIX)
        set(VCPKG_RUNTIME_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<IF:$<CONFIG:Debug>,debug/lib,lib>")

        set(CATCH2_RUNTIME_DEPENDENCIES
            "$<IF:$<CONFIG:Debug>,libCatch2d.so,libCatch2.so>"
            "$<IF:$<CONFIG:Debug>,libCatch2Maind.so,libCatch2Main.so>"
        )
    endif ()

    foreach (RUNTIME_FILE ${CATCH2_RUNTIME_DEPENDENCIES})
        install(FILES "${VCPKG_RUNTIME_DIR}/${RUNTIME_FILE}"
            DESTINATION .
            OPTIONAL
        )
    endforeach ()
endif ()

enable_testing()

add_subdirectory(translation)
add_subdirectory(utils)
add_subdirectory(programIR)
add_subdirectory(analysis)

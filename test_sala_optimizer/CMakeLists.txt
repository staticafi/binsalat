add_library(optimizer_tests_common INTERFACE)

message(STATUS "Searching for Catch2 library ...")
find_package(Catch2 3 CONFIG REQUIRED)
message(STATUS "Catch2 version: ${Catch2_VERSION}")

include(Catch)

target_include_directories(optimizer_tests_common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../salac/optimizer/include
)
target_link_libraries(optimizer_tests_common INTERFACE
    ${BOOST_LIST_OF_LIBRARIES_TO_LINK_WITH}
    optimizer
    sala
    utility
)

enable_testing()

add_subdirectory(translation)
add_subdirectory(utils)
add_subdirectory(programIR)
add_subdirectory(analysis)

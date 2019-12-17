# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our project. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(GTEST_VENDOR google)

foreach(GLIB gmock gmock_main gtest gtest_main)
    set_target_properties(${GLIB} PROPERTIES
                          FOLDER ${THIRD_PARTY_FOLDER}/${GTEST_VENDOR})
endforeach()


file(GLOB TEST_FILES
    NAMES "*.cpp"
)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE}
                           NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE ${Lib_INCLUDE_DIRS})
    target_link_libraries(${TEST_NAME} PRIVATE gtest_main ${LibName})
    set_target_properties(${TEST_NAME} PROPERTIES
                          FOLDER tests)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

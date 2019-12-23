file(GLOB_RECURSE Lib_SRC_FILES
    NAME "*.h" "*.hpp" "*.c" "*.cpp")

add_library(${Lib} ${Lib_SRC_FILES})
target_include_directories(${Lib} PUBLIC ${Lib_INCLUDE_DIRS})

if(NOT WIN32)
    find_package(pthread)
    target_link_libraries(${Lib} PUBLIC pthread)
endif()
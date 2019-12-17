add_executable(${App} main.cpp)
target_include_directories(${App} PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(${App} PRIVATE ${Lib})

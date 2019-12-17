cmake_minimum_required (VERSION 3.8)

set(ExerciseNumber template) # change it to current exercise number
set(ProjectName oop_exercise_${ExerciseNumber}) # project name that follows `oop_exercise_#` form
project(${ProjectName})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE ON)

# Default project configuration
# Try to DO NOT change an any file from listed down
include(config)
include(lib)
include(app)
include(tests)

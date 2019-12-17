# Verbose log configuration
include(CMakeParseArguments)
set(VERBOSE OFF CACHE STRING "Verbose log mode")
# verbose_log definition
# This function depends on VERBOSE variable:
#   VERBOSE is ON: log enabled
#   VERBOSE is OFF: log disabled
function(verbose_log)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        ""          # list of names of the boolean arguments (only defined ones will be true)
        ""          # list of names of mono-valued arguments
        "MESSAGE"   # list of names of multi-valued arguments (output variables are lists)
        ${ARGN}     # arguments of the function to parse, here we take the all original ones
    )
    if(NOT PARSED_ARGS_MESSAGE)
        message(FATAL_ERROR "invalid verbose_log arguments")
    endif()
    if(VERBOSE)
        message(STATUS ${PARSED_ARGS_MESSAGE})
    endif()
endfunction(verbose_log)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ language standard")
set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SRC_DIRS     ${PROJECT_SOURCE_DIR}/src)
verbose_log(MESSAGE "Project include dirs: " ${PROJECT_INCLUDE_DIRS})
verbose_log(MESSAGE "Project source dirs: " ${PROJECT_SRC_DIRS})

# Main targets names
set(App ${ProjectName})
set(Lib lib${ProjectName})
verbose_log(MESSAGE "Application name: " ${App})
verbose_log(MESSAGE "Main library name: " ${Lib})

# Third pary environment
set(THIRD_PARTY_FOLDER third_party CACHE STRING "Third party folder")

cmake_minimum_required(VERSION 3.16)

project(SimpleEventSystem VERSION 1.0.0 LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a build directory.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# optional targets
option(SES_ENABLE_DOXYGEN "Create doxygen documentation target" OFF)
option(SES_ENABLE_BUILD_TESTS "Build the test executables" OFF)
option(SES_ENABLE_BUILD_EXAMPLES "Build examples" OFF)
option(SES_ENABLE_CALLGRIND_TARGETS "Create additional targets for callgrind/kcachegrind for executables" OFF) #
option(SES_ENABLE_STATIC_ANALYSIS "Create additional targets for explicit static analysis of targets" OFF) #

# other build options
option(SES_ENABLE_COVERAGE_REPORT "Create code coverage report for tests" OFF)
option(SES_ENABLE_BRUTAL_COMPILE_OPTIONS "Add additional warning flags when building" OFF) #
option(SES_ENABLE_SANITIZERS "Enable sanitizers during compilation" OFF) #

if(SES_ENABLE_COVERAGE_REPORT AND NOT SES_ENABLE_BUILD_TESTS)
    message(FATAL_ERROR "Code coverage only available when building tests")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(SES_ENABLE_BRUTAL_COMPILE_OPTIONS)
        include(brutal-compiler-options)

        set(SES_WARNING_SUPPRESSORS)

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            list(APPEND SES_WARNING_SUPPRESSORS 
                -Wno-unsafe-loop-optimizations
                -Wno-effc++
            )
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            list(APPEND SES_WARNING_SUPPRESSORS 
                -Wno-padded
                -Wno-date-time
                -Wno-c++98-compat
                -Wno-exit-time-destructors
            )
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            list(APPEND SES_WARNING_SUPPRESSORS 
                /wd4820
                /wd4514
                /wd4711
                /wd5045
                /wd4996
                /wd4668
            )
        endif()
    endif()

    if(SES_ENABLE_SANITIZERS)
        include(sanitize)
    endif()

    if(SES_ENABLE_CALLGRIND_TARGETS)
        include(grind)
    endif()

    if(SES_ENABLE_STATIC_ANALYSIS)
        include(static-analysis)
    endif()
endif()

if(SES_ENABLE_DOXYGEN)
    # To generate html documentation, build the 'docs' target
    include(doxy)
    cmake_path(APPEND SES_DOXYFILE_PATH ${CMAKE_SOURCE_DIR} docs)
    scm_create_docs_target(${SES_DOXYFILE_PATH})
endif()

if(SES_ENABLE_COVERAGE_REPORT)
    # To generate html coverage report, build the 'ccov-all' target
    message(STATUS "Adding code coverage")
    set(CODE_COVERAGE ON)
    include(code-coverage)
    add_code_coverage_all_targets()
endif()

add_subdirectory(source)

if(SES_ENABLE_BUILD_TESTS)
    # run tests with ctest
    message(STATUS "Building tests")
    include(CTest)
    add_subdirectory(tests)
endif()

if(SES_ENABLE_BUILD_EXAMPLES)
    # you can find example executables in build/examples/... (depending on the cmake generator)
    message(STATUS "Building examples")
    add_subdirectory(examples)
endif()

set(SES_LIBRARY_NAME simpleEventSystem)

set(SES_SIMPLE_EVENT_SYSTEM_SOURCES
    "event.cpp"
    "eventGenerator.cpp"
    "eventListener.cpp"
    "eventLoop.cpp"
)

set(SES_SIMPLE_EVENT_SYSTEM_HEADERS
    "event.hpp"
    "eventDebug.hpp"
    "eventGenerator.hpp"
    "eventListener.hpp"
    "eventLoop.hpp"
)

set(SES_SIMPLE_EVENT_SYSTEM_INCLUDES ${CMAKE_SOURCE_DIR}/include)

add_library(${SES_LIBRARY_NAME} STATIC ${SES_SIMPLE_EVENT_SYSTEM_SOURCES})
target_include_directories(${SES_LIBRARY_NAME} PUBLIC ${SES_SIMPLE_EVENT_SYSTEM_INCLUDES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(SES_ENABLE_BRUTAL_COMPILE_OPTIONS)
        scm_add_brutal_compiler_options(${SES_LIBRARY_NAME} PUBLIC ${SES_WARNING_SUPPRESSORS})
    endif()

    if(SES_ENABLE_SANITIZERS)
        # scm_add_address_sanitizer_with_options(${SES_LIBRARY_NAME} PUBLIC)
        # scm_add_undefined_behavior_sanitizer_with_options(${SES_LIBRARY_NAME} PUBLIC)
        # scm_add_thread_sanitizer_with_options(${SES_LIBRARY_NAME} PUBLIC)
        # scm_add_memory_sanitizer_with_options(${SES_LIBRARY_NAME} PUBLIC)
    endif()

    if(SES_ENABLE_CALLGRIND_TARGETS)
        scm_create_grind_target(${SES_LIBRARY_NAME})
    endif()

    if(SES_ENABLE_STATIC_ANALYSIS)
        # this target_include_directories is only needed for the ${SC-TEST_NAME}-static-analyze target
        # because at this time it cannot extract include directories from the linked simpleContainers target
        # remove this line once that is working!
        target_include_directories(${SES_LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

        scm_add_static_analysis_target(${SES_LIBRARY_NAME})
    endif()
endif()

if(SES_ENABLE_COVERAGE_REPORT)
    target_code_coverage(${SES_LIBRARY_NAME} AUTO ALL)
endif()

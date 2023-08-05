set(SIMPLE_EVENT_SYSTEM_SOURCES "event.cpp"
                 "eventGenerator.cpp"
                 "eventListener.cpp"
                 "eventLoop.cpp"
)

set(SIMPLE_EVENT_SYSTEM_HEADERS "event.hpp"
                 "eventDebug.hpp"
                 "eventGenerator.hpp"
                 "eventListener.hpp"
                 "eventLoop.hpp"
)

set(SIMPLE_EVENT_SYSTEM_INCLUDES ${CMAKE_SOURCE_DIR}/include)

add_library(simpleEventSystem STATIC ${SIMPLE_EVENT_SYSTEM_SOURCES})
target_include_directories(simpleEventSystem PUBLIC ${SIMPLE_EVENT_SYSTEM_INCLUDES})

if(ENABLE_COVERAGE_REPORT)  
    target_code_coverage(simpleEventSystem AUTO ALL)
endif()
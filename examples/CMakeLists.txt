if(ENABLE_BUILD_EXAMPLES)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    set(TEST_SOURCES "example_01.cpp")

    foreach(SOURCE IN LISTS TEST_SOURCES)
        get_filename_component(EXAMPLE_NAME ${SOURCE} NAME_WLE)
        message(STATUS "Creating example: " ${EXAMPLE_NAME} " from source: " ${SOURCE})
        
        add_executable(${EXAMPLE_NAME} ${SOURCE})
        target_link_libraries(${EXAMPLE_NAME} PUBLIC 
            simpleEventSystem
            Threads::Threads
        )
    endforeach()
endif()

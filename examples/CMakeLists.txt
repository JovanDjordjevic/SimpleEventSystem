if(SES_ENABLE_BUILD_EXAMPLES)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    set(SES_EXAMPLE_SOURCES "example_01.cpp")

    foreach(SES_SOURCE IN LISTS SES_EXAMPLE_SOURCES)
        get_filename_component(SES_EXAMPLE_NAME ${SES_SOURCE} NAME_WLE)
        message(STATUS "Creating example: ${SES_EXAMPLE_NAME} from source: ${SES_SOURCE}")
        
        add_executable(${SES_EXAMPLE_NAME} ${SES_SOURCE})
        target_link_libraries(${SES_EXAMPLE_NAME} PUBLIC 
            simpleEventSystem
            Threads::Threads
        )
    endforeach()
endif()

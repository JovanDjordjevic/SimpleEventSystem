if(SES_ENABLE_BUILD_TESTS)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)

    set(SES_TEST_SOURCES "test_registration.cpp"
                         "test_posting.cpp"
    )

    foreach(SES_SOURCE IN LISTS SES_TEST_SOURCES)
        get_filename_component(SES_TEST_NAME ${SES_SOURCE} NAME_WLE)
        message(STATUS "Creating test: ${SES_TEST_NAME} from source: ${SES_SOURCE}")

        add_executable(${SES_TEST_NAME} ${SES_SOURCE})
        target_link_libraries(${SES_TEST_NAME} PUBLIC 
            simpleEventSystem
            Threads::Threads
        )

        add_test(NAME ${SES_TEST_NAME} COMMAND ${SES_TEST_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

        if(SES_ENABLE_COVERAGE_REPORT)
            target_code_coverage(${SES_TEST_NAME} AUTO ALL)
        endif()
    endforeach()
endif()

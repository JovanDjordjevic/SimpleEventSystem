set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(TEST_SOURCES "main.cpp")

add_executable(eventLoopTest ${TEST_SOURCES})
target_link_libraries(eventLoopTest PUBLIC
    simpleEventSystem
    Threads::Threads
)

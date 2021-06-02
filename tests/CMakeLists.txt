find_package(CURL REQUIRED)

function(qservice_test test_name)
    set(TEST_EXECUTABLE run_${test_name})
    set(TEST_SOURCE ${test_name}.cpp)

    add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE})
    target_include_directories(${TEST_EXECUTABLE} PRIVATE ${CURL_INCLUDE_DIRS})
    # CMake 3.20 and upstream GTest define GTest::gtest, older versions define GTest::GTest
    if (TARGET GTest::gtest)
        target_link_libraries(${TEST_EXECUTABLE} GTest::gtest GTest::gtest_main qservice_static ${CURL_LIBRARIES})
    else ()
        target_link_libraries(${TEST_EXECUTABLE} GTest::GTest GTest::Main qservice_static ${CURL_LIBRARIES})
    endif ()
    add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()


qservice_test(first_test)
qservice_test(qservice_test)
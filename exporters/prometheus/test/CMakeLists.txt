foreach(testname prometheus_exporter_utils_test)
  add_executable(${testname} "${testname}.cc")
  target_link_libraries(
    ${testname} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}
    prometheus_exporter prometheus-cpp::pull)
  gtest_add_tests(TARGET ${testname} TEST_PREFIX exporter. TEST_LIST
                  ${testname})
endforeach()

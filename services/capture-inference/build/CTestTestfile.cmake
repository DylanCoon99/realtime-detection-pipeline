# CMake generated Testfile for 
# Source directory: /Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference
# Build directory: /Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[capture-inference_tests]=] "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/build/bin/Debug/capture-inference_tests")
  set_tests_properties([=[capture-inference_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;94;add_test;/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[capture-inference_tests]=] "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/build/bin/Release/capture-inference_tests")
  set_tests_properties([=[capture-inference_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;94;add_test;/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[capture-inference_tests]=] "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/build/bin/MinSizeRel/capture-inference_tests")
  set_tests_properties([=[capture-inference_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;94;add_test;/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[capture-inference_tests]=] "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/build/bin/RelWithDebInfo/capture-inference_tests")
  set_tests_properties([=[capture-inference_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;94;add_test;/Users/Dylan/Documents/realtime-detection-pipeline/services/capture-inference/CMakeLists.txt;0;")
else()
  add_test([=[capture-inference_tests]=] NOT_AVAILABLE)
endif()

# CMake generated Testfile for 
# Source directory: /home/oleksandr/workspace/C_C++/Unix_server
# Build directory: /home/oleksandr/workspace/C_C++/Unix_server/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit_tests "/home/oleksandr/workspace/C_C++/Unix_server/build/unit_tests")
set_tests_properties(unit_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/oleksandr/workspace/C_C++/Unix_server/CMakeLists.txt;21;add_test;/home/oleksandr/workspace/C_C++/Unix_server/CMakeLists.txt;0;")
subdirs("_deps/cmocka-build")

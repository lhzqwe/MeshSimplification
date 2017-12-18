# CMake generated Testfile for 
# Source directory: D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5
# Build directory: D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(demangle "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Debug/demangle_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(demangle "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Release/demangle_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(demangle "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/MinSizeRel/demangle_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(demangle "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/RelWithDebInfo/demangle_unittest.exe")
else()
  add_test(demangle NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Debug/logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Release/logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/MinSizeRel/logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/RelWithDebInfo/logging_unittest.exe")
else()
  add_test(logging NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(stl_logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Debug/stl_logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(stl_logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/Release/stl_logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(stl_logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/MinSizeRel/stl_logging_unittest.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(stl_logging "D:/MeshSimplification/GLProjectInRibbon/GLProjectInRibbon/glog/glog-0.3.5/build/RelWithDebInfo/stl_logging_unittest.exe")
else()
  add_test(stl_logging NOT_AVAILABLE)
endif()

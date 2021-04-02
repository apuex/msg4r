CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(msg4r)

EXECUTE_PROCESS( COMMAND uname -s COMMAND tr -d '\n' OUTPUT_VARIABLE OSNAME )
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE OSARCH )

SET(CMAKE_CXX_STANDARD 17)

INCLUDE(CTest)
ENABLE_TESTING()

if(MSVC)
  SET(CMAKE_CXX_FLAGS "/MD /EHsc")
  ADD_DEFINITIONS(-D_WIN32_WINNT=_WIN32_WINNT_WS08)
  ADD_DEFINITIONS(-D_AFXDLL)
  ADD_DEFINITIONS(-D_MBCS)
  ADD_DEFINITIONS(-D_WINDLL)
endif()

ADD_DEFINITIONS(-DMSG4R_BIG_ENDIAN)
ADD_DEFINITIONS(-DMSG4R_SIZE_T=uint8_t)

INCLUDE_DIRECTORIES(
  app
  include
  ${CMAKE_INSTALL_PREFIX}/include
  ${INCLUDE_DIRECTORIES}
)

LINK_DIRECTORIES(
  lib
  ${CMAKE_INSTALL_PREFIX}/lib
  ${LINK_DIRECTORIES} 
  ${LIBRARY_OUTPUT_PATH}
)

INCLUDE_DIRECTORIES(
  include
)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0065 NEW)

find_package(Threads REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS date_time json serialization program_options log unit_test_framework)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()

SET(MSG4R_SRCS
  include/msg4r.h
  src/msg4r.cpp
)

SET(MSG4R_COMPILER_TEST_SRCS
  test/msg4r-compiler-test.cpp
)

SET(MSG4R_ISTREAM_TEST_SRCS
  test/msg4r-istream-test.cpp
)

SET(MSG4R_JSON_TEST_SRCS
  test/msg4r-json-test.cpp
)

SET(MSG4R_BINARY_TEST_SRCS
  test/msg4r-binary-test.cpp
)

SET(MSG4R_ENDIANNESS_TEST_SRCS
  test/msg4r-endianness-test.cpp
)

SET(MSG4R_INT_TEST_SRCS
  test/msg4r-int-test.cpp
)

SET(MSG4R_LIST_TEST_SRCS
  test/msg4r-list-test.cpp
)

SET(MSG4R_MAP_TEST_SRCS
  test/msg4r-map-test.cpp
)

SET(MSG4R_SET_TEST_SRCS
  test/msg4r-set-test.cpp
)

SET(MSG4R_STRING_TEST_SRCS
  test/msg4r-string-test.cpp
)

SET(MSG4R_STRUCT1_TEST_SRCS
  test/msg4r-struct1-test.cpp
)

SET(MSG4R_STRUCT2_TEST_SRCS
  test/msg4r-struct2-test.cpp
)

SET(MSG4R_TRUMP_TEST_SRCS
  test/msg4r-trump-test.cpp
)

SET(MSG4R_VECTOR_TEST_SRCS
  test/msg4r-vector-test.cpp
)

SET(MSG4R_FAIL_TEST_SRCS
  test/msg4r-fail-test.cpp
)

ADD_LIBRARY(msg4r STATIC ${MSG4R_SRCS})
target_compile_definitions(msg4r PUBLIC _MSG4R_)

ADD_EXECUTABLE(msg4r-compiler-test ${MSG4R_COMPILER_TEST_SRCS})
if(UNIX)
set_target_properties(msg4r-compiler-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

ADD_EXECUTABLE(msg4r-istream-test ${MSG4R_ISTREAM_TEST_SRCS})
target_include_directories(msg4r-istream-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-istream-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-istream-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

ADD_EXECUTABLE(msg4r-json-test ${MSG4R_JSON_TEST_SRCS})
target_include_directories(msg4r-json-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-json-test ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-json-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

ADD_EXECUTABLE(msg4r-binary-test ${MSG4R_BINARY_TEST_SRCS})
#target_compile_definitions(msg4r-binary-test PUBLIC BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
target_include_directories(msg4r-binary-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-binary-test ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-binary-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

ADD_EXECUTABLE(msg4r-endianness-test ${MSG4R_ENDIANNESS_TEST_SRCS})
target_include_directories(msg4r-endianness-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-endianness-test ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-endianness-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

ADD_EXECUTABLE(msg4r-int-test ${MSG4R_INT_TEST_SRCS})
target_include_directories(msg4r-int-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-int-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-int-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-int-test COMMAND msg4r-int-test)

ADD_EXECUTABLE(msg4r-list-test ${MSG4R_LIST_TEST_SRCS})
target_include_directories(msg4r-list-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-list-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-list-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-list-test COMMAND msg4r-list-test)

ADD_EXECUTABLE(msg4r-map-test ${MSG4R_MAP_TEST_SRCS})
target_include_directories(msg4r-map-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-map-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-map-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-map-test COMMAND msg4r-map-test)

ADD_EXECUTABLE(msg4r-set-test ${MSG4R_SET_TEST_SRCS})
target_include_directories(msg4r-set-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-set-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-set-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-set-test COMMAND msg4r-set-test)

ADD_EXECUTABLE(msg4r-string-test ${MSG4R_STRING_TEST_SRCS})
target_include_directories(msg4r-string-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-string-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-string-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-string-test COMMAND msg4r-string-test)

ADD_EXECUTABLE(msg4r-struct1-test ${MSG4R_STRUCT1_TEST_SRCS})
target_include_directories(msg4r-struct1-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-struct1-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-struct1-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-struct1-test COMMAND msg4r-struct1-test)

ADD_EXECUTABLE(msg4r-struct2-test ${MSG4R_STRUCT2_TEST_SRCS})
target_include_directories(msg4r-struct2-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-struct2-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-struct2-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-struct2-test COMMAND msg4r-struct2-test)

ADD_EXECUTABLE(msg4r-trump-test ${MSG4R_TRUMP_TEST_SRCS})
target_include_directories(msg4r-trump-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-trump-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-trump-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-trump-test COMMAND msg4r-trump-test)

ADD_EXECUTABLE(msg4r-vector-test ${MSG4R_VECTOR_TEST_SRCS})
target_include_directories(msg4r-vector-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-vector-test msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
if(UNIX)
set_target_properties(msg4r-vector-test PROPERTIES LINK_FLAGS_RELEASE -s)
endif()
ADD_TEST(NAME msg4r-vector-test COMMAND msg4r-vector-test)

ADD_TEST(NAME msg4r-compiler-test COMMAND msg4r-compiler-test)
ADD_TEST(NAME msg4r-istream-test COMMAND msg4r-istream-test)
ADD_TEST(NAME msg4r-json-test COMMAND msg4r-json-test)
ADD_TEST(NAME msg4r-binary-test COMMAND msg4r-binary-test)
ADD_TEST(NAME msg4r-endianness-test COMMAND msg4r-endianness-test)

set_target_properties(msg4r PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

INSTALL(TARGETS
  msg4r
  CONFIGURATIONS Release
  EXPORT msg4r
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

INSTALL(TARGETS
  msg4r
  CONFIGURATIONS Debug
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

INSTALL(FILES 
  include/msg4r.h
  DESTINATION include
  CONFIGURATIONS Release)
    

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
find_package(Boost 1.71.0 REQUIRED COMPONENTS date_time json program_options log unit_test_framework)
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

SET(MSG4R_JSON_TEST_SRCS
  test/msg4r-json-test.cpp
)

SET(MSG4R_FAIL_TEST_SRCS
  test/msg4r-fail-test.cpp
)

ADD_LIBRARY(msg4r SHARED ${MSG4R_SRCS})
target_compile_definitions(msg4r PUBLIC _MSG4R_)

ADD_EXECUTABLE(msg4r-compiler-test ${MSG4R_COMPILER_TEST_SRCS})
ADD_EXECUTABLE(msg4r-json-test ${MSG4R_JSON_TEST_SRCS})
target_include_directories(msg4r-json-test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(msg4r-json-test ${Boost_unit_test_framework})

ADD_TEST(NAME msg4r-compiler-test COMMAND msg4r-compiler-test)
ADD_TEST(NAME msg4r-json-test COMMAND msg4r-json-test)

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
    

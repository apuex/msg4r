CMAKE_MINIMUM_REQUIRED(VERSION 3.10.3)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0065 NEW)
cmake_policy(SET CMP0091 NEW) # Note: This property has effect only when policy CMP0091 is set to NEW prior to the first project() or enable_language() command that enables a language using a compiler targeting the MSVC ABI. 

PROJECT(msg4r)

EXECUTE_PROCESS( COMMAND uname -s COMMAND tr -d '\n' OUTPUT_VARIABLE OSNAME )
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE OSARCH )

SET(CMAKE_CXX_STANDARD 11)

option(ENABLE_TESTS "Enable building tests" ON)
option(ENABLE_VALGRIND "Enable the valgrind memory checker for the tests." OFF)

if(MSVC)
  SET(CMAKE_CXX_FLAGS "/MD /EHsc")
  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0502)
  ADD_DEFINITIONS(-D_AFXDLL)
  ADD_DEFINITIONS(-D_MBCS)
  ADD_DEFINITIONS(-D_WINDLL)
  ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
endif()

ADD_DEFINITIONS(-DMSG4R_BIG_ENDIAN)
ADD_DEFINITIONS(-DMSG4R_SIZE_T=uint16_t)

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

find_package(Threads REQUIRED)
find_package(Boost 1.75.0 REQUIRED COMPONENTS date_time json serialization program_options log unit_test_framework)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()

SET(MSG4R_SRCS
  include/msg4r.hpp
  src/msg4r.cpp
)

ADD_LIBRARY(msg4r STATIC ${MSG4R_SRCS})
target_compile_definitions(msg4r PUBLIC _MSG4R_)
set_target_properties(msg4r PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

if(ENABLE_TESTS)
  include(CTest)
  enable_testing()

  set(unit_tests
    msg4r-binary-test
    msg4r-compiler-test
    msg4r-endianness-test
    msg4r-fail-test
    msg4r-inheritance-test
    msg4r-int-test
    msg4r-istream-test
    msg4r-json-test
    msg4r-list-test
    msg4r-map-test
    msg4r-number-parser-test
    msg4r-pair-test
    msg4r-poc-int-test
    msg4r-set-test
    msg4r-shared-ptr-test
    msg4r-state-test
    msg4r-streambuf-test
    msg4r-string-test
    msg4r-struct1-test
    msg4r-struct2-test
    msg4r-template-test
    msg4r-trump-test
    msg4r-vector-test
  )

  if(ENABLE_VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind)
    if("${MEMORYCHECK_COMMAND}" MATCHES "MEMORYCHECK_COMMAND-NOTFOUND")
      message(WARNING "Valgrind couldn't be found.")
      unset(MEMORYCHECK_COMMAND)
    else()
      set(MEMORYCHECK_COMMAND_OPTIONS
        --trace-children=yes
       	--leak-check=full
       	--error-exitcode=1
       	--suppressions=${CMAKE_CURRENT_SOURCE_DIR}/valgrind.supp
      )
    endif()
  endif(ENABLE_VALGRIND)

foreach(unit_test ${unit_tests})
  add_executable("${unit_test}" "tests/${unit_test}.cpp")
  target_include_directories(${unit_test} PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(${unit_test} msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
  if(UNIX)
    set_target_properties("${unit_test}" PROPERTIES LINK_FLAGS_RELEASE -s)
  else(UNIX)
  endif(UNIX)
  
  if(ENABLE_VALGRIND)
    if(MEMORYCHECK_COMMAND)
      add_test(NAME "${unit_test}"
        COMMAND "${MEMORYCHECK_COMMAND}"
       	        "${MEMORYCHECK_COMMAND_OPTIONS}"
                "${CMAKE_CURRENT_BINARY_DIR}/${unit_test}"
      )
    else(MEMORYCHECK_COMMAND)
      add_test(NAME "${unit_test}"
        COMMAND "./${unit_test}"
      )
    endif(MEMORYCHECK_COMMAND)
  else(ENABLE_VALGRIND)
    add_test(NAME "${unit_test}"
      COMMAND "./${unit_test}"
    )
  endif(ENABLE_VALGRIND)
endforeach()

endif(ENABLE_TESTS)

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
  include/msg4r.hpp
  DESTINATION include
  CONFIGURATIONS Release)
    

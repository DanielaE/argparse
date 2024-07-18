cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_VERSION VERSION_LESS 3.31)
  if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.30)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
    cmake_policy(VERSION 3.28)
  elseif(CMAKE_VERSION VERSION_LESS 3.28)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.27)
      set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
    else ()
      set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
    endif()
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
  else()
    cmake_policy(VERSION 3.28)
  endif()
else()
  cmake_policy(VERSION 3.31)
endif()

project(argparse_tests)

if (ARGPARSE_USE_STD_MODULE)
  set(CMAKE_CXX_MODULE_STD 1)
endif()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wpedantic -Wsign-conversion -Wshadow -Wconversion -Werror -Wextra")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Disable deprecation for windows
if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(ARGPARSE_BUILD_MODULE)
  add_library(argparse_test STATIC)

  target_sources(argparse_test
    PUBLIC
      FILE_SET modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/../module
        FILES ${CMAKE_CURRENT_LIST_DIR}/../module/argparse.ixx
  )

  if (ARGPARSE_USE_STD_MODULE)
    target_compile_features(argparse_test PUBLIC cxx_std_23)
    target_compile_definitions(argparse_test PRIVATE ARGPARSE_MODULE_USE_STD_MODULE)
  else()
    target_compile_features(argparse_test PUBLIC cxx_std_20)
  endif()

  target_compile_definitions(argparse_test PRIVATE ARGPARSE_ATTACH_TO_GLOBAL_MODULE)

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (ARGPARSE_USE_STD_MODULE)
      target_compile_options(argparse_test PRIVATE -Wno-deprecated-declarations)
    endif()
  elseif (MSVC)
    # I do mean C++20
    target_compile_options(argparse_test PRIVATE /dxifcInlineFunctions- /Zc:__cplusplus /Zc:throwingNew /Zc:inline /Zc:externConstexpr /Zc:templateScope /Zc:checkGwOdr /Zc:enumTypes)    
  endif()

  add_library(argparse_module_test OBJECT test-module.cpp)
  target_link_libraries(argparse_module_test PUBLIC argparse_test)
endif()

# ARGPARSE executable
file(GLOB ARGPARSE_TEST_SOURCES
    main.cpp
    test_actions.cpp
    test_append.cpp
    test_as_container.cpp
    test_bool_operator.cpp
    test_choices.cpp
    test_compound_arguments.cpp
    test_container_arguments.cpp
    test_default_args.cpp
    test_default_value.cpp
    test_error_reporting.cpp
    test_get.cpp
    test_help.cpp
    test_hidden_alias.cpp
    test_hidden_argument.cpp
    test_invalid_arguments.cpp
    test_is_used.cpp
    test_issue_37.cpp
    test_mutually_exclusive_group.cpp
    test_negative_numbers.cpp
    test_optional_arguments.cpp
    test_parent_parsers.cpp
    test_parse_args.cpp
    test_positional_arguments.cpp
    test_repr.cpp
    test_required_arguments.cpp
    test_scan.cpp
    test_store_into.cpp
    test_stringstream.cpp
    test_version.cpp
    test_subparsers.cpp
    test_parse_known_args.cpp
    test_equals_form.cpp
    test_prefix_chars.cpp
)
set_source_files_properties(main.cpp
    PROPERTIES
    COMPILE_DEFINITIONS DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
ADD_EXECUTABLE(ARGPARSE_TESTS ${ARGPARSE_TEST_SOURCES})
INCLUDE_DIRECTORIES("../include" ".")
set_target_properties(ARGPARSE_TESTS PROPERTIES OUTPUT_NAME tests)

if(ARGPARSE_BUILD_MODULE)
  target_link_libraries(ARGPARSE_TESTS PRIVATE argparse_module_test)
endif()

if (ARGPARSE_USE_STD_MODULE)
  target_compile_definitions(ARGPARSE_TESTS PRIVATE ARGPARSE_MODULE_USE_STD_MODULE)
  target_compile_features(ARGPARSE_TESTS PRIVATE cxx_std_23)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ARGPARSE_TESTS PRIVATE -Wno-deprecated-declarations)
  endif()
else()
  target_compile_features(ARGPARSE_TESTS PRIVATE cxx_std_17)
endif()

# Set ${PROJECT_NAME} as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ARGPARSE_TESTS)

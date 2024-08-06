cmake_minimum_required(VERSION 3.12.4)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED PROJECT_NAME)
  set(ARGPARSE_IS_TOP_LEVEL ON)
else()
  set(ARGPARSE_IS_TOP_LEVEL OFF)
endif()

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

project(argparse
        VERSION 3.1.0 
        DESCRIPTION "A single header argument parser for C++17"
        HOMEPAGE_URL "https://github.com/p-ranav/argparse"
        LANGUAGES CXX
)

option(ARGPARSE_INSTALL "Include an install target" ${ARGPARSE_IS_TOP_LEVEL})
option(ARGPARSE_BUILD_TESTS "Build tests" ${ARGPARSE_IS_TOP_LEVEL})
option(ARGPARSE_BUILD_SAMPLES "Build samples" OFF)
option(ARGPARSE_BUILD_MODULE "Build the module interface" OFF)
option(ARGPARSE_MIXED_MODULE "Enable mixing module and header" OFF)
option(ARGPARSE_USE_STD_MODULE "Use the modularized standard library" OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (ARGPARSE_BUILD_MODULE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("Warning: the support for modules in gcc is not good enough!\n")
  endif()
  cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
endif()

if (ARGPARSE_USE_STD_MODULE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(ARGPARSE_USE_STD_MODULE OFF)
    message("Warning: no support for a modularized standard library!\nOption ARGPARSE_USE_STD_MODULE is switched off\n")
  else()
    cmake_minimum_required(VERSION 3.30 FATAL_ERROR)
  endif()
endif()

if (ARGPARSE_BUILD_MODULE)
  add_library(argparse STATIC)

  target_sources(argparse
    PUBLIC
      FILE_SET modules TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/module
        FILES ${CMAKE_CURRENT_LIST_DIR}/module/argparse.ixx
  )
  target_compile_definitions(argparse PRIVATE ARGPARSE_AUTOLINK)
  
  if(ARGPARSE_MIXED_MODULE)
    target_compile_definitions(argparse PRIVATE ARGPARSE_ATTACH_TO_GLOBAL_MODULE)
  endif()

  if (ARGPARSE_USE_STD_MODULE)
    target_compile_features(argparse
      PRIVATE   cxx_std_23
      INTERFACE cxx_std_20)
    target_compile_definitions(argparse PRIVATE ARGPARSE_MODULE_USE_STD_MODULE)
    set_target_properties(argparse PROPERTIES CXX_MODULE_STD 1)
  else()
    target_compile_features(argparse PUBLIC cxx_std_20)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (ARGPARSE_USE_STD_MODULE)
      target_compile_options(argparse PRIVATE -Wno-deprecated-declarations)
    endif()
  elseif (MSVC)
  # I do mean C++20
    target_compile_options(argparse PRIVATE /Zc:__cplusplus /Zc:throwingNew /Zc:inline /Zc:externConstexpr /Zc:templateScope /Zc:checkGwOdr /Zc:enumTypes)
  endif()

  target_include_directories(argparse PRIVATE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

else()
  add_library(argparse INTERFACE)

  if (ARGPARSE_USE_STD_MODULE)
    target_compile_definitions(argparse INTERFACE ARGPARSE_MODULE_USE_STD_MODULE)
  endif()
  target_compile_features(argparse INTERFACE cxx_std_17)
  target_include_directories(argparse INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

endif()
add_library(argparse::argparse ALIAS argparse)

if(ARGPARSE_BUILD_SAMPLES)
  add_subdirectory(samples)
endif()
  
if(ARGPARSE_BUILD_TESTS)
  add_subdirectory(test)
endif()
  
if(ARGPARSE_INSTALL)
  if(ARGPARSE_BUILD_MODULE)
    install(TARGETS argparse EXPORT argparseConfig
            FILE_SET modules DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)
  else()
    install(TARGETS argparse EXPORT argparseConfig)
  endif()
  install(EXPORT argparseConfig
          NAMESPACE argparse::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
  install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/argparse/argparse.hpp
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)


  set(CONFIG_FILE_NAME_WITHOUT_EXT "${PROJECT_NAME}Config")
  set(CMAKE_CONFIG_FILE_BASENAME "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME_WITHOUT_EXT}")
  set(CMAKE_CONFIG_VERSION_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}-version.cmake")
  set(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}.cmake")

  if(${CMAKE_VERSION} VERSION_GREATER "3.14")
  	set(OPTIONAL_ARCH_INDEPENDENT "ARCH_INDEPENDENT")
  endif()

  write_basic_package_version_file("${CMAKE_CONFIG_VERSION_FILE_NAME}"
      COMPATIBILITY SameMajorVersion
      ${OPTIONAL_ARCH_INDEPENDENT}
  )

  export(EXPORT argparseConfig
         NAMESPACE argparse::)

  install(FILES "${CMAKE_CONFIG_VERSION_FILE_NAME}"
         DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

  set(PackagingTemplatesDir "${CMAKE_CURRENT_SOURCE_DIR}/packaging")

  set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
  set(CPACK_PACKAGE_VENDOR "argparse (C++) developers")
  set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
  set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
  set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
  set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
  set(CPACK_PACKAGE_MAINTAINER "Pranav Srinivas Kumar")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

  set(CPACK_DEBIAN_PACKAGE_NAME "lib${PROJECT_NAME}-dev")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6-dev")
  set(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake, pkg-config, pkg-conf")

  set(CPACK_RPM_PACKAGE_NAME "lib${PROJECT_NAME}-devel")
  set(CPACK_RPM_PACKAGE_SUGGESTS "${CPACK_DEBIAN_PACKAGE_SUGGESTS}")

  set(CPACK_DEB_COMPONENT_INSTALL ON)
  set(CPACK_RPM_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)
  set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

  set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")
  configure_file("${PackagingTemplatesDir}/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
  install(FILES "${PKG_CONFIG_FILE_NAME}"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
  )
endif()

include(CPack)

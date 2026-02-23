# Hanzo S3 C++ Library for Amazon S3 Compatible Cloud Storage
# Copyright 2021-2024 Hanzo AI, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0091 NEW)

# Hanzo S3 C++ Project
# -----------------

set(S3_CPP_MAJOR_VERSION "0")
set(S3_CPP_MINOR_VERSION "3")
set(S3_CPP_PATCH_VERSION "0")
set(S3_CPP_VERSION_STRING "${S3_CPP_MAJOR_VERSION}.${S3_CPP_MINOR_VERSION}.${S3_CPP_PATCH_VERSION}")

project(miniocpp
  DESCRIPTION "Hanzo S3 C++ Client SDK provides simple APIs to access S3 compatible object storage"
  VERSION ${S3_CPP_VERSION_STRING}
  LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CheckIncludeFiles)
include(CMakePackageConfigHelpers)

option(S3_CPP_TEST "Build tests" OFF)
option(S3_CPP_MAKE_DOC "Build documentation" OFF)

set(S3_CPP_CFLAGS)
set(S3_CPP_LIBS)
set(S3_CPP_STD "17")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR "x${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "xMSVC")
  # MSVC
else()
  # GCC/Clang/AppleClang/...
  LIST(APPEND S3_CPP_CFLAGS -Wall -Wextra -Wconversion)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0 AND NOT MINGW)
    list(APPEND S3_CPP_LIBS stdc++fs)
  endif()
endif()

# Dependencies
# ------------

find_package(OpenSSL REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(unofficial-inih CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

list(APPEND S3_CPP_LIBS
  unofficial::curlpp::curlpp
  unofficial::inih::inireader
  nlohmann_json::nlohmann_json
  pugixml
  OpenSSL::SSL OpenSSL::Crypto
  ZLIB::ZLIB
)

if (WIN32)
  list(APPEND S3_CPP_LIBS wsock32)
  list(APPEND S3_CPP_LIBS ws2_32)
endif()

# Hanzo S3 C++ Library
# -----------------

set(S3_CPP_SOURCES
  src/args.cc
  src/baseclient.cc
  src/client.cc
  src/credentials.cc
  src/error.cc
  src/http.cc
  src/providers.cc
  src/request.cc
  src/response.cc
  src/select.cc
  src/signer.cc
  src/sse.cc
  src/types.cc
  src/utils.cc
)

set(S3_CPP_HEADERS
  include/miniocpp/args.h
  include/miniocpp/baseclient.h
  include/miniocpp/client.h
  include/miniocpp/config.h
  include/miniocpp/credentials.h
  include/miniocpp/error.h
  include/miniocpp/http.h
  include/miniocpp/providers.h
  include/miniocpp/request.h
  include/miniocpp/response.h
  include/miniocpp/select.h
  include/miniocpp/signer.h
  include/miniocpp/sse.h
  include/miniocpp/types.h
  include/miniocpp/utils.h
)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

IF (BUILD_SHARED_LIBS)
  IF (WIN32)
    message(FATAL_ERROR "Unable to build shared library on Windows yet, this library lacks decorator support.")
  ELSE ()
    add_library(miniocpp SHARED ${S3_CPP_SOURCES} ${S3_CPP_HEADERS})
  ENDIF ()
ELSE ()
  add_library(miniocpp STATIC ${S3_CPP_SOURCES} ${S3_CPP_HEADERS})
ENDIF ()

target_compile_options(miniocpp PRIVATE ${S3_CPP_CFLAGS})
target_compile_features(miniocpp PUBLIC cxx_std_${S3_CPP_STD})
target_include_directories(miniocpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(miniocpp PUBLIC ${S3_CPP_LIBS})
set_target_properties(miniocpp PROPERTIES VERSION "${S3_CPP_VERSION_STRING}")
set_target_properties(miniocpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add a cmake alias - this is how users should use miniocpp in their cmake projects.
add_library(miniocpp::miniocpp ALIAS miniocpp)

# Hanzo S3 C++ Tests
# ---------------

if (S3_CPP_TEST)
  set(EXAMPLE_APPS
    MakeBucket
    RemoveBucket
    BucketExists
    ListBuckets
    StatObject
    RemoveObject
    DownloadObject
    UploadObject
    GetObject
    ListObjects
    PutObject
    CopyObject
    ComposeObject
    RemoveObjects
    SelectObjectContent
    ListenBucketNotification
    DeleteBucketPolicy
    GetBucketPolicy
    SetBucketPolicy
    DeleteBucketNotification
    GetBucketNotification
    SetBucketNotification
    DeleteBucketEncryption
    GetBucketEncryption
    SetBucketEncryption
    GetBucketVersioning
    SetBucketVersioning
    DeleteBucketReplication
    GetBucketReplication
    SetBucketReplication
    DeleteBucketLifecycle
    GetBucketLifecycle
    SetBucketLifecycle
    DeleteBucketTags
    GetBucketTags
    SetBucketTags
    DeleteObjectLockConfig
    GetObjectLockConfig
    SetObjectLockConfig
    DeleteObjectTags
    GetObjectTags
    SetObjectTags
    DisableObjectLegalHold
    EnableObjectLegalHold
    IsObjectLegalHoldEnabled
    GetObjectRetention
    SetObjectRetention
    GetPresignedObjectUrl
    GetPresignedPostFormData
    PutObjectProgress
    GetObjectProgress
  )

  foreach(target ${EXAMPLE_APPS})
    add_executable(${target} examples/${target}.cc)
    target_compile_features(${target} PUBLIC cxx_std_${S3_CPP_STD})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(${target} PRIVATE miniocpp::miniocpp ${S3_CPP_LIBS})
  endforeach()

  add_executable(tests tests/tests.cc)
  target_compile_features(tests PUBLIC cxx_std_${S3_CPP_STD})
  target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
  target_link_libraries(tests miniocpp ${S3_CPP_LIBS})
endif()

# Hanzo S3 C++ Documentation
# -----------------------

if (S3_CPP_MAKE_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else()
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif()
endif()

# Installation Instructions
# -------------------------

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/miniocpp-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/miniocpp-config.cmake"
  INSTALL_DESTINATION
    "{CMAKE_INSTALL_LIBDIR}/cmake/miniocpp"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(TARGETS miniocpp
        EXPORT miniocpp-targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(EXPORT miniocpp-targets
        NAMESPACE miniocpp::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/miniocpp"
        EXPORT_LINK_INTERFACE_LIBRARIES)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/miniocpp-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/miniocpp")

install(FILES
        ${S3_CPP_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/miniocpp")

configure_file(miniocpp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/miniocpp.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/miniocpp.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

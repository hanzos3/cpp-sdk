// Hanzo S3 C++ Library for Amazon S3 Compatible Cloud Storage
// Copyright 2022-2024 Hanzo AI, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef S3_CPP_CONFIG_H_INCLUDED
#define S3_CPP_CONFIG_H_INCLUDED

#define S3_CPP_STRINGIFY(x) #x
#define S3_CPP_TO_STRING(x) S3_CPP_STRINGIFY(x)

#define S3_CPP_MAJOR_VERSION 0
#define S3_CPP_MINOR_VERSION 3
#define S3_CPP_PATCH_VERSION 0

#define S3_CPP_VERSION                                                  \
  "" S3_CPP_TO_STRING(S3_CPP_MAJOR_VERSION) "." S3_CPP_TO_STRING( \
      S3_CPP_MINOR_VERSION) "." S3_CPP_TO_STRING(S3_CPP_PATCH_VERSION)

#if defined(_M_X64) || defined(__x86_64__)
#define S3_CPP_ARCH_STRING "x86_64"
#elif defined(_M_IX86) || defined(__X86__) || defined(__i386__)
#define S3_CPP_ARCH_STRING "x86"
#elif defined(_M_ARM64) || defined(__arm64__) || defined(__aarch64__)
#define S3_CPP_ARCH_STRING "arm64"
#elif defined(_M_ARM) || defined(_M_ARMT) || defined(__arm__) || \
    defined(__thumb__) || defined(__thumb2__)
#define S3_CPP_ARCH_STRING "arm32"
#elif defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
#define S3_CPP_ARCH_STRING "mips64"
#elif defined(_MIPS_ARCH_MIPS32) || defined(_M_MRX000) || defined(__mips__)
#define S3_CPP_ARCH_STRING "mips32"
#elif (defined(__riscv) || defined(__riscv__)) && defined(__riscv_xlen)
#define S3_CPP_ARCH_STRING "riscv" S3_CPP_STRINGIFY(__riscv_xlen)
#elif defined(__loongarch__)
#define S3_CPP_ARCH_STRING "loongarch"
#elif defined(__s390__) || defined(__s390x__)
#define S3_CPP_ARCH_STRING "s390"
#else
#define S3_CPP_ARCH_STRING "unknown-arch"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define S3_CPP_PLATFORM_STRING "windows"
#elif defined(__ANDROID__)
#define S3_CPP_PLATFORM_STRING "android"
#elif defined(__linux__) || defined(__linux)
#define S3_CPP_PLATFORM_STRING "linux"
#elif defined(__APPLE__) || defined(__MACH__)
#define S3_CPP_PLATFORM_STRING "darwin"
#elif defined(__FreeBSD__)
#define S3_CPP_PLATFORM_STRING "freebsd"
#elif defined(__NetBSD__)
#define S3_CPP_PLATFORM_STRING "netbsd"
#elif defined(__OpenBSD__)
#define S3_CPP_PLATFORM_STRING "openbsd"
#else
#define S3_CPP_PLATFORM_STRING "unknown-os"
#endif

#define DEFAULT_USER_AGENT                                       \
  "HanzoS3 (" S3_CPP_PLATFORM_STRING "; " S3_CPP_ARCH_STRING \
  ") hanzo-s3-cpp/" S3_CPP_VERSION ""

#endif  // S3_CPP_CONFIG_H_INCLUDED

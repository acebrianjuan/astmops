# Copyright (C) 2020 Álvaro Cebrián Juan <acebrianjuan@gmail.com>
#
# ASTMOPS is a surveillance performance analyzer
#      for Air Traffic Control
#
# This file is part of ASTMOPS.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.10.2)
project(astmops LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
enable_testing()
add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -Wall -Werror -Wextra -Wmisleading-indentation)


# Code Coverage Configuration
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )

  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif()


# Use GNU standard installation directories.
include(GNUInstallDirs)

add_subdirectory(src)

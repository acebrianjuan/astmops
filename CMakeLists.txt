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
enable_testing()
#add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -Wall -Werror -Wextra -Wmisleading-indentation)

add_subdirectory(src)

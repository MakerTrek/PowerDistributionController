# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(firmware)

# Collect all .src files recursively
file(GLOB_RECURSE app_sources src/*)

target_sources(app PRIVATE ${app_sources})
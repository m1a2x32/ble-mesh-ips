# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(BOARD_FLASH_RUNNER jlink)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE 
        src/main.c
        src/mesh/prov_conf.c
        src/mesh/model_manager.c
        src/mesh/persistant_store_manager.c)

target_include_directories(app PRIVATE include)
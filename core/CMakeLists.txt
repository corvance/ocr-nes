##################################################################################
##                                                                              ##
##  OCR-NES - An NES Emulator written for the OCR A-Level                       ##
##  Computer Science Programming Project.                                       ##
##                                                                              ##
##  Copyright (C) 2021 - 2022 Conaer Macpherson                                 ##
##                                                                              ##
##################################################################################

# File: CMakeLists.txt
# Author: Conaer Macpherson (Candidate No. 6189)
# Brief: The core emulation library CMakeLists.txt.
# Version: 0.1
# Date: 2022-03-22
#
# Copyright (c) 2021 - 2022

cmake_minimum_required(VERSION 3.20)
project(ocrnes-core)

set(CMAKE_CXX_STANDARD 17)

include_directories(./include)

add_library(ocrnes-core
    src/bus.cpp
    src/cartridge.cpp
    src/cpu.cpp
    src/ppu.cpp
    src/mapper.cpp

    src/mappers/mapper_000.cpp
    src/mappers/mapper_001.cpp
    src/mappers/mapper_002.cpp
    src/mappers/mapper_003.cpp
    src/mappers/mapper_004.cpp
    src/mappers/mapper_066.cpp

)
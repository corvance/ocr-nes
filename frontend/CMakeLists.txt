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
# Brief: The frontend CMakeLists.txt.
# Version: 0.1
# Date: 2022-03-22
#
# Copyright (c) 2021 - 2022

cmake_minimum_required(VERSION 3.20)
project(ocrnes-frontend)
set(CMAKE_CXX_STANDARD 17)

# Find SFML shared libraries
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

# Path to libOCRNES.a
set(OCRNES_CORE ${CMAKE_CURRENT_SOURCE_DIR}/../core/build/libocrnes-core.a)

# Dear ImGUI
set(IMGUI_DIR ../libs/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends})

set(IMGUI_SFML_DIR ../libs/imgui-sfml)

include_directories(./include ../core/include ../renderer ${IMGUI_SFML_DIR})

add_executable(ocrnes-frontend
    src/main.cpp
    src/app.cpp

    src/emulator.cpp

    src/gui/gui.cpp

    src/input/bitmask.cpp
    src/input/input.cpp

    src/config.cpp

    # Dear ImGui (and imgui-sfml) Sources.
    ${IMGUI_SFML_DIR}/imgui-SFML.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Link executable to emulator core and SFML.
target_link_libraries(ocrnes-frontend sfml-graphics ${OCRNES_CORE} GL)

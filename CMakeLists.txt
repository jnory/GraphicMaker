# modified from wxWidgets sample. See COPYING.md for details

# Declare the minimum required CMake version
cmake_minimum_required(VERSION 2.8.12)

# Name the project
project(graphic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Request the required wxWidgets libs
find_package(wxWidgets 3.2 COMPONENTS core base REQUIRED CONFIG)
find_package(lodepng CONFIG REQUIRED)

# Include the wxWidgets use file to initialize various settings
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

# Define a variable containing a list of source files for the project
set(SRC_FILES
    gui_main.cpp
    design/gui.cpp
    design/gui.h
    io/bitmap.cpp
    parse/parser.cpp
    shape/equilateral_triangle.cpp
    shape/line.cpp
    shape/point.cpp
    shape/rectangle.cpp
    shape/regular_polygon.cpp
    shape/shape.cpp
    shape/square.cpp
    shape/triangle.cpp
    shape/png_image.cpp
    shape/png_image.hpp
    io/bitmap.hpp
    parse/parser.hpp
    shape/equilateral_triangle.hpp
    shape/line.hpp
    shape/point.hpp
    shape/rectangle.hpp
    shape/regular_polygon.hpp
    shape/shape.hpp
    shape/square.hpp
    shape/triangle.hpp
    parse/reader.cpp
    parse/reader.hpp
    )

if(WIN32)
    set_source_files_properties(
        design/gui.cpp
        PROPERTIES COMPILE_FLAGS /source-charset:utf-8
    )
endif()

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} lodepng)
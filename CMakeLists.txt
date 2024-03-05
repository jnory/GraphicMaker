# modified from wxWidgets sample. See COPYING.md for details

# Declare the minimum required CMake version
cmake_minimum_required(VERSION 3.26)

# Name the project
project(graphic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Request the required wxWidgets libs
find_package(wxWidgets 3.2 COMPONENTS core base REQUIRED CONFIG)
find_package(lodepng CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Include the wxWidgets use file to initialize various settings
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

# Define a variable containing a list of source files for the project
set(SRC_PARSER_FILES
        parse/parser.cpp
        parse/parser.hpp
        parse/lexer.cpp
        parse/lexer.hpp
        parse/commands.hpp
        parse/environment.hpp
        parse/commands.cpp
        parse/statement.hpp
        parse/statement.cpp
        parse/shape_def.hpp
        parse/shape_def.cpp
)
set(SRC_SHAPE_FILES
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
        shape/equilateral_triangle.hpp
        shape/line.hpp
        shape/point.hpp
        shape/rectangle.hpp
        shape/regular_polygon.hpp
        shape/shape.hpp
        shape/square.hpp
        shape/triangle.hpp
        shape/circle.hpp
        shape/circle.cpp
)

set(SRC_DESIGN_FILES
        design/gui.cpp
        design/gui.h
)

set(SRC_BITMAP_FILES
        io/bitmap.cpp
        io/bitmap.hpp
)

set(SRC_CUI_FILES
        ${SRC_BITMAP_FILES}
        ${SRC_SHAPE_FILES}
        ${SRC_PARSER_FILES}
)

set(SRC_FILES
        ${SRC_CUI_FILES}
        ${SRC_DESIGN_FILES}
)

if(WIN32)
    add_compile_options("/source-charset:utf-8")
    add_compile_options("/W4")
    set(CMAKE_DEPFILE_FLAGS_CXX "")
else()
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
endif()

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES} gui_main.cpp)
# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} lodepng)


add_executable(${PROJECT_NAME}_cui ${SRC_CUI_FILES} cui_main.cpp)
target_link_libraries(${PROJECT_NAME}_cui lodepng)

enable_testing()

set(GRAPHIC_TEST_FILES
        tests/test_lexer.cpp
        tests/test_statement.cpp
        tests/test_parser.cpp
        tests/test_command.cpp
)

add_executable(${PROJECT_NAME}_test
        ${GRAPHIC_TEST_FILES} ${SRC_PARSER_FILES} ${SRC_SHAPE_FILES})
target_link_libraries(${PROJECT_NAME}_test GTest::gtest GTest::gtest_main lodepng)
target_include_directories(${PROJECT_NAME}_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME run_${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

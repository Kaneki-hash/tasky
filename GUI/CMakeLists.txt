cmake_minimum_required(VERSION 3.1.0)

project(tasky_gui VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS Core Widgets REQUIRED)


add_executable(tasky_gui
    main_gui.cpp
    TaskyMainWindow.cpp
    TaskyMainWindow.h
    TaskOperations.cpp
)

# Link Qt libraries
target_link_libraries(tasky_gui PRIVATE 
    Qt6::Core
    Qt6::Widgets
)


target_include_directories(tasky_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
)


target_compile_definitions(tasky_gui PRIVATE
    QT_NO_KEYWORDS
)
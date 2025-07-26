cmake_minimum_required(VERSION 3.14)
project(2write LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)

add_executable(2write main.cpp main_window.cpp text_edit.cpp)

target_link_libraries(2write PRIVATE Qt6::Widgets Qt6::Gui)

install(TARGETS 2write
    RUNTIME DESTINATION $ENV{HOME}/.local/bin
)

install(CODE "file(WRITE \"\$ENV{HOME}/.local/share/applications/2write.desktop\" [[
[Desktop Entry]
Name=2write
Exec=2write %F
Icon=accessories-text-editor
MimeType=text/plain;
Type=Application
Terminal=False
Categories=TextEditor;Utility;
]])
    message(STATUS \"Installed 2write.desktop\")
")
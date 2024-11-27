cmake_minimum_required(VERSION 3.5)

project(magicMirror VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Network)

# 头文件 cpp文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    sql/sqlConnect.h
    sql/sqlConnect.cpp
    AI/wav2str.cpp
    AI/wav2str.h
    api/ApiManager.h
    api/ApiManager.cpp
)
set(CMAKE_AUTORCC ON)
qt_add_resources(PROJECT_SOURCES magicMirror_src.qrc)

# opencv
set(OpenCV_DIR /home/ayti/sysroot-RPiOS/usr/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# opencv

# 查找MariaDB库
find_library(MARIADB_LIB mariadb)

#
find_package(SndFile REQUIRED)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(magicMirror
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_finalize_executable(magicMirror)
else()
    if(ANDROID)
        add_library(magicMirror SHARED
            ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(magicMirror
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(magicMirror PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Network ${OpenCV_LIBRARIES} onnxruntime ${MARIADB_LIB} SndFile::sndfile)

set_target_properties(magicMirror PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    # 添加安装指令，将可执行文件部署到远端的 $HOME/Desktop 目录
    install(TARGETS magicMirror
        RUNTIME DESTINATION "$ENV{HOME}/Desktop"
    )
endif()

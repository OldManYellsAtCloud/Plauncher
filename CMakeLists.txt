cmake_minimum_required(VERSION 3.16)

project(Launcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick QuickControls2)
find_package(sdbus-c++)

qt_add_executable(appLauncher
    src/main.cpp
    include/settings.h src/settings.cpp
    include/launchermodel.h src/launchermodel.cpp
    include/visibilityhandler.h src/visibilityhandler.cpp
    include/brightnesshandler.h src/brightnesshandler.cpp
    include/taskhandler.h src/taskhandler.cpp
    include/sway_utils.h src/sway_utils.cpp
    include/direction-client-glue.h
)

qt_add_qml_module(appLauncher
    URI Launcher
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES LauncherDelegate.qml
    QML_FILES TaskDelegate.qml
)

target_link_libraries(appLauncher
    PRIVATE Qt6::Quick Qt6::QuickControls2 sdbus-c++
)

target_include_directories(appLauncher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GNUInstallDirs)
install(TARGETS appLauncher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

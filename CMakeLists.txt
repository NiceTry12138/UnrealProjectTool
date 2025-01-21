cmake_minimum_required(VERSION 3.16)

project(UnrealProjectTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/libgit2/lib)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UnrealProjectTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET UnrealProjectTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(UnrealProjectTool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(UnrealProjectTool
            ${PROJECT_SOURCES}
            SubUI/UpdatePlugins/updateplugindialogue.h SubUI/UpdatePlugins/updateplugindialogue.cpp SubUI/UpdatePlugins/updateplugindialogue.ui
            CommonData.h
            CommonData.h
            Util/unrealhelper.h Util/unrealhelper.cpp
            Util/applicationsettings.h Util/applicationsettings.cpp
            SubUI/UpdatePlugins/gitpathlistwidgetitem.h SubUI/UpdatePlugins/gitpathlistwidgetitem.cpp SubUI/UpdatePlugins/gitpathlistwidgetitem.ui
            Util/gitoperationtool.h Util/gitoperationtool.cpp

        )
    endif()
endif()


# include_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libgit2/include/)

target_link_libraries(UnrealProjectTool PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
# target_link_libraries(UnrealProjectTool PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libgit2/lib/git2.lib
#     ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libgit2/lib/libgit2_clar.lib
#     ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/libgit2/lib/cppgit2.lib
# )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.UnrealProjectTool)
endif()
set_target_properties(UnrealProjectTool PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS UnrealProjectTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UnrealProjectTool)
endif()

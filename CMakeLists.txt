cmake_minimum_required(VERSION 3.16)
project(simpleExamBrowser)

add_subdirectory(src/guard)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        WebEngineWidgets
        REQUIRED)



add_executable(simpleExamBrowser main.cpp
        resources.qrc
        ui/browser.cpp
        ui/browser.h
        ui/browser.ui
)

target_link_libraries(simpleExamBrowser
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::WebEngineWidgets
        guarder
)

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/styles/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/styles/")
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/resources")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/")

        foreach (QT_RESOURCES "icudtl.dat" "qtwebengine_devtools_resources.pak" "qtwebengine_resources.pak" "qtwebengine_resources_100p.pak" "qtwebengine_resources_200p.pak")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/resources/${QT_RESOURCES}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/${QT_RESOURCES}")
            endforeach (QT_RESOURCES)
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/translations/qtwebengine_locales")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations/qtwebengine_locales/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/translations/qtwebengine_locales/en-US.pak"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations/qtwebengine_locales/")
    endif ()
    foreach (QT_RESOURCES "icudtl.dat" "qtwebengine_devtools_resources.pak" "qtwebengine_resources.pak" "qtwebengine_resources_100p.pak" "qtwebengine_resources_200p.pak" "v8_context_snapshot.bin" "v8_context_snapshot.debug.bin")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/resources/${QT_RESOURCES}"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/${QT_RESOURCES}")
    endforeach (QT_RESOURCES)
    foreach (QT_LIB Core Gui Widgets Quick WebEngineWidgets WebEngineCore OpenGL Network PrintSupport QuickWidgets QmlModels Qml WebChannel Positioning)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/QtWebEngineProcess${DEBUG_SUFFIX}.exe"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif ()

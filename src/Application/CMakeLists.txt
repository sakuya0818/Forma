cmake_minimum_required(VERSION 3.5)

project(Application VERSION 0.1 LANGUAGES CXX)

# 显式定义可执行文件名称
set(TARGET_EXE_NAME Forma)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

FILE(GLOB ORIGIN *.cpp *.h)
FILE(GLOB STARTUP Startup/*.h Startup/*.cpp)

source_group(Startup FILES ${STARTUP})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${STARTUP}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_EXE_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
    foreach (filepath ${RES_PATHS})
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
        list(APPEND resource_files ${filename})
    endforeach (filepath)

    qt_add_resources(${TARGET_EXE_NAME} "Application"
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )
else ()
    qt5_add_big_resources(PROJECT_SOURCES
        Application.qrc
    )
    add_executable(${TARGET_EXE_NAME}
        ${PROJECT_SOURCES}
    )
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET_EXE_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET_EXE_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET_EXE_NAME})

target_include_directories(${TARGET_EXE_NAME} PUBLIC
    Startup
)
target_link_libraries(${TARGET_EXE_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ElaWidgetTools
    Utils
    assimp
)

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${TARGET_EXE_NAME})
endif ()
set_target_properties(${TARGET_EXE_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_EXE_NAME})
endif ()

# 添加编译时定义（即预处理器宏）
target_compile_definitions(${TARGET_EXE_NAME}
    PRIVATE $<$<CONFIG:Debug>:DEBUG>
    PRIVATE SPDLOG_FMT_EXTERNAL
)

if (WIN32)
    add_custom_command(TARGET ${TARGET_EXE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:ElaWidgetTools>
            $<TARGET_FILE:Utils>
            $<TARGET_FILE_DIR:${TARGET_EXE_NAME}>
    )
    include(GNUInstallDirs)
    install(TARGETS ${TARGET_EXE_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}
    )
    if (MSVC)
        install(
            FILES $<TARGET_PDB_FILE:${TARGET_EXE_NAME}>
            DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME} OPTIONAL)
    endif ()
    # QT依赖安装 只做Release安装 Debug自行打包
    install(FILES ${QT_SDK_DIR}/plugins/platforms/qwindows.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}/platforms)
    install(FILES ${QT_SDK_DIR}/plugins/generic/qtuiotouchplugin.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}/generic)
    install(
        FILES ${QT_SDK_DIR}/plugins/imageformats/qgif.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qicns.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qico.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qjpeg.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qtga.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qtiff.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qwbmp.dll
        FILES ${QT_SDK_DIR}/plugins/imageformats/qwebp.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}/imageformats
    )
    install(FILES ${QT_SDK_DIR}/plugins/styles/qwindowsvistastyle.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}/styles)
    FILE(GLOB TRANSLATIONS ${QT_SDK_DIR}/translations_install/*.qm)
    install(FILES ${TRANSLATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}/translations)
    install(
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Widgets.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Core.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Gui.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Network.dll
        FILES ${QT_SDK_DIR}/bin/Qt${QT_VERSION_MAJOR}Xml.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${TARGET_EXE_NAME}
    )
endif ()
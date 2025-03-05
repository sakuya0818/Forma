# 指定 CMake 的最低版本要求为 3.5
cmake_minimum_required(VERSION 3.5)

# 定义项目名称为 "Forma"，并设置版本号为 0.1
project(Forma VERSION 0.1 LANGUAGES CXX)

# 启用自动处理ui、元对象代码和资源文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
# 要求必须支持指定的 C++ 标准，否则报错
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt库，要求至少有Widgets组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# 根据查找到的Qt版本设置相应的Qt组件
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 添加子目录 deps/fmt，构建 fmt 库
add_subdirectory(deps/fmt)

# 配置 ASSIMP 库的构建选项
# 禁用默认构建所有导入器
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
# 禁用默认构建所有导出器
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
# 禁用构建 ASSIMP 的测试
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# 禁用构建共享库（默认构建静态库）
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# 启用 COLLADA 导入器
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
# 启用 OBJ 导入器
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
# 启用 FBX 导入器
set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
# 添加子目录 deps/assimp，构建 ASSIMP 库
add_subdirectory(deps/assimp)

# 添加子目录 deps/ElaWidgetTools，构建 ElaWidgetTools 库
add_subdirectory(deps/ElaWidgetTools)

# 设置utils模块源文件
set(DANDELION_UTILS_SOURCES
    src/utils/logger.cpp
)

# 添加源文件
set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/MainWindow.ui
        ${DANDELION_UTILS_SOURCES}
)

# 定义可执行文件，源文件为 PROJECT_SOURCES
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

# 设置头文件包含路径和库文件搜索路径
target_include_directories(${PROJECT_NAME}
    PRIVATE deps
)
target_link_directories(${PROJECT_NAME} PRIVATE deps)
# 链接模块到项目中
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    ElaWidgetTools
    assimp
    fmt::fmt
)
# 添加编译时定义（即预处理器宏）
target_compile_definitions(${PROJECT_NAME}
    PRIVATE $<$<CONFIG:Debug>:DEBUG>
    PRIVATE SPDLOG_FMT_EXTERNAL
)

# 添加 CompilerFlags.cmake 脚本，设置编译器选项
set(current_target ${PROJECT_NAME})
include(${CMAKE_CURRENT_SOURCE_DIR}/CompilerFlags.cmake)

# 设置目标的属性，主要用于 macOS 和 Windows 平台
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif ()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

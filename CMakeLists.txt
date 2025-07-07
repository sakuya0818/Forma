# 指定 CMake 的最低版本要求为 3.5
cmake_minimum_required(VERSION 3.5)

# 项目名称为 "Forma"，版本号为 0.1
project(Forma VERSION 0.1 LANGUAGES CXX)

# 启用自动处理ui、元对象代码和资源文件
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
# 要求必须支持指定的 C++ 标准，否则报错
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#  设置 C++ 编译选项，强制MSVC编译器使用UTF-8编码
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#在这里配置QT路径 例如 D:/Qt/6.6.2/msvc2019_64
SET(QT_SDK_DIR D:/Qt/5.15.2/msvc2019_64 CACHE PATH "QT SDK DIR" FORCE)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install CACHE PATH "Installation path" FORCE)
list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})

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

# 添加子项目 ElaWidgetTools
add_subdirectory(src/ElaWidgetTools)

# 添加子项目 Application
add_subdirectory(src/Application)

# 设置头文件包含路径和库文件搜索路径
target_include_directories(${PROJECT_NAME}
    PRIVATE deps
)

# 为编译assimpku,添加 CompilerFlags.cmake 脚本，设置编译器选项
set(current_target ${PROJECT_NAME})
include(${CMAKE_CURRENT_SOURCE_DIR}/CompilerFlags.cmake)

# 设置目标的属性，主要用于 macOS 和 Windows 平台
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif ()

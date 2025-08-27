cmake_minimum_required(VERSION 3.5)

project(Utils LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 设置为动态库
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME} SHARED)
else ()
    add_library(${PROJECT_NAME} SHARED)
endif ()

# 添加源文件
target_sources(${PROJECT_NAME}
    PRIVATE
        src/Logger.cpp
        include/Logger/Logger.h
)

target_compile_definitions(${PROJECT_NAME} PRIVATE UTILS_LIBRARY)

# 添加头文件路径
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/deps
)

# 链接spdlog，仅限内部使用
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt${QT_VERSION_MAJOR}::Core
)

# 安装规则
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
    LIBRARY DESTINATION ${PROJECT_NAME}/lib
    RUNTIME DESTINATION ${PROJECT_NAME}/bin
)

install(
    DIRECTORY include/
    DESTINATION include
)

# 导出配置（供主程序 find_package 使用）
install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${PROJECT_NAME}/lib/cmake
)

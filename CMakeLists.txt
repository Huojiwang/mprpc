cmake_minimum_required(VERSION 3.0)

project(mprpc)
#可以进行gdb调试
set(CMAKE_BUILD_TYPE "debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

link_directories(${PROJECT_SOURCE_DIR}/lib)
#包含了mprpc框架的所有的相关代码
add_subdirectory(src)
#example包含了mprpc框架使用的示例代码
add_subdirectory(example)
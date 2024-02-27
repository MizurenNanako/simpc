cmake_minimum_required(VERSION 3.5)
project(MyCompilerProject)

# 设置C编译器
set(CMAKE_C_COMPILER gcc) # 可以根据需要更改为其他C编译器

# 设置编译选项
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11") # 可以根据需要添加其他编译选项

# 添加源文件
file(GLOB SOURCES "src/*.c")

# 添加头文件目录
include_directories(include)

# 生成可执行文件
add_executable(my_compiler ${SOURCES})

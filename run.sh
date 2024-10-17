#!/bin/bash

# 检查是否提供了输入文件
if [ "$#" -ne 1 ]; then
    echo "使用方法: $0 <输入文件>"
    exit 1
fi

input_file="$1"
output_file="${input_file}_exe_time.txt"

# 编译 C++ 程序
g++ -mcmodel=medium -Ofast -o solver ./main.cpp

# 确保输出文件为空
> "$output_file"

# 记录开始时间
start_time=$(date +%s.%N)

# 执行 C++ 程序
./solver < "$input_file"

# 记录结束时间
end_time=$(date +%s.%N)

# 计算执行时间
elapsed_time=$(echo "$end_time - $start_time" | bc)

# 将输入文件名和执行时间写入输出文件
echo "Input: $input_file, Time: $elapsed_time seconds" >> "$output_file"

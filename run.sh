#!/bin/bash

# 检查是否提供了输入文件夹
if [ "$#" -ne 1 ]; then
    echo "使用方法: $0 <输入文件夹>"
    exit 1
fi

input_dir="$1"

# 检查输入是否是一个有效的文件夹
if [ ! -d "$input_dir" ]; then
    echo "错误: $input_dir 不是一个有效的文件夹"
    exit 1
fi

# 创建输出文件夹
output_dir="${input_dir}_output"
mkdir -p "$output_dir"

# 编译 C++ 程序
g++ -mcmodel=medium -Ofast -o solver ./main.cpp

# 遍历输入文件夹中的所有文件
for input_file in "$input_dir"/*; do
    # 确保是普通文件
    if [ -f "$input_file" ]; then
        # 获取不带路径的文件名并去掉 .txt 扩展
        filename=$(basename "$input_file")
        filename_without_ext="${filename%%.*}"
        output_file="${output_dir}/${filename_without_ext}_output.txt"

        # 执行 C++ 程序并将输出重定向到文件
        ./solver < "$input_file" > "$output_file"

        # 在终端输出处理信息
        # echo "Processed: $input_file, Output: ${output_file}"
        echo "Processed: $filename, Output: ${output_file}"
    fi
done

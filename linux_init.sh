#!/bin/bash

echo "请输入您的密码以获取管理员权限："
read -s sudo_password
echo "$sudo_password" | sudo -S echo "已获取管理员权限"

echo "$sudo_password" | sudo -S apt-get update
echo "$sudo_password" | sudo -S apt-get install -y g++-multilib gcc-multilib libc6-dev-i386 zlib1g-dev cmake git unzip 

required_dirs=("ecco/tcl" "ecco/tomlplusplus" "ecco/metamod" "ecco/mmlib", "angelscript/metamod", "base/mmlib")
missing_dirs=()

for dir in "${required_dirs[@]}"; do
    if [ ! -d "$dir" ]; then
        missing_dirs+=("$dir")
    fi
done

if [ ${#missing_dirs[@]} -ne 0 ]; then
    echo "检测到缺失的目录：${missing_dirs[*]}"
    echo "正在初始化git子模块..."
    git submodule update --init
fi

echo "所有操作已完成"

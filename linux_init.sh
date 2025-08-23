#!/bin/bash

echo "请输入您的密码以获取管理员权限："
read -s sudo_password
echo "$sudo_password" | sudo -S echo "已获取管理员权限"

echo "$sudo_password" | sudo -S apt-get update
echo "$sudo_password" | sudo -S apt-get install -y g++-multilib gcc-multilib libc6-dev-i386 zlib1g-dev cmake git

required_dirs=("tcl" "tomlplusplus" "metamod" "mmlib")
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

current_dir=$(pwd)
cd tcl/unix || { echo "无法进入tcl/unix目录"; exit 1; }

touch tclStubInit.c tclOOStubInit.c tclOOScript.h
mkdir -p ../build

./configure CFLAGS=-m32 CPPFLAGS=-m32 LDFLAGS=-m32 --disable-64bit --enable-shared --prefix=$(cd ../build;pwd)

make -j4 all
make install

cd "$current_dir" || { echo "无法返回原目录"; exit 1; }

echo "所有操作已完成"

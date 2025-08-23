#!/bin/bash

# 显示版本选择菜单
echo "Chose MeatMod Version："
echo "1) 5.13 (metamod-p, metamod-r)"
echo "2) 5.18 (metamod-fallguys)"

# 读取用户输入
read -p "Chose (1 or 2)：" version_choice

# 根据选择设置对应的参数
case $version_choice in
    1)
        META_INCLUDE="./mmlib/include/metamod"
        META_VERSION="_META_5_13"
        ;;
    2)
        META_INCLUDE="./metamod/metamod"
        META_VERSION="_META_5_18"
        ;;
    *)
        echo "Ivalid，chose 1 or 2"
        exit 1
        ;;
esac

current_dir=$(pwd)

# 编译TCL
cd ecco/tcl/unix || { exit 1; }

touch tclStubInit.c tclOOStubInit.c tclOOScript.h
mkdir -p ../build

./configure CFLAGS=-m32 CPPFLAGS=-m32 LDFLAGS=-m32 --disable-64bit --enable-shared --prefix=$(cd ../build;pwd)

make -j4 all
make install

cd "$current_dir" || { exit 1; }

# 编译ecco核心
mkdir -p ecco/build && cd ecco/build
cmake .. -DMETA_INCLUDE="$META_INCLUDE" -DMETA_VERSION="$META_VERSION" -DCMAKE_BUILD_TYPE=Release
make -j4
cd "$current_dir" || { exit 1; }

# 编译base
mkdir -p base/build && cd base/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
cd "$current_dir" || { exit 1; }

# 编译angelscript
mkdir -p angelscript/build && cd angelscript/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
cd "$current_dir" || { exit 1; }

# 构建package
mkdir -p dist/ecco
cp -f ecco/build/libecco.so dist/ecco/ecco.so

extensionDir="dist/ecco/extensions"
mkdir -p "$extensionDir"
cp -f angelscript/build/libangelscript.so "$extensionDir/"
cp -f base/build/libbase.so "$extensionDir/"

cp -rf ecco/package/lang dist/ecco/
cp -rf ecco/package/scripts dist/ecco/
cp -f ecco/package/Config.toml dist/ecco/

tclDir="dist/ecco/tcl"
mkdir -p "$tclDir"
cp -f ecco/tcl/build/lib/libtcl9.1.so "$tclDir/"

unzip -o "ecco/tcl/unix/libtcl9.1a1.zip" -d "$tclDir/"
mv -f "$tclDir/tcl_library" "$tclDir/lib"

mkdir -p "$tclDir/pkg"

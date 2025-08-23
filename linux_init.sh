#!/bin/bash

echo "password for sudo："
read -s sudo_password
echo "$sudo_password" | sudo -S echo "got password"

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
    git submodule update --init
fi

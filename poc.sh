#!/usr/bin/env sh

# init submodules
git submodule update --init

# build standalone scene viewer
cd src/backend_scene/standalone_view || exit
mkdir -p build && cd build || exit
cmake .. -DBUILD_QML=ON
make -j"$(nproc)"

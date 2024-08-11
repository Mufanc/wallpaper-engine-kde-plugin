#!/usr/bin/env sh

# init submodules
git submodule update --init

# build standalone scene viewer
cd src/backend_scene/standalone_view || exit
mkdir build && cd build || exit
cmake .. -DBUILD_QML=ON
make -j"$(nproc)"

# preview
./sceneviewer-wrapper ~/.local/share/Steam/steamapps/common/wallpaper_engine/assets ~/.local/share/Steam/steamapps/workshop/content/431960/1845706469/scene.pkg

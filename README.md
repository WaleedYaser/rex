# REX

[![](https://img.shields.io/badge/OS-windows--latest-blue)](https://github.com/WaleedYaser/rex)
[![](http://github-actions.40ants.com/WaleedYaser/rex/matrix.svg?only=unit-tests.build.windows-latest)](https://github.com/WaleedYaser/rex)
[![](https://img.shields.io/badge/OS-ubuntu--latest-blue)](https://github.com/WaleedYaser/rex)
[![](http://github-actions.40ants.com/WaleedYaser/rex/matrix.svg?only=unit-tests.build.ubuntu-latest)](https://github.com/WaleedYaser/rex)

[![unit-tests](https://github.com/WaleedYaser/rex/actions/workflows/utests.yml/badge.svg)](https://github.com/WaleedYaser/rex/actions/workflows/utests.yml)
[![deploy-wasm](https://github.com/WaleedYaser/rex/actions/workflows/deploy_wasm.yml/badge.svg)](https://github.com/WaleedYaser/rex/actions/workflows/deploy_wasm.yml)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/WaleedYaser/rex.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/WaleedYaser/rex/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/WaleedYaser/rex.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/WaleedYaser/rex/context:cpp)

[![Project Status: Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](https://www.repostatus.org/badges/latest/wip.svg)](https://www.repostatus.org/#wip)
![Lines of code](https://img.shields.io/tokei/lines/github/WaleedYaser/rex)
![GitHub repo size](https://img.shields.io/github/repo-size/WaleedYaser/rex)

Rex is a side project I'm working on now for the purpose of learning more about graphics programming and rendering. Currently it's just a 3D software rasterizer.

## Supported platforms
- Windows
- Linux
- Web-Assembly (https://waleedyaser.github.io/rex-wasm/)

## Build and install
- Download and install CMake (at least version 3.12): https://cmake.org/download/
- On Linux make sure to install these dependencies:
	```
	sudo apt install -y libxcb1-dev libxcb-util-dev libxcb-keysyms1-dev libxcb-image0-dev
	```

- Configure and build the project by executing the following commands:
	```
	cmake -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release -j
	```
- The output **(rex-viewer.exe)** will be in `build/bin/Release/` directory.
- You can install binaries to any folder by executing the following command:
	```
	cmake --install build --prefix INSTALL_PATH
	```
	replace `INSTALL_PATH` with the path you want.
- You can package the binaries to a zip file using the following steps:
	```
	cd build
	cpack -G ZIP
	```
	You should have `rex-VERSION-win64.zip` file in the build directory.

## External
- https://github.com/doctest/doctest
- https://github.com/nothings/stb

# Resources
- [Kohi Game Engine series on YouTube.](https://www.youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj)
- https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started
- https://www.scratchapixel.com/
- https://www.youtube.com/c/MollyRocket
- https://github.com/RandyGaul/cute_headers
- http://marcelbraghetto.github.io/a-simple-triangle/2019/03/10/part-06/?utm_source=pocket_mylist
- https://github.com/MoustaphaSaad/mn
- https://github.com/raysan5/raylib

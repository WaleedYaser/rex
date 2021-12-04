# REX
[![Project Status: Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](https://www.repostatus.org/badges/latest/wip.svg)](https://www.repostatus.org/#wip)
[![unit-tests](https://github.com/WaleedYaser/rex/actions/workflows/utests.yml/badge.svg)](https://github.com/WaleedYaser/rex/actions/workflows/utests.yml)

Rex is a side project I'm working on now for the purpose of learning more about graphics programming and rendering. Currently it's just a 3D software rasterizer.

## Supported platforms
- Windows

## Build and install
- Download and install CMake (at least version 3.16): https://cmake.org/download/
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

@echo OFF & setlocal & pushd %~dp0
rem build rex-viewer.exe

call ../scripts/utils.bat :clang_build_exec %~dp0 rex-viewer

popd & endlocal
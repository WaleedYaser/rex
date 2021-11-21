@echo OFF & setlocal & pushd %~dp0
rem build rex.dll

call ..\scripts\utils.bat :clang_build_library %~dp0 rex

popd & endlocal
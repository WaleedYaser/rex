@echo OFF & call %*

rem NOTE: not sure what "call %*" do exactly. from my understanding it enables me to call the functions defined in this
rem   file from other files. how?! I'm not sure.

rem make sure we don't call the funcions in this file directly when we reference the file.
rem /B: exit the current batch script instead of exiting terminal.
rem %ERRORLEVEL%: integer number returned by the child process when it terminates. it will equal 0 if process succeeded
rem   and 1 or greated if process encountered an error.
exit /B %ERRORLEVEL%

rem output current clang version
:clang_version
  clang -v
  if %ERRORLEVEL% neq 0 (
    call :echo_error "Failed to find clang compiler. Make sure to install it and that it has been added to the path"
    exit %ERRORLEVEL%
  )
exit /B 0

rem build library dll in debug mode for now
rem parameter1: library src directory
rem parameter2: library name
:clang_build_library
  setlocal enabledelayedexpansion
  rem NOTE: not sure why do we need enabledelayedexpansion

  call :timer_begin

  rem %~1: parameter1
  rem %~2: parameter2
  set folder=%~1
  set name=%~2

  rem make sure all paths are relative to "folder" directory
  echo %folder%
  pushd %folder%

  rem create build directory if it doesn't exist
  set build_dir=..\bin\
  if not exist %build_dir% mkdir %build_dir%

  rem list all cpp files in the "folder" directory
  rem for /R: for all files in the current path
  set cpp_filenames=
  for /R %%f in (*.cpp) do (
    rem NOTE: not sure what variables between two "!" means
    set cpp_filenames=!cpp_filenames! %%f
    echo   %%f
  )

  rem reference for clang flags: https://clang.llvm.org/docs/UsersManual.html
  rem useful resource for clang warnings:
  rem   https://embeddedartistry.com/blog/2017/06/07/warnings-weverything-and-the-kitchen-sink/
  rem -shared: build shared library
  rem -g: generate complete debug info
  rem -gcodeview:
  rem -Werror: turn warnings into errors
  rem -Weverything: enable (litrally) all clang warnings
  rem -Wall: enable decent amount of warning coverage
  rem -Wextra: more warnings
  rem -Wpedantic: all warnings demaded by strict ISO C and ISO C++
  @REM set compiler_flags=-shared -g -Werror -Weverything
  set compiler_flags=-shared -g -Wall -Wextra
  set include_flags=-Isrc
  set linker_flags=-luser32
  set defines=-DREX_DEBUG=1 -DREx_RELEASE=0 -DREX_EXPORT=1 -DREX_IMPORT=0

  clang %cpp_filenames% %compiler_flags% -o %build_dir%%name%.dll %defines% %include_flags% %linker_flags%
  if %ERRORLEVEL% neq 0 (
    call :echo_error "Failed to build %name%.dll with exit code %ERRORLEVEL%"
    exit %ERRORLEVEL%
  )

  call :timer_end total_seconds
  call :echo_success "Build %name%.dll Succeeded (build time: %total_seconds%s)"
  REM empty line
  echo.

  popd & endlocal
exit /B 0

rem build executable file in debug mode and link it to rex.dll for now
rem parameter1: executalble src directory
rem parameter2: executalble name
:clang_build_exec
  setlocal enabledelayedexpansion

  call :timer_begin

  set folder=%~1
  set name=%~2
  set build_dir=..\bin\
  if not exist %build_dir% mkdir %build_dir%

  pushd %folder%
  echo %folder%

  set cpp_filenames=
  for /R %%f in (*.cpp) do (
    set cpp_filenames=!cpp_filenames! %%f
    echo   %%f
  )

  @REM set compiler_flags=-g -Werror -Weverything
  set compiler_flags=-g -Werror -Wall -Wextra
  set include_flags=-Isrc -I../rex/src
  set linker_flags=-L%build_dir% -lrex.lib -luser32 -lGdi32 -lWinmm
  set defines=-DREX_DEBUG=1 -DREX_RELEASE=0 -DREX_EXPORT=0 -DREX_IMPORT=1

  clang %cpp_filenames% %compiler_flags% -o %build_dir%%name%.exe %defines% %include_flags% %linker_flags%
  if %ERRORLEVEL% neq 0 (
    call :echo_error "Failed to build %name%.exe with exit code %ERRORLEVEL%"
    exit %ERRORLEVEL%
  )

  call :timer_end total_seconds
  call :echo_success "Build %name%.exe Succeeded (build time: %total_seconds%s)"
  REM empty line
  echo.

  popd & endlocal
exit /B 0

rem echo error message as error and color it in red
rem parameter1: message
:echo_error
  setlocal
  rem reference for colors: https://stackoverflow.com/a/38617204
  set NOCOLOR=[0m
  set RED=[91m
  echo %RED%Error: %~1%NOCOLOR%
  endlocal
exit /B 0

rem echo message in yellow
rem parameter1: message
:echo_header
  setlocal
  set NOCOLOR=[0m
  set YELLOW=[93m
  echo %YELLOW%%~1%NOCOLOR%
  endlocal
exit /B 0

rem echo message in green
rem parameter1: message
:echo_success
  setlocal
  set NOCOLOR=[0m
  set GREEN=[92m
  echo %GREEN%%~1%NOCOLOR%
  endlocal
exit /B 0


rem begin time calculation
:timer_begin
  rem %TIME%: current system time
  SET begin_time=%TIME%
exit /B 0

rem end time calculation and return time in seconds since calling timer_begin
rem parameter1: out total_seconds
:timer_end
  SET end_time=%TIME%

  rem TODO: understand timer_end function and write proper documentation for it
  set options="tokens=1-4 delims=:.,"
  for /F %options% %%a in ("%begin_time%") do (
    set start_h=%%a
    set /a start_m=100%%b %% 100
    set /a start_s=100%%c %% 100
    set /a start_ms=100%%d %% 100
  )
  for /F %options% %%a in ("%end_time%") do (
    set end_h=%%a
    set /a end_m=100%%b %% 100
    set /a end_s=100%%c %% 100
    set /a end_ms=100%%d %% 100
  )

  set /a hours=%end_h%-%start_h%
  set /a mins=%end_m%-%start_m%
  set /a secs=%end_s%-%start_s%
  set /a ms=%end_ms%-%start_ms%

  if %ms% lss 0 (
    set /a secs = %secs% - 1
    set /a ms = 100%ms%
  )
  if %secs% lss 0 (
    set /a mins = %mins% - 1
    set /a secs = 60%secs%
  )
  if %mins% lss 0 (
    set /a hours = %hours% - 1
    set /a mins = 60%mins%
  )
  if %hours% lss 0 set /a hours = 24%hours%
  if 1%ms% lss 100 set ms=0%ms%

  set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
  set "%~1=%totalsecs%.%ms%"
exit /B 0
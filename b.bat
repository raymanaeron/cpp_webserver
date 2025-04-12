@echo off
setlocal

:: Create root build folder if needed
if exist build_cpp_webserver rmdir /s /q build_cpp_webserver
mkdir build_cpp_webserver
cd build_cpp_webserver

:: Build cpp_webserver target
cmake ../ -G "MinGW Makefiles"
if errorlevel 1 goto :error

mingw32-make
if errorlevel 1 goto :error

echo cpp_webserver build complete.
goto :eof

:error
echo Build failed.
exit /b 1

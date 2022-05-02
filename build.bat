@echo off

set DIR=%cd%
cmake CMakeLists.txt -G "Visual Studio 17 2022" -A x64
cmake -S %DIR% -B %DIR%\build
@REM cmake --build %DIR%\build --config Release
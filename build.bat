@echo off

set DIR=%cd%

cmake -S %DIR% -B %DIR%\build
cmake --build %DIR%\build --config Release
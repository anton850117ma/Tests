@echo off

set DIR=%cd%

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

cmake -S %DIR% -B %DIR%\build
cmake --build %DIR%\build --config Release
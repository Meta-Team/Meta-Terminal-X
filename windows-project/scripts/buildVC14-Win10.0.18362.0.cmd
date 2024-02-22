REM for VS2022
set VCVARDIR="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
REM add /C flag to exit vcvarsall and execute the following commands

REM Original MSVC140(VS2015)
REM call %VCVARDIR% x64 10.0.18362.0 -vcvars_ver=14.0
REM Qt6 requires a C++17 later compiler
REM you can install MSVC v142(VS2019) in the Visual Studio 2022 Installer(maintainer)
call %VCVARDIR% x64 10.0.18362.0 -vcvars_ver=14.29
cmake -GNinja -B ../build .. -DCMAKE_PREFIX_PATH=D:\Qt\6.5.3\msvc2019_64
cmake --build ../build
cmake --install ../build
call "..\build\Meta-Terminal-X.exe"
set VCVARDIR="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
REM add /C flag to exit vcvarsall and execute the following commands
call %VCVARDIR% x64 10.0.18362.0 -vcvars_ver=14.0

cmake -GNinja -B ../build ..
cmake --build ../build
cmake --install ../build
cd ..\build
call "main.exe"
pause
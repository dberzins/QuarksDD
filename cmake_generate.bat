IF EXIST .\build rm -rf build
IF NOT EXIST .\build mkdir build
pushd build
@REM cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake -G "Visual Studio 17 2022" -A Win32 ..
popd

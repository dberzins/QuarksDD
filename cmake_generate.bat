IF EXIST .\build rm -rf build
IF NOT EXIST .\build mkdir build
pushd build
cmake -G "Visual Studio 16 2019" -A Win32 ..
popd

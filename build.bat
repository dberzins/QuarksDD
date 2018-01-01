@echo off

pushd build

REM del *.pdb > NUL 2> NUL
pwd

"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" QuarksDD.sln /project QuarksDD.vcxproj /rebuild Debug  /projectconfig Debug   

popd



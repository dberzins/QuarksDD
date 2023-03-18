@echo off

pushd build

REM del *.pdb > NUL 2> NUL
cd ,

@REM "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv.com" QuarksDD.sln /project Tests.vcxproj /rebuild Debug  /projectconfig Debug   
"C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.com" QuarksDD.sln /project Tests.vcxproj /rebuild Debug  /projectconfig Debug   

popd



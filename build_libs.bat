@echo off

pushd build

REM del *.pdb > NUL 2> NUL
pwd

REM "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" QuarksDD.sln /project QuarksDD_shared.vcxproj /rebuild Debug  /projectconfig Debug   
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" QuarksDD.sln /project QuarksDD_static.vcxproj /rebuild Debug  /projectconfig Debug   
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com" QuarksDD.sln /project INSTALL.vcxproj /rebuild Debug  /projectconfig Debug   

popd



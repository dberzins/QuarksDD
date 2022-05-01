@echo off

pushd build

REM del *.pdb > NUL 2> NUL
cd ,

"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv.com" QuarksDD.sln /project QuarksDD.vcxproj /rebuild Debug  /projectconfig Debug   

popd



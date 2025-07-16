mkdir out\build\x64-Debug\dat
cd out\build\x64-Debug\dat

mklink /D ascii ..\..\..\..\assets\FSW\ascii
mklink /D bgm   ..\..\..\..\assets\FSW\bgm
mklink /D mesh  ..\..\..\..\assets\FSW\mesh
mklink /D sfx   ..\..\..\..\assets\FSW\sfx
mklink /D tex   ..\..\..\..\assets\FSW\tex

cd ..\..\..\..
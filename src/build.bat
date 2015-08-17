@echo off

mkdir ..\..\build\dolphintas
pushd ..\..\build\dolphintas
cl -Od -FC -Zi -EHsc ..\..\dolphintas\src\main.cpp^
 -link User32.lib Gdi32.lib

popd